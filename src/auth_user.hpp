#ifndef CROW_AUTH_USER_HPP
#define CROW_AUTH_USERHPP
#pragma once
#include <memory>
#include <string>

class User {
	std::string id_;
	std::string salt_;
	std::string salted_password_hash_;

public:
	User() = default;
	virtual ~User() = default;

	const std::string& id() const { return this->id_; }
	User& id(const std::string& id) { this->id_ = id; return *this; }
	
	User& password(const std::string& password)
	{
		std::string value2hash{salt_};
		value2hash.append(password);
		salted_password_hash_ = hash(value2hash);
		return *this;
	}

	const std::string& hash() const {
		return salted_password_hash_;
	}

	std::string password_hash(const std::string& password) const {
		std::string value2hash{ salt_ };
		value2hash.append(password);
		return this->hash(value2hash);
	}

private: 
	std::string hash(const std::string& value2hash) const {
		// TODO integrate cryptographic hash function (bcrypt?)
		return "hash_" + value2hash;
	}

};

struct UserDb {
	static UserDb& get();

	std::shared_ptr<User> getUser(const std::string& name);
};
#endif // #ifndef CROW_AUTH_USER_HPP
