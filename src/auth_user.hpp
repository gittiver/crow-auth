#ifndef CROW_AUTH_USER_HPP
#define CROW_AUTH_USER_HPP
#pragma once
#include <memory>
#include <string>
#include <tl/expected.hpp>

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

struct UserDb {
	static UserDb& get();

	enum class eUserDbResult {
		OK,
		NOT_VALID,
		NOT_FOUND
	};

	std::shared_ptr<User> getUser(const std::string& name);
	tl::expected<User*,eUserDbResult> add_user(User& user);
	eUserDbResult delete_user(const std::string& user_id);

protected:
	UserDb() = default;
	virtual ~UserDb() = default;
};
#endif // #ifndef CROW_AUTH_USER_HPP
