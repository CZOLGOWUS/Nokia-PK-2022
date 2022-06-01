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

class SMS
{
public:
    SMS();
    SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool isRead, SmsTransmissionState state = initial);
    SMS(const SMS &sms);
    SMS &operator =(const SMS &sms);

    std::string getMessage();
    common::PhoneNumber getFromNumber();
    common::PhoneNumber getToNumber();
    void setSMSTransmissionState(SmsTransmissionState state);
    SmsTransmissionState getSMSTransmissionState();
    void setIsReadStatus(bool status);
    bool getIsReadStatus();

private:
    std::string message;
    common::PhoneNumber from;
    common::PhoneNumber to;
    bool isRead = false;
    SmsTransmissionState smsTransmissionState;
};

}
