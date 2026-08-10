#define CROW_ENABLE_DEBUG
#define CROW_LOG_LEVEL 0
#include <sys/stat.h>

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <type_traits>
#include <regex>

#include "catch2/catch_all.hpp"

#include "crow/middlewares/session.h"

#include "auth_user.hpp"
#include "bcrypt/BCrypt.hpp"


using namespace std;
using namespace crow;

#ifdef CROW_USE_BOOST
namespace asio = boost::asio;
using asio_error_code = boost::system::error_code;
#else
using asio_error_code = asio::error_code;
#endif

#define LOCALHOST_ADDRESS "127.0.0.1"

TEST_CASE("user create")
{
	User user;
	REQUIRE(user.id().empty());
	REQUIRE(user.hash().empty());
	REQUIRE(user.email().empty());
	REQUIRE(user.verified()==false);

	REQUIRE(user.validate_password("") == false);
	REQUIRE(user.validate_password("x") == false);
}

TEST_CASE("user id")
{
	User user;
	user.id("tester");

	REQUIRE(user.id() == "tester");
}

TEST_CASE("user password hash")
{
	User user;

	const std::string password = "my_user_password";
	user.password(password);

	// we check the hash against rehashed password
	REQUIRE(BCrypt::validatePassword(password, user.hash()) == true);

	user.password("");
	// fails on empty password
	REQUIRE(BCrypt::validatePassword(password, user.hash()) == false);

	user.password("pwd2");
	REQUIRE(BCrypt::validatePassword("pwd2", user.hash()) == true);
}

TEST_CASE("user email")
{
	User user;

	user.email("x@y.com");
	REQUIRE(user.email()=="x@y.com");
}

TEST_CASE("user validate password")
{
	User user;

	const std::string password = "my_user_password";
	user.password(password);

	// we check the hash against rehashed password
	REQUIRE(user.validate_password(password) == true);
	REQUIRE(user.validate_password("wrong password") == false);

	user.password("");
	// fails on empty password
	REQUIRE(user.validate_password(password) == false);

	user.password("pwd2");
	REQUIRE(user.validate_password("pwd2") == true);
	REQUIRE(user.validate_password("not pwd2") == false);
}

