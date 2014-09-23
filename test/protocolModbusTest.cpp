#include "gtest\gtest.h"
#include "protocolModbus.h"


//class MyClassTest: public ::testing::Test {
//protected:
//	uint8_t buf[];
//};
//
//TEST_F(MyClassTest, fallingTest) {
//	ASSERT_TRUE(true);
//}
//
//TEST_F(MyClassTest, myClasssReturnZero) {
////	ASSERT_EQ(0, m.getZero());
//}


class MyClassTest: public ::testing::Test {
protected:
	static const uint8_t SIZE_BUF = 255;
	uint8_t buf[SIZE_BUF];
};

TEST_F(MyClassTest, getMaxNumRegisters) {
	TProtocolModbus tmp(buf, SIZE_BUF);

	ASSERT_EQ(32, tmp.getMaxNumRegisters());
}

TEST_F(MyClassTest, myClasssReturnZero) {

}

class MyClassTest1: public ::testing::Test {
};

TEST_F(MyClassTest1, fallingTest) {
	ASSERT_TRUE(true);
}

TEST_F(MyClassTest1, myClasssReturnZero) {
}
