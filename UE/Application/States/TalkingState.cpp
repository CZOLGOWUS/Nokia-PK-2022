#include "TalkingState.hpp"
#include "ConnectedState.hpp"
#include "UeGui/ICallMode.hpp"

namespace ue
{
    TalkingState::TalkingState(Context &context)
    : BaseState(context, "TalkingState"){
        context.user.setAcceptCallback([&]{ handleAcceptOnTalk(); });
        context.user.setRejectCallback([&]{ endCall(); });
        using namespace std::chrono_literals;
        context.timer.startTimer(120000ms);
    }

    void TalkingState::handleTimeout()
    {
        endCall();
    }

    void TalkingState::handleUnknownRecipientAfterCallAccepted()
    {
        context.timer.stopTimer();
        context.user.showPartnerNotAvailable();
        context.user.setAcceptCallback([&]{ handleCallEnded(); });
        context.user.setRejectCallback([&]{ handleCallEnded(); });
    }

    void TalkingState::handleUnknownRecipientAfterCallTalk()
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

    void TalkingState::handleCallDropped(common::PhoneNumber from)
    {
        context.timer.stopTimer();
        context.user.showCallEnded();
        context.user.setAcceptCallback([&]{ handleCallEnded(); });
        context.user.setRejectCallback([&]{ handleCallEnded(); });
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

    void TalkingState::handleAcceptOnTalk()
    {
        using namespace std::chrono_literals;
        context.timer.stopTimer();
        auto& callMode = context.user.initCallMode();
        std::string msg = callMode.getOutgoingText();
        callMode.clearOutgoingText();
        context.bts.sendCallTalk(context.currentCallingStatus.callingNumber, msg);
        context.user.showNewTalkMessage(context.currentCallingStatus.callingNumber, msg, true);
        context.timer.startTimer(120000ms);
    }

    void TalkingState::handleCallTalk(common::PhoneNumber from, std::string msg)
    {
        using namespace std::chrono_literals;
        context.timer.stopTimer();
        context.user.showNewTalkMessage(from, msg, false);
        context.timer.startTimer(120000ms);
    }

    void TalkingState::handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType)
    {
        switch(msgType) {
            case common::MessageId::Sms:
            {
                context.smsDb.addSMS(
                        ue::SMS{
                                context.user.getPhoneNumber(),
                                from ,
                                text,
                                false,
                                Received
                        }
                );
                context.user.showSMSNotification();
                break;
            }
            case common::MessageId::UnknownRecipient:
            {
                auto sms = context.smsDb.getLastSMSSend();
                if(sms) {
                    sms.value()->smsTransmissionState = Bounce;
                    sms.value()->isRead = false;
                    context.user.showSMSNotification();
                }

                break;
            }
            default:{
                break;
            }
        }
    }

}
