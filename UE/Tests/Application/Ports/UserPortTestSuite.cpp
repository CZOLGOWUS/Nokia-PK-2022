#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include "Ports/UserPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IUeGuiMock.hpp"
#include "SMSDB/SMS.hpp"

namespace ue
{
using namespace ::testing;

class UserPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::PhoneNumber SECOND_PHONE_NUMBER{113};
    const std::string MESSAGES[3] = {"Hello. How are you?", "Test message 2.", "Test message 3."};
    const std::string SUMMARIES_UNREAD[3] = {"*Hello. How are", "*Test message 2", "*Test message 3"};
    const std::string SUMMARIES_SENDING_ERROR_UNREAD[3] = {"SEND ERR: Hello", "SEND ERR: Test ", "SEND ERR: Test "};
    const std::string SUMMARIES_SENDING_ERROR_READ[3] = {"*SEND ERR: Hell", "*SEND ERR: Test", "*SEND ERR: Test"};
    const std::string SUMMARIES_READ[3] = {"Hello. How are ", "Test message 2.", "Test message 3."};
    std::vector<std::pair<unsigned int,std::shared_ptr< SMS>>> smsContainer;

    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserEventsHandlerMock> handlerMock;
    StrictMock<IUeGuiMock> guiMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<ITextModeMock> textModeMock;
    StrictMock<ITextModeMock> alertModeMock;
    StrictMock<IDialModeMock> dialModeMock;
    StrictMock<ICallModeMock> callModeMock;
    StrictMock<ISmsComposeModeMock> smsComposeModeMock;

    UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER};

    UserPortTestSuite()
    {
        smsContainer.emplace_back(0,std::make_unique<SMS>(ue::SMS{PHONE_NUMBER,SECOND_PHONE_NUMBER,MESSAGES[0], false, ue::Received}));
        smsContainer.emplace_back(1,std::make_unique<SMS>(ue::SMS{PHONE_NUMBER,SECOND_PHONE_NUMBER,MESSAGES[1], true, ue::Received}));
        smsContainer.emplace_back(2,std::make_unique<SMS>(ue::SMS{PHONE_NUMBER,SECOND_PHONE_NUMBER,MESSAGES[2], false, ue::Bounce}));

        EXPECT_CALL(guiMock, setTitle(HasSubstr(to_string(PHONE_NUMBER))));
        objectUnderTest.start(handlerMock);
    }
    ~UserPortTestSuite()
    {
        objectUnderTest.stop();
    }
};

TEST_F(UserPortTestSuite, shallStartStop)
{
}

TEST_F(UserPortTestSuite, shallShowNotConnected)
{
    EXPECT_CALL(guiMock, showNotConnected());
    objectUnderTest.showNotConnected();
}

TEST_F(UserPortTestSuite, shallShowConnecting)
{
    EXPECT_CALL(guiMock, showConnecting());
    objectUnderTest.showConnecting();
}

TEST_F(UserPortTestSuite, shallShowMainMenu)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));
    objectUnderTest.showMainMenu();
}

TEST_F(UserPortTestSuite, shallShowSMSList)
{
    EXPECT_CALL(guiMock, showNewSms(false));
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(Exactly(3));
    objectUnderTest.showSMSList(smsContainer);
}

TEST_F(UserPortTestSuite, shallShowSingleSMSOnUserSelect)
{
    EXPECT_CALL(guiMock, showNewSms(false));
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(Exactly(3));

    objectUnderTest.showSMSList(smsContainer);

    EXPECT_CALL(guiMock,setViewTextMode()).WillOnce(ReturnRef(textModeMock));
    EXPECT_CALL(textModeMock, setText(_));

    objectUnderTest.showSMS(*smsContainer.at(0).second);
}

TEST_F(UserPortTestSuite, shallShowSMSCompose)
{
    EXPECT_CALL(guiMock,setSmsComposeMode()).WillOnce(ReturnRef(smsComposeModeMock));

    objectUnderTest.initSmsComposer();
}

TEST_F(UserPortTestSuite, shallInitDialMode)
{
    EXPECT_CALL(guiMock,setDialMode()).WillOnce(ReturnRef(dialModeMock));
    objectUnderTest.initDialMode();
}

TEST_F(UserPortTestSuite, shallShowTalkingMode)
{
    EXPECT_CALL(guiMock,setCallMode()).WillOnce(ReturnRef(callModeMock));
    objectUnderTest.showTalking();
}

TEST_F(UserPortTestSuite, shallShowCalling)
{
    EXPECT_CALL(guiMock,setAlertMode()).WillOnce(ReturnRef(alertModeMock));
    EXPECT_CALL(alertModeMock, setText(_));
    objectUnderTest.showCalling(PHONE_NUMBER);
}

TEST_F(UserPortTestSuite, shallShowNewCallRequest)
{
    EXPECT_CALL(guiMock,setAlertMode()).WillOnce(ReturnRef(alertModeMock));
    EXPECT_CALL(alertModeMock, setText(_));
    objectUnderTest.showNewCallRequest(PHONE_NUMBER);
}

TEST_F(UserPortTestSuite, shallShowPartnerNotAvailable)
{
    EXPECT_CALL(guiMock,setAlertMode()).WillOnce(ReturnRef(alertModeMock));
    EXPECT_CALL(alertModeMock, setText(_));
    objectUnderTest.showPartnerNotAvailable();
}

TEST_F(UserPortTestSuite, shallShowCallDropped)
{
    EXPECT_CALL(guiMock,setAlertMode()).WillOnce(ReturnRef(alertModeMock));
    EXPECT_CALL(alertModeMock, setText(_));
    objectUnderTest.showCallDropped();
}

TEST_F(UserPortTestSuite, shallShowCallEnded)
{
EXPECT_CALL(guiMock,setAlertMode()).WillOnce(ReturnRef(alertModeMock));
EXPECT_CALL(alertModeMock, setText(_));
objectUnderTest.showCallEnded();
}


}
