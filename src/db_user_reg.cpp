#include "db_user_reg.hpp"
#include <sqlpp11/sqlite3/sqlite3.h>
#include "sqlpp11/select.h"

namespace sql = sqlpp::sqlite3;

struct DatabaseUserRegistration : UserRegistration {
  DatabaseUserRegistration();
  virtual ~DatabaseUserRegistration() = default;

  std::shared_ptr<User> getUser(const std::string& name) override;
  tl::expected<std::shared_ptr<User>, eResult> add_user(const User& user) override;
  eResult delete_user(const std::string& user_id) override;

  void init() override;

  std::shared_ptr<Bearer> get_bearer(const std::string &name) override;
private:
  std::unique_ptr<sql::connection> connection_;
};

DatabaseUserRegistration::DatabaseUserRegistration() {
  sql::connection_config config;
  config.path_to_database = ":memory:";
  config.flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
  config.debug = true;
  connection_ = std::make_unique<sql::connection>(config);
  connection_->execute("CREATE TABLE users");
}

std::shared_ptr<User> DatabaseUserRegistration::getUser(const std::string &name) {
  const auto result = std::find_if(users.begin(), users.end(),
                                   [&name](auto user) -> bool { return user->id() == name; });
  return (result == users.end()) ? nullptr : *result;
}

tl::expected<std::shared_ptr<User>, DatabaseUserRegistration::eResult>
DatabaseUserRegistration::add_user(const User &user) {
  // check unique names first
  std::string name = user.id();
  const auto result = std::find_if(users.begin(), users.end(),
                                   [&name](auto user) -> bool { return user->id() == name; });
  // only add if name not found
  if (result != users.end()) {
    return tl::unexpected(UserRegistration::eResult::NOT_VALID);
  } else {
    std::shared_ptr<User> nuser = std::make_shared<User>(user);
    users.emplace_back(nuser);
    return nuser;
  }
}

DatabaseUserRegistration::eResult DatabaseUserRegistration::delete_user(const std::string &user_id) {
  auto result = std::remove_if(users.begin(),
                               users.end(),
                               [&user_id](std::shared_ptr<User> user) -> bool { return user->id() == user_id; });
  return (result != users.end()) ? UserRegistration::eResult::OK : DatabaseUserRegistration::eResult::NOT_FOUND;
}

void DatabaseUserRegistration::init() {
  // TODO implement
  // TODO add tests
}

std::shared_ptr<Bearer> DatabaseUserRegistration::get_bearer(const std::string &name) {
  // TODO implement
  // TODO add tests
  auto bearer = std::make_shared<Bearer>();
  return bearer;
}

