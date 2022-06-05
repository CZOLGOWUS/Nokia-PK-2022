#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "SMSDB/BasicSMSDatabase.hpp"
#include "SMSDB/SMS.hpp"
#include "Messages/PhoneNumber.hpp"


using namespace ::testing;

class BasicSMSDBTestSuite : public Test
{
protected:
    const common::PhoneNumber FROM[2] = {101, 102};
    const common::PhoneNumber TO{201};
    const std::string MESSAGES[2] = {"Hello. How are you?", "Test message 2."};

    ue::BasicSMSDatabase objectUnderTest{};
};

struct SingleRecordSMSDBTestSuite : BasicSMSDBTestSuite
{
public:
    SingleRecordSMSDBTestSuite();
};

SingleRecordSMSDBTestSuite::SingleRecordSMSDBTestSuite()
{
    objectUnderTest.addSMS(ue::SMS( FROM[0], TO, MESSAGES[0], false, ue::initial ));
}

TEST_F(SingleRecordSMSDBTestSuite, shallReturnAddedMessage)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).message, MESSAGES[0]);
}

TEST_F(SingleRecordSMSDBTestSuite, shallReturnAddedSourceNumber)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).from, FROM[0]);
}

TEST_F(SingleRecordSMSDBTestSuite, shallReturnAddedDestinationNumber)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).to, TO);
}

struct MultipleRecordsSMSDBTestSuite : BasicSMSDBTestSuite
{
protected:
    MultipleRecordsSMSDBTestSuite();
    const std::string REPLY_MESSAGE = "Reply. Testing long message.";
    const std::string REPLY_SUMMARY_READ = "Reply. Testing ";
};

MultipleRecordsSMSDBTestSuite::MultipleRecordsSMSDBTestSuite()
{
    objectUnderTest.addSMS(ue::SMS(FROM[0], TO, MESSAGES[0],true,ue::Send));
    objectUnderTest.addSMS(ue::SMS( FROM[1], TO, MESSAGES[1],true,ue::Send));
    //reply to FROM[1] number
    objectUnderTest.addSMS(ue::SMS( TO, FROM[1], REPLY_MESSAGE,true));
}

TEST_F(MultipleRecordsSMSDBTestSuite, shallReturnAddedMessages)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).message, MESSAGES[0]);
    ASSERT_EQ(objectUnderTest.getSMS(1).message, MESSAGES[1]);

    ASSERT_EQ(objectUnderTest.getSMS(2).message, REPLY_MESSAGE);
}

TEST_F(MultipleRecordsSMSDBTestSuite, shallReturnAddedSourceNumbers)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).from, FROM[0]);
    ASSERT_EQ(objectUnderTest.getSMS(1).from, FROM[1]);

    ASSERT_EQ(objectUnderTest.getSMS(2).from, TO);
}

TEST_F(MultipleRecordsSMSDBTestSuite, shallReturnAddedDestinationNumbers)
{
    ASSERT_EQ(objectUnderTest.getSMS(0).to, TO);
    ASSERT_EQ(objectUnderTest.getSMS(1).to, TO);

    ASSERT_EQ(objectUnderTest.getSMS(2).to, FROM[1]);
}
