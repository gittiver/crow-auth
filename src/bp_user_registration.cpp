//
// Created by Gulliver on 13.03.24.
//

#include "bp_user_registration.hpp"

bp_user_registration::bp_user_registration()
    : crow::Blueprint("user") {

CROW_BP_ROUTE((*this), "/api/register")
.methods(crow::HTTPMethod::Post)
      ([]() {
        // ...
        return crow::response(crow::status::OK);
      });

CROW_BP_ROUTE((*this), "/api/profile/<int>")
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
([](int /*id*/) {
            //...
            return crow::response(crow::status::OK);
          });
}
