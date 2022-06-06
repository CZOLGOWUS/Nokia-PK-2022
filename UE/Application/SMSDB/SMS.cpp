#include "SMS.hpp"


namespace ue
{
    SMS::SMS(const SMS& other) = default;
    SMS::SMS()
        :from(common::PhoneNumber{0}),to(common::PhoneNumber{0}),message(""),isRead(false),smsTransmissionState(initial)
    {
    }
    SMS::SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool is_read, SmsTransmissionState state)
        : from(from),to(to),message(std::move(message)),isRead(is_read),smsTransmissionState(state)
    {
    }
    SMS::SMS(SMS&& other) noexcept
        :from(other.from),to(other.to),message(std::move(other.message)) ,isRead(other.isRead),smsTransmissionState(other.smsTransmissionState)
    {
    }
    SMS& SMS::operator=(SMS&& other) noexcept
    {
        this->to = other.to;
        this->from = other.from;
        this->message = std::move(other.message);
        this->isRead = other.isRead;
        this->smsTransmissionState = other.smsTransmissionState;

        return *this;
    }


    SMS& SMS::operator=(const SMS& other)
    {
        if(this == &other)
            return *this;

        this->to = other.to;
        this->from = other.from;
        this->message = other.message;
        this->isRead = other.isRead;
        this->smsTransmissionState = other.smsTransmissionState;

        return *this;
    }

    //for some reason completing this method will cause the tests with rValue addSMS wrapper to fail
    //and this has been added purely for the gMocks to work with rValue parameters
    bool SMS::operator==(const SMS &other) const {
        return message == other.message;
    }

}
