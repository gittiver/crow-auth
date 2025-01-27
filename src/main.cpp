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

private:
  void on_init() override {};
  bool is_user_authenticated(const std::string &username,
                             const std::string &password) override
  { return true; }

  bool is_bearer_authenticated(const std::string &bearer) override
  { return true; };
};

class AdminAuth : public IAuthenticate {

private:
  void on_init() override {};
  bool is_user_authenticated(const std::string &username,
                             const std::string &password) override
  { return username=="admin" && password=="admin"; }

  bool is_bearer_authenticated(const std::string &bearer) override
  { return false; };
};

int main() {
  crow::App<LoginRequiredMiddleware,AdminRequiredMiddleware> app;
  app.get_middleware<LoginRequiredMiddleware>().p_auth_delegate = std::make_unique<SimpleAuth>();
  app.get_middleware<AdminRequiredMiddleware>().p_auth_delegate = std::make_unique<AdminAuth>();

  CROW_ROUTE(app, "/api/do_authenticated")
      .methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get)
      .CROW_LOGIN_REQUIRED(app)
          ([]() {

            CROW_LOG_INFO << "do authenticated after successful authentication";
            return crow::response(crow::status::OK);
          });

  CROW_ROUTE(app, "/api/admin_only")
        .methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get)
        .CROW_ADMIN_REQUIRED(app)
            ([]() {

              CROW_LOG_INFO << "admin authenticated";
              return crow::response(crow::status::OK);
            });


  app.port(18080).run();

}
