#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
private:
    void handleCallEnded ();
public:
    void handleCallRequest(common::PhoneNumber from) override;
    void handleUnknownRecipientAfterCallAccepted() override;
    TalkingState(Context& context);

};

}
