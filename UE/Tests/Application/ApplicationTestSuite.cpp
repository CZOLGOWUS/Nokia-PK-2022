#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/ISMSDatabaseMock.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::PhoneNumber NUMBER{101};
    const common::BtsId BTS_ID{42};
    const common::PhoneNumber SENDER_NUMBER{111};

    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    NiceMock<ISMSDatabaseMock> smsDbMock;

    Expectation expectNoTConnected = EXPECT_CALL(userPortMock, showNotConnected());

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock,
                                smsDbMock};
};




struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{
    void requestAttachOnSib();
};

void ApplicationNotConnectedTestSuite::requestAttachOnSib()
{
    EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
    EXPECT_CALL(timerPortMock, startTimer(_));
    EXPECT_CALL(userPortMock, showConnecting());

    objectUnderTest.handleSib(BTS_ID);
}

TEST_F(ApplicationNotConnectedTestSuite, shallRequestAttachOnSib)
{
    requestAttachOnSib();
}




struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    ApplicationConnectingTestSuite();
    void requestConnectOnAttachRequest();
};

void ApplicationConnectingTestSuite::requestConnectOnAttachRequest()
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showMainMenu());
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));

    objectUnderTest.handleAttachAccept();
}

ApplicationConnectingTestSuite::ApplicationConnectingTestSuite()
{
    //preparation
    requestAttachOnSib();
}




TEST_F(ApplicationConnectingTestSuite, shallCompleteAttachWhenAttachAccepted)
{
    requestConnectOnAttachRequest();
}

TEST_F(ApplicationConnectingTestSuite, shallFaillAttachWhenAttachRejected)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showNotConnected());

    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallFailAttachOnTimeOut)
{
    EXPECT_CALL(userPortMock, showNotConnected());

    objectUnderTest.handleTimeout();
}





struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{

    const std::string MESSAGES[2] = {"Hello. How are you?", "Test message 2."};
    const std::string SUMMARIES_UNREAD[2] = {"*Hello. How are", "*Test message 2"};
    const std::string SUMMARIES_READ[2] = {"Hello. How are ", "Test message 2."};

    ApplicationConnectedTestSuite();
    void handleCallRequest(common::PhoneNumber number);
    void requestAcceptOnCallRequest(common::PhoneNumber number);
};

ApplicationConnectedTestSuite::ApplicationConnectedTestSuite()
{
    //preparation
    requestConnectOnAttachRequest();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleDisconnect)
{
    EXPECT_CALL(userPortMock, showNotConnected());

    objectUnderTest.handleDisconnected();
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReceivingSMS)
{
    EXPECT_CALL(userPortMock, showSMSNotification());
    EXPECT_CALL(userPortMock, getPhoneNumber());
    EXPECT_CALL(smsDbMock, addSMS_rvr( ue::SMS{SENDER_NUMBER,PHONE_NUMBER ,MESSAGES[0],false,initial}));

    objectUnderTest.handleSMS(SENDER_NUMBER,MESSAGES[0],common::MessageId::Sms);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleRecevingBouncedSMSWithNoSMSs)
{
    EXPECT_CALL(smsDbMock, getLastSMSSend());

    objectUnderTest.handleSMS(SENDER_NUMBER,MESSAGES[0],common::MessageId::UnknownRecipient);
}



void ApplicationConnectedTestSuite::requestAcceptOnCallRequest(common::PhoneNumber number)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showTalking());
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    EXPECT_CALL(timerPortMock, startTimer(_));
    objectUnderTest.handleCallAccepted(number);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{

    using namespace std::chrono_literals;
    EXPECT_CALL(userPortMock, showNewCallRequest(NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(30000ms));
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    objectUnderTest.handleCallRequest(NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallAccepted)
{
    requestAcceptOnCallRequest(NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallDropped)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showCallDropped());
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    objectUnderTest.handleCallDropped(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleUnknownRecipientAfterCallRequest)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showPartnerNotAvailable());
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    objectUnderTest.handleUnknownRecipientAfterCallRequest();
}

struct ApplicationTalkingTestSuite : ApplicationConnectedTestSuite
{
    ApplicationTalkingTestSuite();
};

ApplicationTalkingTestSuite::ApplicationTalkingTestSuite()
{
    //preparation
    requestAcceptOnCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleUnknownRecipientAfterCallAccepted)
{
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showPartnerNotAvailable());
    EXPECT_CALL(userPortMock, setAcceptCallback(_));
    EXPECT_CALL(userPortMock, setRejectCallback(_));
    objectUnderTest.handleUnknownRecipientAfterCallAccepted();
}

TEST_F(ApplicationTalkingTestSuite, shallRejectCallRequestDuringTalking)
{
    EXPECT_CALL(btsPortMock, sendCallDropped(PHONE_NUMBER));
    objectUnderTest.handleCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleUnknownRecipientAfterCallTalk)
{
EXPECT_CALL(timerPortMock, stopTimer());
EXPECT_CALL(userPortMock, showPartnerNotAvailable());
EXPECT_CALL(userPortMock, setAcceptCallback(_));
EXPECT_CALL(userPortMock, setRejectCallback(_));
objectUnderTest.handleUnknownRecipientAfterCallAccepted();
}

TEST_F(ApplicationTalkingTestSuite, shallHandleDroppingCall)
{
EXPECT_CALL(timerPortMock, stopTimer());
EXPECT_CALL(userPortMock, showCallEnded());
EXPECT_CALL(userPortMock, setAcceptCallback(_));
EXPECT_CALL(userPortMock, setRejectCallback(_));
objectUnderTest.handleCallDropped(PHONE_NUMBER);
}

TEST_F(ApplicationTalkingTestSuite, shallHandleCallTalk)
{
EXPECT_CALL(timerPortMock, stopTimer());
EXPECT_CALL(userPortMock, showNewTalkMessage(PHONE_NUMBER, "msg", _));
EXPECT_CALL(timerPortMock, startTimer(_));

objectUnderTest.handleCallTalk(PHONE_NUMBER, "msg");
}



}