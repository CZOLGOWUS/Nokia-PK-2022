#include "Application.hpp"
#include "States/NotConnectedState.hpp"


namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer,
                         ISMSDatabase& smsDb)
    : context{iLogger, bts, user, timer, smsDb},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleDisconnected()
{
    context.state->handleDisconnected();
}

void Application::handleSMS(common::PhoneNumber from, std::string text, common::MessageId msgType)
{
    context.state->handleSMS(from, text, msgType);
}

void Application::handleCallRequest(common::PhoneNumber from)
{
    context.state->handleCallRequest(from);
}

void Application::handleCallAccepted(common::PhoneNumber from)
{
    context.state->handleCallAccepted(from);
}

void Application::handleCallDropped(common::PhoneNumber from)
{
    context.state->handleCallDropped(from);
}

void Application::handleUnknownRecipientAfterCallRequest()
{
    context.state->handleUnknownRecipientAfterCallRequest();
}

void Application::handleUnknownRecipientAfterCallAccepted()
{
    context.state->handleUnknownRecipientAfterCallAccepted();
}

void Application::handleUnknownRecipientAfterCallTalk()
{
    context.state->handleUnknownRecipientAfterCallTalk();
}

void Application::handleCallTalk(common::PhoneNumber from, std::string msg)
{
    context.state->handleCallTalk(from, msg);
}

}
