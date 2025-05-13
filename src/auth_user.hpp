#ifndef CROW_AUTH_USER_HPP
#define CROW_AUTH_USER_HPP
#pragma once
#include <memory>
#include <optional>
#include <string>
#include <tl/expected.hpp>

#include "auth.hpp"

class User {
  std::string id_;
  std::string salted_password_hash_;
  std::string email_;

public:
  User() = default;

  User(const std::string &id, const std::string &password_, const std::string &email_ = "")
    : id_(id)
      , email_(email_) { password(password_); };

  virtual ~User() = default;

  const std::string &id() const { return this->id_; }

  User &id(const std::string &id) {
    this->id_ = id;
    return *this;
  }

  User & email(const std::string &email) {
    this->email_ = email;
    return *this;
  }

  const std::string& email() const { return this->email_; }

  User &password(const std::string &password);

  const std::string &hash() const {
    return salted_password_hash_;
  }

  bool validate_password(const std::string &password) const;
};

class Bearer {
	std::string id_;
	std::shared_ptr<User> user;
public:
	const std::string& id() const { return this->id_; }
	Bearer& id(const std::string& id) { this->id_ = id; return *this; }

	std::shared_ptr<User> get_user() const { return user;  };
};

struct AuthDb {
	static	std::unique_ptr<AuthDb> get(const std::string& connection);

  enum class eAuthDbResult {
    OK,
    NOT_VALID,
    NOT_FOUND
  };

	std::shared_ptr<User> getUser(const std::string& name);
	tl::expected<std::shared_ptr<User>, AuthDb::eAuthDbResult> add_user(const User& user);
	eAuthDbResult delete_user(const std::string& user_id);

  void init();

  std::shared_ptr<Bearer> get_bearer(const std::string &name);
  virtual ~AuthDb() = default;

protected:
	AuthDb() = default;
private:
	std::vector<std::shared_ptr<User>> users;
	std::vector<Bearer> bearers;
};

class AuthDbAuth : public IAuthenticate {
  std::string connection_url_;

  std::mutex mutex_;

  std::unique_ptr<AuthDb> authDb;

public:
  AuthDbAuth &connection(const std::string &connection_url) {
	if (this->connection_url_ != connection_url) {
		this->connection_url_ = connection_url;
		on_init();
	}
    return *this;
  }

  void on_init() override 
  { 
	  authDb = AuthDb::get(connection_url_);  
  }
  
  AuthDb* user_db() { return authDb.get(); }

  bool is_user_authenticated(const std::string &username, const std::string &password) override;

  bool is_bearer_authenticated(const std::string &bearer) override;
};
#endif // #ifndef CROW_AUTH_USER_HPP
