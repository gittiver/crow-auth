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
#include "crow.h"
#include "crow/middlewares/cookie_parser.h"
#include "crow/middlewares/cors.h"
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

  REQUIRE(user.validate_password("")==false);
  REQUIRE(user.validate_password("x")==false);
}

TEST_CASE("user id")
{
  User user;
  user.id("tester");

  REQUIRE(user.id()=="tester");
}

TEST_CASE("user password hash")
{
  User user;

  const std::string password = "my_user_password";
  user.password(password);

  // we check the hash against rehashed password
  REQUIRE(BCrypt::validatePassword(password,user.hash())==true);

  user.password("");
  // fails on empty password
  REQUIRE(BCrypt::validatePassword(password,user.hash())==false);

  user.password("pwd2");
  REQUIRE(BCrypt::validatePassword("pwd2",user.hash())==true);
}

TEST_CASE("user validate password")
{
  User user;

  const std::string password = "my_user_password";
  user.password(password);

  // we check the hash against rehashed password
  REQUIRE(user.validate_password(password)==true);
  REQUIRE(user.validate_password("wrong password")==false);

  user.password("");
  // fails on empty password
  REQUIRE(user.validate_password(password)==false);

  user.password("pwd2");
  REQUIRE(user.validate_password("pwd2")==true);
  REQUIRE(user.validate_password("not pwd2")==false);
}

TEST_CASE("userdb create")
{
  UserDb& user_db=UserDb::get();
}

TEST_CASE("userdb add_user")
{
  UserDb& user_db=UserDb::get();
  User new_user;
  new_user.id("tester").password("tester_password");
  auto result = user_db.add_user(new_user);
  REQUIRE(result.error() == UserDb::eUserDbResult::OK);
  auto get_new_user = user_db.getUser("tester");

  REQUIRE(get_new_user->id()=="tester");
  REQUIRE(get_new_user->validate_password("tester_password"));
}

TEST_CASE("userdb get_user")
{
  UserDb& user_db=UserDb::get();
  auto user = user_db.getUser("tester");
  REQUIRE(user==nullptr);
}

TEST_CASE("userdb del_user")
{
  UserDb& user_db=UserDb::get();
  User new_user;
  new_user.id("tester").password("tester_password");
  user_db.add_user(new_user);

  // bas case: delete not existing
  auto result = user_db.delete_user("not_existing");
  REQUIRE(result == UserDb::eUserDbResult::NOT_FOUND);

  // good case: delete previously added
  result = user_db.delete_user("tester");
  REQUIRE(result == UserDb::eUserDbResult::OK);
}

TEST_CASE("userdb validate user(name)")
{
  // TODO create function and testcase
CHECK(false);

}

