#include "BasicSMSDatabase.hpp"

namespace ue
{


    BasicSMSDatabase::BasicSMSDatabase()
        : nextId{0}
    {
    }

    void BasicSMSDatabase::addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead, SmsTransmissionState state )
    {
        data.emplace_back(
                nextId++, std::make_unique<SMS>(from,to,message,isRead,state)
                );
    }

    void BasicSMSDatabase::addSMS(SMS& sms)
    {
        data.emplace_back(
                nextId++, std::make_unique<SMS>(sms.getFromNumber(),sms.getToNumber(),sms.getMessage(),sms.getIsReadStatus(),sms.getSMSTransmissionState())
        );
    }

    SMS& BasicSMSDatabase::getSMS(unsigned int id)
    {
        return *data.at(id).second;
    }

    const std::vector<std::pair<unsigned int, std::shared_ptr< SMS>>>& BasicSMSDatabase::getAllSMS()
    {
        return data;
    }

    std::optional<std::shared_ptr<SMS>> BasicSMSDatabase::getLastSMSSend()
    {
        for (size_t i = data.size() - 1; i > 0; --i)
        {
            std::shared_ptr< SMS>& sms = data[i].second;
            if(sms->getSMSTransmissionState() == Send)
                return sms;
        }
        return {};
    }
}
