#ifndef CROW_AUTH_USER_HPP
#define CROW_AUTH_USER_HPP
#pragma once
#include <memory>
#include <string>
#include <tl/expected.hpp>

#include "auth.hpp"

class User {
	std::string id_;
	std::string salted_password_hash_;

public:
	User() = default;
	virtual ~User() = default;

	const std::string& id() const { return this->id_; }
	User& id(const std::string& id) { this->id_ = id; return *this; }
	
	User& password(const std::string& password);

	const std::string& hash() const {
		return salted_password_hash_;
	}

	bool validate_password(const std::string& password) const;
};

class Bearer {
	std::string id_;
public:
	const std::string& id() const { return this->id_; }
	Bearer& id(const std::string& id) { this->id_ = id; return *this; }

	std::optional<User> get_user() const;
};

struct AuthDb {
	static AuthDb& get();

	enum class eAuthDbResult {
		OK,
		NOT_VALID,
		NOT_FOUND
	};

	std::shared_ptr<User> getUser(const std::string& name);
	tl::expected<User*,eAuthDbResult> add_user(User& user);
	eAuthDbResult delete_user(const std::string& user_id);

	void init();

	std::shared_ptr<Bearer> get_bearer(const std::string& name);

protected:
	AuthDb() = default;
	virtual ~AuthDb() = default;
};

class AuthDbAuth : public IAuthenticate {
	std::string connection_url_;

	std::mutex mutex_;
public:
	AuthDbAuth& connection(const std::string& connection_url)
	{ this->connection_url_ = connection_url; return *this; }

	void on_init() override { AuthDb::get().init();};
	static AuthDb& user_db() { return AuthDb::get(); };

	bool is_user_authenticated(const std::string &username, const std::string &password) override;

	bool is_bearer_authenticated(const std::string &bearer) override;
};
#endif // #ifndef CROW_AUTH_USER_HPP
