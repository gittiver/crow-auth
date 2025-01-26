//
// Created by Gulliver on 13.03.24.
//

#include "bp_user_registration.hpp"
#include "auth.hpp"

bp_user_registration::bp_user_registration(const crow::App<LoginRequiredMiddleware>& app)
    : crow::Blueprint("user") {

CROW_BP_ROUTE((*this), "/api/register")
.methods(crow::HTTPMethod::Post)
      ([]() {
        // ...
        return crow::response(crow::status::OK);
      });

CROW_BP_ROUTE((*this), "/api/logout")
  .CROW_MIDDLEWARES(app,LoginRequiredMiddleware)
  ([]() {
        // ...
        return crow::response(crow::status::OK);
      });

CROW_BP_ROUTE((*this), "/api/profile/<int>")
  .methods(crow::HTTPMethod::Get)
  .CROW_MIDDLEWARES(app,LoginRequiredMiddleware)

      ([](int /*id*/) {
        //...
        return crow::response(crow::status::OK);
      });

CROW_BP_ROUTE((*this), "/api/profile/<int>")
  .methods(crow::HTTPMethod::Post)
  .CROW_MIDDLEWARES(app,LoginRequiredMiddleware)

      ([](int /*id*/) {
        //...
        return crow::response(crow::status::OK);
      });


 
CROW_BP_ROUTE((*this), "/register")
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
  ([](int /*id*/) {
            //...
            return crow::response(crow::status::OK);
          });
}
