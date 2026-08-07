//
// Created by Gulliver on 13.03.24.
//

#include "bp_user_registration.hpp"

#include <tl/expected.hpp>

#include "auth.hpp"

auto to_string(const char* pszValue) -> std::string {
    if (pszValue == nullptr) return {};
    return { pszValue };
}

struct user_register {

  std::string name;
  std::string email;
  std::string password;
  bool activated{false};

  enum class eError { eOk, eFail};

  static tl::expected<user_register, eError> from_body_params(const crow::query_string& body_params) {
      user_register res;

      res.name = body_params.get("name");
      res.email = body_params.get("email");
      
      res.password = to_string(body_params.get("password"));
      
      if (res.name.empty()|| res.email.empty() || res.password.empty()) {
          return tl::unexpected<eError>(eError::eFail);
      } else {
        return res;
      }
  }
  static tl::expected<user_register, eError> from_json(const crow::json::rvalue &json) {
    if (!json.has("name") || !json.has("email") || !json.has("password")) {
      return tl::unexpected<eError>(eError::eFail);
    } else {
      user_register res;
      res.name = json["name"].s();
      res.email = json["email"].s();
      res.password = json["password"].s();
      return res;
    }
  }
};

bp_user_registration::bp_user_registration(const crow::App<LoginRequiredMiddleware>& app, AuthDb& authDb)
    : crow::Blueprint("user") {

CROW_BP_ROUTE((*this), "/api/register")
.methods(crow::HTTPMethod::Post)
      ([this,&authDb](crow::request request) {
    crow::response resp;
    if (!settings.registration_flow_enabled) {
        resp.code = crow::status::SERVICE_UNAVAILABLE;
    }
    else {
        tl::expected<user_register, user_register::eError> user_registration;
        if (request.get_header_value("Content-Type") == "application/x-www-form-urlencoded")
        {
            user_registration = user_register::from_body_params(request.get_body_params());
        
        }
        else {
            crow::json::rvalue json = crow::json::load(request.body);
            user_registration = user_register::from_json(json);
        }
        if (!user_registration) {
            CROW_LOG_ERROR << "Invalid user registration request";
            resp.code = crow::status::BAD_REQUEST;
        }
        else {
            asio::detail::scoped_lock<std::mutex> lock(authdb_mutex);
            auto registered = authDb.getUser(user_registration->name);
            if (registered) {
                resp.code = crow::status::CONFLICT;
            }
            else {
                User u;
                u.id(user_registration->name)
                 .password(user_registration->password)
                 .email(user_registration->email);

                auto added = authDb.register_user(u);
                if (!added) {
                    resp.code = crow::status::INTERNAL_SERVER_ERROR;
                }
                else {
                    resp.code = crow::status::OK;
                }
            }
        }
    }
    return resp;
  });

CROW_BP_ROUTE((*this), "/api/verify-registration")
.methods(crow::HTTPMethod::Get)
([&authDb](crow::request& request) {
    //request to https ://backend-host/api/v1/accounts/verify-registration/ via HTTP POST with following JSON payload:

    //{
    //    "user_id": "<user id>",
    //        "timestamp" : "<timestamp>",
    //        "signature" : "<signature>"
    //}
    crow::response response;

    auto token = request.url_params.get("token");
    auto email = request.url_params.get("email");
    if (token == nullptr) {
        response.code = crow::status::BAD_REQUEST;
    }
    else {
        auto timestamp = std::chrono::system_clock::now();
        authDb.verify_token(token,email,timestamp);
        response.code = crow::status::OK;
    }
    return response;
});


CROW_BP_ROUTE((*this), "/api/user")
  .CROW_MIDDLEWARES(app,LoginRequiredMiddleware)
  ([] {
            //...
            return crow::response(crow::status::NOT_IMPLEMENTED);
          });
}

bp_user_registration& bp_user_registration::register_verification_url(const std::string& url)
{
    settings.register_verification_url=url;
    return *this;
}

const std::string& bp_user_registration::register_verification_url() const
{
    return settings.register_verification_url;
}

bp_user_registration& bp_user_registration::registration_flow_enable(const bool enable_register_flow) 
{
    settings.registration_flow_enabled = enable_register_flow;
    return *this;
}
bool bp_user_registration::is_registration_flow_enabled() const
{
    return settings.register_verification_enabled;
}

bp_user_registration& bp_user_registration::register_verification_enable(const bool enable_verification)
{
    settings.registration_flow_enabled = enable_verification;
    return *this;
}

bool bp_user_registration::is_register_verification_enabled() const
{
    return settings.registration_flow_enabled;
}
