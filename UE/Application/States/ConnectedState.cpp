#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "TalkingState.hpp"

namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
}

void ConnectedState::handleDisconnected()
{
    context.setState<NotConnectedState>();
}

void ConnectedState::handleCallRequest(common::PhoneNumber from)
{
    context.user.showNewCallRequest(from);
}

void ConnectedState::handleSendCallAccept(common::PhoneNumber)
{
    context.setState<TalkingState>();
    //TODO Wysylanie common::MessageId::CallAccepted wiadomości do bts
}

}
