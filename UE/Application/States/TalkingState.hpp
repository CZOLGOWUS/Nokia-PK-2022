#pragma once

#include "BaseState.hpp"

namespace ue
{
class TalkingState : public BaseState
{
private:
    void handleCallEnded ();
    void endCall();
public:
    void handleCallRequest(common::PhoneNumber from) override;
    void handleUnknownRecipientAfterCallAccepted() override;
    void handleCallDropped(common::PhoneNumber) override;
    TalkingState(Context& context);

};

}
