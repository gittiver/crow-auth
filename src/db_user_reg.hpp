#ifndef CROW_DB_USER_REG_HPP
#define CROW_DB_USER_REG_HPP


#pragma once
#include "auth_user.hpp"

UserRegistration* get_user_registration(const std::string &connection_url);

#endif // #ifndef CROW_DB_USER_REG_HPP
