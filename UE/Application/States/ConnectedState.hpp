#pragma once

#include "BaseState.hpp"
#include "Constants/MenuConstans.h"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ISmsComposeMode.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
private:
    void handleMainMenu();
    void handleSMSList();
    void handleComposeSMSView();

    void handleAcceptOnMainMenu();
    void handleAcceptOnSMSList();
    void handleSMSView(unsigned int elemIndex);
    void handleAcceptOnComposeSMSView(IUeGui::ISmsComposeMode& smsComposer);

    void handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType) override;

    void handleTimeout() override;

public:
    ConnectedState(Context& context);
    void handleDisconnected() override;

    void handleCallRequest(common::PhoneNumber) override;

    void handleSendCallAccept(common::PhoneNumber) override;
    void handleSendCallReject(common::PhoneNumber) override;

};

}
