//
// Created by Gulliver on 13.03.24.
//

#include "bp_user_registration.hpp"

#include <tl/expected.hpp>

#include "auth.hpp"


struct user_register {

  std::string name;
  std::string email;
  std::string password;

  enum class eError { eOk, eFail};

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
        crow::json::rvalue json = crow::json::load(request.body);
        auto user_registration = user_register::from_json(json);
        if (!user_registration) {
          CROW_LOG_ERROR << "Invalid user registration JSON request" ;
          return crow::status::BAD_REQUEST;
        } else {
          asio::detail::scoped_lock<std::mutex> lock(authdb_mutex);
          auto registered = authDb.getUser(user_registration->name);
          if (registered) {
            return crow::status::CONFLICT;
          } else {
            User u;
            u.id(user_registration->name)
            .password(user_registration->password)
            .email(user_registration->email);

            auto added = authDb.add_user(u);
            if (!added) {
               return crow::status::INTERNAL_SERVER_ERROR;
            } else {
              return crow::status::OK;
            }
          }
        }
        // ...
      });

CROW_BP_ROUTE((*this), "/api/logout")
  .CROW_MIDDLEWARES(app,LoginRequiredMiddleware)
  ([]() {
        // ...
        return crow::response(crow::status::OK);
      });


/*CROW_BP_ROUTE((*this), "/register")
.methods(crow::HTTPMethod::Get)
([]() {
    // ...
    return crow::response(crow::status::OK,"GET register_body");
    });

CROW_BP_ROUTE((*this), "/register")
.methods(crow::HTTPMethod::Post)
      ([]() {
        // ...
        return crow::response(crow::status::OK);
      });

CROW_BP_ROUTE((*this), "/profile/<int>")
  .CROW_MIDDLEWARES(app,LoginRequiredMiddleware)
  ([](int /*id#1#) {
            //...
            return crow::response(crow::status::OK);
          });*/
}
