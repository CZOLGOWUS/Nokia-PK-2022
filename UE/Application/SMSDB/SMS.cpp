#include "SMS.hpp"


namespace ue
{
    SMS::SMS()
    {
        this->from = common::PhoneNumber{0};
        this->to = common::PhoneNumber{0};
        this->smsTransmissionState = initial;
        this->message = "";
        this->isRead = false;
    }

    SMS::SMS(common::PhoneNumber from, common::PhoneNumber to, std::string message, bool is_read, SmsTransmissionState state)
        : from(from),to(to),message(std::move(message)),isRead(is_read),smsTransmissionState(state)
    {
    }

    SMS::SMS(const SMS& other) = default;

    SMS::SMS(SMS&& other) noexcept
        :to(other.to),from(other.from),message(std::move(other.message)) ,isRead(other.isRead),smsTransmissionState(other.smsTransmissionState)
    {
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

}
