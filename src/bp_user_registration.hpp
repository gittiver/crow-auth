//
// Created by Gulliver on 13.03.24.
//

#ifndef CROW_EXAMPLE1_BP_USER_REGISTRATION_HPP
#define CROW_EXAMPLE1_BP_USER_REGISTRATION_HPP

#include "auth.hpp"
#include "auth_user.hpp"
#include "crow/app.h"

struct bp_user_registration : crow::Blueprint {
	struct Settings {
		std::string register_verification_url;

		std::string email_body_template_path;

		bool registration_flow_enabled{ true };
		bool register_verification_enabled{ true };
	};
	bp_user_registration(const crow::App<LoginRequiredMiddleware>& app, AuthDb& authDb);

	bp_user_registration& register_verification_url(const std::string& url);
	const std::string& register_verification_url() const;

	bp_user_registration& registration_flow_enable(const bool enable_register_flow);
	bool is_registration_flow_enabled() const;

	bp_user_registration& register_verification_enable(const bool enable_verification);
	bool is_register_verification_enabled() const;

	std::mutex authdb_mutex;
	Settings settings;
};


#endif //CROW_EXAMPLE1_BP_USER_REGISTRATION_HPP
