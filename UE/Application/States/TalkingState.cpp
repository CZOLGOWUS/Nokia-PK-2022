#include "TalkingState.hpp"
#include "ConnectedState.hpp"

namespace ue
{
    TalkingState::TalkingState(Context &context)
    : BaseState(context, "TalkingState"){
//        context.user.setAcceptCallback([&]{ handleCallEnded(); });
        context.user.setRejectCallback([&]{ endCall(); });
    }

    void TalkingState::handleUnknownRecipientAfterCallAccepted()
    {
        context.timer.stopTimer();
        context.user.showPartnerNotAvailable();
        context.user.setAcceptCallback([&]{ handleCallEnded(); });
        context.user.setRejectCallback([&]{ handleCallEnded(); });
    }

    void TalkingState::handleCallRequest(common::PhoneNumber from)
    {
        context.bts.sendCallDropped(from);
    }

    void TalkingState::handleCallDropped(common::PhoneNumber)
    {
        context.timer.stopTimer();
        handleCallEnded();
    }

    void TalkingState::handleCallEnded()
    {
        context.currentCallingStatus.callingNumber.value = 0;
        context.currentCallingStatus.isOutgoingCall = false;
        context.setState<ConnectedState>();
    }

    void TalkingState::endCall()
    {
        context.bts.sendCallDropped(context.currentCallingStatus.callingNumber);
        context.timer.stopTimer();
        handleCallEnded();
    }

}
