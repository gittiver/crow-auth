#include "auth_user.hpp"
#include <memory>
#include <openssl/blowfish.h>
#include "bcrypt/BCrypt.hpp"


User& User::password(const std::string& password) {
	salted_password_hash_ = BCrypt::generateHash(password);
	return *this;
}

bool User::validate_password(const std::string& password) const {
	return BCrypt::validatePassword(password, salted_password_hash_);
}

AuthDb& AuthDb::get() {
	static AuthDb instance;
	return instance;
}

std::shared_ptr<User> AuthDb::getUser(const std::string& name) {
	const auto result = std::find_if(users.begin(), users.end(),
		[&name](auto user) -> bool { return user->id() == name; });
	return (result == users.end()) ? nullptr : *result;
}

tl::expected<std::shared_ptr<User>, AuthDb::eAuthDbResult> AuthDb::register_user(const User& user, bool verified) {
	// check unique names first 
	std::string name = user.id();
	const auto result = std::find_if(users.begin(), users.end(),
		[&name](auto user) -> bool { return user->id() == name; });
	// only add if name not found
	if (result != users.end()) {
		return tl::unexpected(AuthDb::eAuthDbResult::NOT_VALID);
	}
	else {


		std::shared_ptr<User> nuser = std::make_shared<User>(user);
		/// TODO replace token by cryptographical safe generated one
		nuser->verification_token("abc");
		users.emplace_back(nuser);
		this->store(user);
		return nuser;
	}
}

void AuthDb::verify_token(const std::string& token, const std::string& email, const std::chrono::time_point<std::chrono::system_clock>& timestamp)
{
	//check for mail adress, token, timestamp valid and not yet activated
	const auto result = std::find_if(users.begin(), users.end(),
		[&token,&email](auto user) -> bool { 
			return !user->verified() && user->email() == email && user->verification_token()==token
		/* TBD && timestamp*/ ; });
	if (result != users.end()) {
		(*result)->verified(true);
	} 
}

AuthDb::eAuthDbResult AuthDb::delete_user(const std::string& user_id) {
	auto result = std::remove_if(users.begin(),
		users.end(),
		[&user_id](std::shared_ptr<User> user) -> bool { return user->id() == user_id; });
	if (result != users.end()) {
		users.erase(result);
		return AuthDb::eAuthDbResult::OK;
	}
	return AuthDb::eAuthDbResult::NOT_FOUND;
}

void AuthDb::store(const User& user)
{
	for (auto user : users) {
		
	}

}

void AuthDb::init() {
	// TODO implement
	// TODO add tests
}

std::shared_ptr<Bearer> AuthDb::get_bearer(const std::string& name) {
	// TODO implement
	// TODO add tests
	auto bearer = std::make_shared<Bearer>();
	return bearer;
}

bool AuthDbAuth::is_user_authenticated(const std::string& username, const std::string& password) {
	std::scoped_lock<std::mutex> lock{ mutex_ };
	auto user = AuthDb::get().getUser(username);
	if (!user) {
		return false;
	}
	else {
		return user->verified() && user->validate_password(password);
	}
}

bool AuthDbAuth::is_bearer_authenticated(const std::string& name) {
	std::scoped_lock<std::mutex> lock{ mutex_ };
	auto bearer = AuthDb::get().get_bearer(name);
	return bearer != nullptr;
}
