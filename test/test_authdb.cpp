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

TEST_CASE("userdb create")
{
	AuthDb& user_db = AuthDb::get();
}

TEST_CASE("userdb register_user")
{
	AuthDb& user_db = AuthDb::get();

	auto deleted = user_db.delete_user("tester");

	User new_user;
	new_user.id("tester").password("tester_password").email("x@y.com");

	auto result = user_db.register_user(new_user);

	REQUIRE(result.has_value());
	REQUIRE(result.value()->id() == "tester");
	REQUIRE(result.value()->validate_password("tester_password"));
	REQUIRE(result.value()->email() == "x@y.com");
	REQUIRE(!result.value()->verified());
	REQUIRE(!result.value()->verification_token().empty());
}

TEST_CASE("userdb get_user")
{
	AuthDb& user_db = AuthDb::get();
	auto user = user_db.getUser("does not exist");

	REQUIRE(!user);
}

TEST_CASE("userdb del_user")
{
	AuthDb& user_db = AuthDb::get();
	auto result = user_db.delete_user("tester");

	User new_user;
	new_user.id("tester").password("tester_password");
	auto registered = user_db.register_user(new_user);
	if (registered.has_value()) {

	} else {
		auto error = registered.error();
	}

	// bas case: delete not existing
	result = user_db.delete_user("not_existing");
	REQUIRE(result == AuthDb::eAuthDbResult::NOT_FOUND);

	// good case: delete previously added
	result = user_db.delete_user("tester");
	REQUIRE(result == AuthDb::eAuthDbResult::OK);
}

TEST_CASE("userdb validate user(name)")
{
	AuthDb& user_db = AuthDb::get();
	User new_user;
	auto _ = user_db.delete_user("tester");

	new_user.id("tester").password("tester_password").email("x@y.com");
	auto sucessfully_registered_user =  user_db.register_user(new_user);
	REQUIRE(sucessfully_registered_user.has_value());
	REQUIRE(sucessfully_registered_user.value()->id()=="tester");
	
	auto registration_invalid_user = user_db.register_user(new_user);
	REQUIRE(registration_invalid_user.error() == AuthDb::eAuthDbResult::NOT_VALID);
}

