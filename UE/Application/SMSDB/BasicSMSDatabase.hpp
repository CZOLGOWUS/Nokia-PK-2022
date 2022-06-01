#pragma once

#include "ISMSDatabase.hpp"
#include "SMS.hpp"

namespace ue
{

class BasicSMSDatabase : public ISMSDatabase
{
public:
    BasicSMSDatabase();
    void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead = false, SmsTransmissionState state = initial) override;
    void addSMS(SMS& sms) override;
    SMS& getSMS(unsigned int id) override;
    std::optional<std::shared_ptr<SMS>> getLastSMSSend() override;
    const std::vector<std::pair<unsigned int, std::shared_ptr<SMS>>> & getAllSMS() override; //returns summaries

private:
    std::vector<std::pair<unsigned int,std::shared_ptr<SMS>>> data;
    unsigned int nextId;
};

}
