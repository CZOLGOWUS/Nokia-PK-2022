#pragma once

#include <gmock/gmock.h>
#include "Ports/IUserPort.hpp"
#include "IUeGui.hpp"
#include "IUeGuiMock.hpp"

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;

};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock(IUeGuiMock &userGuiMock);
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    //MOCK_METHOD(IUeGui&, getUserGui, (), (final));
    IUeGui &getUserGui();
    MOCK_METHOD(void, showMainMenu, (), (final));
    MOCK_METHOD(common::PhoneNumber,getPhoneNumber,(),(final));
    MOCK_METHOD(void,showSMSList,(smsContainer&&),(final));
    MOCK_METHOD(void,showSMSList,(smsContainer&),(final));
    MOCK_METHOD(void,showSMS,(ITextMessage&),(final));
    MOCK_METHOD(void,showSMS,(ITextMessage&&),(final));
    MOCK_METHOD(IUeGui::ISmsComposeMode&,initSmsComposer,(),(final));
    MOCK_METHOD(IUeGui::IListViewMode&,initListViewMode,(),(final));
    MOCK_METHOD(IUeGui::ITextMode&,initTextMode,(),(final));
    MOCK_METHOD(PhoneNumber,getInputPhoneNumber,(IUeGui::ISmsComposeMode&),(final));
    MOCK_METHOD(std::string,getInputString,(IUeGui::ISmsComposeMode&),(final));

private:
    IUeGuiMock &gui;
};

}
