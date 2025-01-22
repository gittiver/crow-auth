#include <optional>

#include <crow/http_response.h>
#include <crow/json.h>

#include "auth.hpp"
#include "bp_user_registration.hpp"

static crow::json::wvalue error2json(const std::string &errstring) {
  crow::json::wvalue r;
  r["error"] = errstring;
  return r;
}

class SimpleAuth : public IAuthenticate {
  bool is_user_authenticated(const std::string &username,
			     const std::string &password) override
  { return true; }

  bool is_bearer_authenticated(const std::string &bearer) override
  { return true; };
};

int main() {
  crow::App<LoginRequiredMiddleware> app;
  app.get_middleware<LoginRequiredMiddleware>().p_auth_delegate = std::make_unique<SimpleAuth>();

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
