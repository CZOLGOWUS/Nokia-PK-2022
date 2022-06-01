#pragma once

#include <gmock/gmock.h>
#include "SMSDB/ISMSDatabase.hpp"

namespace ue {

class ISMSDatabaseMock : public ISMSDatabase{
public:
    ISMSDatabaseMock();
    ~ISMSDatabaseMock() override;

    MOCK_METHOD(void, addSMS,(common::PhoneNumber,common::PhoneNumber,std::string,bool,SmsTransmissionState),(final));
    MOCK_METHOD(SMS&, getSMS,(unsigned int),(final));
    MOCK_METHOD((std::vector<std::pair<unsigned int, std::shared_ptr<SMS>>>&), getAllSMS,(),(final));
    MOCK_METHOD(void,addSMS,(SMS&),(final));
    MOCK_METHOD((std::optional<std::shared_ptr<SMS>>),getLastSMSSend,(),(final));
};

}