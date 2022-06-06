#pragma once

#include "Messages/PhoneNumber.hpp"

namespace
{
#define MAX_SUMMARY_SIZE 15
}

namespace ue{

enum SmsTransmissionState
{
    initial,
    Send,
    Received,
    Bounce
};

struct SMS
{
    SMS();
    SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead, SmsTransmissionState state = initial);
    SMS(const SMS &sms);
    SMS(SMS&& sms) noexcept;
    SMS &operator =(const SMS &sms);
    SMS& operator=(SMS&& other) noexcept;
    bool operator==(const SMS& sms) const;

    std::string message;
    common::PhoneNumber from{};
    common::PhoneNumber to{};
    bool isRead = false;
    SmsTransmissionState smsTransmissionState;
};

}
