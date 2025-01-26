//
// Created by Gulliver on 13.03.24.
//

#ifndef CROW_EXAMPLE1_BP_USER_REGISTRATION_HPP
#define CROW_EXAMPLE1_BP_USER_REGISTRATION_HPP

#include "auth.hpp"
#include "crow/app.h"

struct bp_user_registration : crow::Blueprint {
  bp_user_registration(const crow::App<LoginRequiredMiddleware>& app);
};


#endif //CROW_EXAMPLE1_BP_USER_REGISTRATION_HPP
