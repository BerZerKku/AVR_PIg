#include "gtest/gtest.h"
#include "security/pwd.h"
#include <cstdio>
#include <iostream>
using namespace std;

// размер массива
#define SIZE_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))

class TPwd_Test: public ::testing::Test {

protected:
	TPwd *pwd;

	virtual void SetUp() {
		pwd = new TPwd();

	};

	virtual void TearDown() {
		delete pwd;
	};

	const uint8_t pwdReset[PWD_LEN] =  {0};
};

TEST_F(TPwd_Test, constant) {
	ASSERT_EQ(600, kTickToDecCounter);
	ASSERT_EQ(1, kTickWait);
	ASSERT_EQ(5, kTickToRestPwd);
	ASSERT_EQ(8, PWD_LEN);
	ASSERT_EQ(0, memcmp(PWD_DEFAULT, "00000000", PWD_LEN));
	ASSERT_EQ(0, memcmp(PWD_RESET, "00000001", PWD_LEN));
}

TEST_F(TPwd_Test, getSet) {
	uint8_t pwdEng[] = "12345678";
	uint8_t pwdAdm[] = "55432198";
	uint8_t pwdError1[] = "a5432198";
	uint8_t pwdError2[] = "1543219A";

	// состояние после сброса
	ASSERT_EQ(NULL, pwd->getPwd(USER_operator));
	ASSERT_EQ(0, memcmp(pwdReset, pwd->getPwd(USER_engineer), PWD_LEN));
	ASSERT_EQ(0, memcmp(pwdReset, pwd->getPwd(USER_admin), PWD_LEN));
	ASSERT_EQ(NULL, pwd->getPwd(USER_MAX));
	ASSERT_EQ(NULL, pwd->getPwd(USER_factory));

	// установка пароля
	ASSERT_FALSE(pwd->setPwd(USER_operator, pwdEng));
	ASSERT_TRUE(pwd->setPwd(USER_engineer, pwdEng));
	ASSERT_TRUE(pwd->setPwd(USER_admin, pwdAdm));
	ASSERT_FALSE(pwd->setPwd(USER_MAX, pwdAdm));
	ASSERT_FALSE(pwd->setPwd(USER_factory, pwdEng));

	// проверка пароля
	ASSERT_EQ(NULL, pwd->getPwd(USER_operator));
	ASSERT_EQ(0, memcmp(pwdEng, pwd->getPwd(USER_engineer), PWD_LEN));
	ASSERT_EQ(0, memcmp(pwdAdm, pwd->getPwd(USER_admin), PWD_LEN));
	ASSERT_EQ(NULL, pwd->getPwd(USER_MAX));
	ASSERT_EQ(NULL, pwd->getPwd(USER_factory));

	// установка пароля содержащего недопустимый символ
	ASSERT_FALSE(pwd->setPwd(USER_engineer, pwdError1));
	ASSERT_EQ(0, memcmp(pwdEng, pwd->getPwd(USER_engineer), PWD_LEN));
	ASSERT_FALSE(pwd->setPwd(USER_admin, pwdError2));
	ASSERT_EQ(0, memcmp(pwdAdm, pwd->getPwd(USER_admin), PWD_LEN));
}

TEST_F(TPwd_Test, checkPassword) {
	uint8_t pwdEng[] = "12345678";
	uint8_t pwdAdm[] = "55432198";
	uint8_t pwdError1[] = "a5432198";

	// при включении не пароль по умолчанию

	ASSERT_FALSE(pwd->checkPassword(USER_admin, (uint8_t *) PWD_DEFAULT));

	// у оператора всегда true
	ASSERT_TRUE(pwd->checkPassword(USER_operator, NULL));
	ASSERT_TRUE(pwd->checkPassword(USER_operator, pwdEng));
	ASSERT_TRUE(pwd->checkPassword(USER_operator, pwdError1));

	// инженер
	ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdReset));
	ASSERT_TRUE(pwd->setPwd(USER_engineer, pwdEng));
	ASSERT_FALSE(pwd->checkPassword(USER_engineer, NULL));
	ASSERT_FALSE(pwd->checkPassword(USER_engineer, pwdReset));
	ASSERT_FALSE(pwd->checkPassword(USER_engineer, (uint8_t *) PWD_DEFAULT));
	// FIXME пользователь заблокирован!
	ASSERT_TRUE(pwd->checkPassword(USER_engineer, pwdEng));
}
