/**
 * \file osdpreaderunitconfiguration.hpp
 * \author Maxime C. <maxime-dev@islog.com>
 * \brief STidSTR Reader unit configuration.
 */

#ifndef LOGICALACCESS_OSDPREADERUNITCONFIGURATION_HPP
#define LOGICALACCESS_OSDPREADERUNITCONFIGURATION_HPP

#include <logicalaccess/readerproviders/readerunitconfiguration.hpp>
#include <logicalaccess/plugins/readers/osdp/lla_readers_osdp_api.hpp>
#include <logicalaccess/cards/aes128key.hpp>

namespace logicalaccess
{
/**
 * \brief The OSDP reader unit configuration base class.
 */
class LLA_READERS_OSDP_API OSDPReaderUnitConfiguration : public ReaderUnitConfiguration
{
  public:
    /**
     * \brief Constructor.
     */
    OSDPReaderUnitConfiguration();

    /**
     * \brief Destructor.
     */
    virtual ~OSDPReaderUnitConfiguration();

    /**
     * \brief Reset the configuration to default values
     */
    void resetConfiguration() override;

    /**
     * \brief Serialize the current object to XML.
     * \param parentNode The parent node.
     */
    void serialize(boost::property_tree::ptree &parentNode) override;

    /**
     * \brief UnSerialize a XML node to the current object.
     * \param node The XML node.
     */
    void unSerialize(boost::property_tree::ptree &node) override;

    /**
     * \brief Get the default Xml Node name for this object.
     * \return The Xml node name.
     */
    std::string getDefaultXmlNodeName() const override;

    /**
     * \brief Get the reader RS485 address.
     * \return The RS485 address.
     */
    unsigned char getRS485Address() const;

    /**
     * \brief Set the reader RS485 address.
     * \param address The RS485 address.
     */
    void setRS485Address(unsigned char address);


    /**
     * \brief Get the key used for AES enciphering.
     * \return The key.
     */
    std::shared_ptr<AES128Key> getMasterKey() const;

    /**
     * \brief Set the key used for AES enciphering.
     * \param key The key.
     */
    void setMasterKey(std::shared_ptr<AES128Key> key);

    std::shared_ptr<AES128Key> getSCBKKey() const;

    void setSCBKKey(std::shared_ptr<AES128Key> key);

    std::shared_ptr<AES128Key> getSCBKDKey() const;

    void setSCBKDKey(std::shared_ptr<AES128Key> key);

  protected:
    /**
     * \brief The reader RS485 address (if communication type RS485 used).
     */
    unsigned char d_rs485Address;

    /**
     * \brief The key used for AES enciphering.
     */
    std::shared_ptr<AES128Key> d_master_key_aes;

    std::shared_ptr<AES128Key> d_scbk_key_aes;

    std::shared_ptr<AES128Key> d_scbk_d_key_aes;
};
}

#endif
