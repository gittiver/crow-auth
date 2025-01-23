#include "auth_user.hpp"
#include <memory>
#include <openssl/blowfish.h>
#include "bcrypt/BCrypt.hpp"

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
