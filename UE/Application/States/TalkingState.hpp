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
    void handleUnknownRecipientAfterCallTalk() override;
    void handleCallDropped(common::PhoneNumber) override;
    void handleCallTalk(common::PhoneNumber from, std::string msg) override;
    void handleAcceptOnTalk();
    void handleTimeout() override;
    void handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType) override;
    TalkingState(Context& context);

};

}
