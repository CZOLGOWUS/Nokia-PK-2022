#pragma once

#include <vector>

#include "Messages/PhoneNumber.hpp"
#include "SMSDB/SMS.hpp"
#include <memory>
#include <optional>

namespace ue
{

class ISMSDatabase{
public:
    virtual ~ISMSDatabase() = default;

    virtual void addSMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead = false, SmsTransmissionState state = initial) = 0;
    virtual void addSMS(SMS& sms) = 0;
    virtual SMS& getSMS(unsigned int id) = 0;
    virtual std::optional<std::shared_ptr<SMS>> getLastSMSSend() = 0;
    virtual const std::vector<std::pair<unsigned int, std::shared_ptr<SMS>>>& getAllSMS() = 0;
};

}
