#include <optional>

#include <crow/http_response.h>
#include <crow/json.h>

#include "auth.hpp"
#include "bp_user_registration.hpp"


crow::json::wvalue error2json(const std::string &errstring) {
  crow::json::wvalue r;
  r["error"] = errstring;
  return r;
}

int main() {
  crow::App<LoginRequiredMiddleware> app;

  CROW_ROUTE(app, "/api/do_authenticated")
      .methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get)
      .CROW_LOGIN_REQUIRED(app)
          ([]() {

            CROW_LOG_INFO << "do authenticated after successful authentication";
            return crow::response(crow::status::OK);
          });

  bp_user_registration user_registration;
  app.register_blueprint(user_registration);
  app.port(18080).run();

}
