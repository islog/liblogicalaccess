#include <gtest/gtest.h>
#include <logicalaccess/dynlibrary/librarymanager.hpp>
#include "logicalaccess/bufferhelper.hpp"
#include "logicalaccess/plugins/crypto/signature_helper.hpp"
#include "logicalaccess/iks/RemoteCrypto.hpp"

using namespace logicalaccess;

static std::string pubkey = "-----BEGIN PUBLIC KEY-----\n\
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwZ1H25sOi39JYwAlV0Tz\n\
d/Srw+ecCfdzSjYT5TjlLAPLoIVJYNS1zQeqfBYf1l5MdnwZcKz+rHSfKdljgcZF\n\
tIvJ+5+B0gf2YJY97eCY3BQocmuhnIKD8+msrQudMlSKE8k4pzaRWbZJe1PD053y\n\
2KoyiZXAkIxxEHxW84cLHvonyQn8MqswBrLVW8g77Ndui0rvLHVRb0BMy7smFSGI\n\
D9tIvRkHPm6vrKo2gq2dvQfH6ApttFObYi75kuOxNaS+xUbipB+TMzklhT35z7dC\n\
El75fICdJ5f9ZY58Mpf4UJ6LPu8tBkTFekeZiKHUE3kAkVLukcSBpDTTmvcUX1CZ\n\
jQIDAQAB\n\
-----END PUBLIC KEY-----\n";

TEST(test_signature, verify1)
{
    auto blob = BufferHelper::getStdString(
        BufferHelper::fromHexString("0A10000010920A438C66000000000000000010021A10D43DEBC7"
                                    "D61B4127A01EC5B1EA3923C820BCA1A083A92C"));
    auto signature = BufferHelper::getStdString(BufferHelper::fromHexString(
        "4D9C69814ED33793E2FB5528AEFD651A3F933C76F4943ECE3DFD62D3A471530BFA5753EF46760F13"
        "9E560095B2E3C4FB0486A1D8686473F69C4F4911BDBCB833A75FBF5782E240B16A91FA64A152F2E7"
        "E6E72EBBC2AABF1CFC3887CD9583018272C6F600E25E0D87010C0260EACAB7C159403DA88725AF94"
        "E3503E5D97A98E7F01EFC9C10E0BEA5E52E85812F5B8BB4814D85A76EAD916391ED3E198CA00AA57"
        "34588178F4F922AC2DB354D067A7331C89ECC1F3189E689314DFD6302DA37D5803D2ACF9A0D54D70"
        "36BBC3E055B6687B95349F1DFE2C2C859FD32AC2DF33CBD2B812DAA668EA43E4F9B103E0E9A0D54A"
        "D78F1EB682F83C6744F021FD0907B19F"));
    ASSERT_TRUE(SignatureHelper::verify_sha512(blob, signature, pubkey));

    auto invalid_signature  = signature;
    invalid_signature.at(0) = '3';
    ASSERT_FALSE(SignatureHelper::verify_sha512(blob, invalid_signature, pubkey));
}

TEST(test_signature, verify_iks)
{
    // The first IKS test needs to setup the global configuration
    // otherwise we cannot instanciate a remote crypto object.
    iks::IslogKeyServer::configureGlobalInstance(
        "iksf", 6565, "/home/xaqq/Documents/iks/crypto/certs/my-client-1.pem",
        "/home/xaqq/Documents/iks/crypto/certs/my-client-1.key",
        "/home/xaqq/Documents/iks/crypto/certs/iks-server-intermediate-ca.pem");

    auto remote_crypto = LibraryManager::getInstance()->getRemoteCrypto();
    SignatureResult sigres;
    sigres.signature = BufferHelper::fromHexString(
        "3A2D7B0847493F53E841A0402AFCC25E914E55F48BD7597BCED08DD7BF76C01B14733115F2075DFB"
        "C3CD7F2EFA8DF7033F4118D9FDE7846E387FAD15B6850A32EA283A51A77BF201C52BD540DF255F30"
        "FE6B0503B80D3C7619BCE1539921CA1BB15797B6302F224E08E5614DFF04F34B4B88D50DE24AF851"
        "ED17D032203FAF74993345592C6601EA7C0052103BABDBE643FC563B449FD0C68CCFE67B7AD31A33"
        "2DE1641F0F601028D266CC5B514CBF35EDACE03DF8F3AEFEAD7B42143134992B78A996FF4E280C2D"
        "737BDA9550018A19CA97AA11BF9D23C86B337D5143A148D08C2F556B2A4F960F7FB3DA58733C0328"
        "2558536D09599DED45833F4C5C36781E");
    sigres.desc.payload = BufferHelper::fromHexString("000010920A438C660000000000000000");
    sigres.desc.run_uuid =
        BufferHelper::fromHexString("D43DEBC7D61B4127A01EC5B1EA3923C8");
    sigres.desc.nonce     = 3;
    sigres.desc.timestamp = 1522842692598;

    ASSERT_TRUE(remote_crypto->verify_signature(sigres, pubkey));
}

TEST(test_signature, verify_iks_fails)
{
    auto remote_crypto = LibraryManager::getInstance()->getRemoteCrypto();
    SignatureResult sigres;
    sigres.signature = BufferHelper::fromHexString(
        "3A2D7B0847493F53E841A0402AFCC25E914E55F48BD7597BCED08DD7BF76C01B14733115F2075DFB"
        "C3CD7F2EFA8DF7033F4118D9FDE7846E387FAD15B6850A32EA283A51A77BF201C52BD540DF255F30"
        "FE6B0503B80D3C7619BCE1539921CA1BB15797B6302F224E08E5614DFF04F34B4B88D50DE24AF851"
        "ED17D032203FAF74993345592C6601EA7C0052103BABDBE643FC563B449FD0C68CCFE67B7AD31A33"
        "2DE1641F0F601028D266CC5B514CBF35EDACE03DF8F3AEFEAD7B42143134992B78A996FF4E280C2D"
        "737BDA9550018A19CA97AA11BF9D23C86B337D5143A148D08C2F556B2A4F960F7FB3DA58733C0328"
        "2558536D09599DED45833F4C5C36781E");
    sigres.desc.payload = BufferHelper::fromHexString("000010920A438C660000000000000000");
    sigres.desc.run_uuid =
        BufferHelper::fromHexString("D43DEBC7D61B4127A01EC5B1EA3923C8");
    sigres.desc.nonce     = 3;
    sigres.desc.timestamp = 1522842692599;

    ASSERT_FALSE(remote_crypto->verify_signature(sigres, pubkey));
}
