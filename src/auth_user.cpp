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
UserDb& UserDb::get()
{
	static UserDb instance;
	return instance;
}

std::shared_ptr<User> UserDb::getUser(const std::string& name)
{
	// TODO create useful user object	
	return std::shared_ptr<User>();
}

tl::expected<User*,UserDb::eUserDbResult> UserDb::add_user(User& user) {
	return tl::make_unexpected(UserDb::eUserDbResult::NOT_VALID);
}

UserDb::eUserDbResult UserDb::delete_user(const std::string& user_id) {
	return UserDb::eUserDbResult::NOT_FOUND;
}
