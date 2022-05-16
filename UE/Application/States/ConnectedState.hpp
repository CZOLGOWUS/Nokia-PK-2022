#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnected() override;
    void handleCallRequest(common::PhoneNumber) override;

    void handleSendCallAccept(common::PhoneNumber) override;
    void handleSendCallReject(common::PhoneNumber) override;
};

}
