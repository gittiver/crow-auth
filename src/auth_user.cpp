#include "auth_user.hpp"
#include <memory>
#include <openssl/blowfish.h>
#include "bcrypt/BCrypt.hpp"


User& User::password(const std::string& password)
{
	salted_password_hash_ = BCrypt::generateHash(password);
	return *this;
}

bool User::validate_password(const std::string& password) const {
	return BCrypt::validatePassword(password, salted_password_hash_);
}
AuthDb& AuthDb::get()
{
	static AuthDb instance;
	return instance;
}

std::shared_ptr<User> AuthDb::getUser(const std::string& name)
{
	// TODO create useful user object	
	return std::shared_ptr<User>();
}

tl::expected<User*,AuthDb::eAuthDbResult> AuthDb::add_user(User& user) {
	return tl::make_unexpected(AuthDb::eAuthDbResult::NOT_VALID);
}

AuthDb::eAuthDbResult AuthDb::delete_user(const std::string& user_id) {
	return AuthDb::eAuthDbResult::NOT_FOUND;
}

void AuthDb::init() {
	// TODO implement
	// TODO add tests

}

std::shared_ptr<Bearer> AuthDb::get_bearer(const std::string &name) {
	// TODO implement
	// TODO add tests
	auto bearer = std::make_shared<Bearer>();
	return bearer;
}

bool AuthDbAuth::is_user_authenticated(const std::string &username, const std::string &password) {
	std::scoped_lock<std::mutex> lock{mutex_};
	auto user = AuthDb::get().getUser(username);
	if (!user) {
		return false;
	}
	else {
		return user->validate_password(password);
	}
}

bool AuthDbAuth::is_bearer_authenticated(const std::string &name) {
	std::scoped_lock<std::mutex> lock{mutex_};
	auto bearer = AuthDb::get().get_bearer(name);
	return bearer!=nullptr;
}
