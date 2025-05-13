#include <optional>

#include <crow/http_response.h>
#include <crow/json.h>

#include "auth.hpp"
#include "bp_user_registration.hpp"
#include "auth_user.hpp"

static crow::json::wvalue error2json(const std::string &errstring) {
  crow::json::wvalue r;
  r["error"] = errstring;
  return r;
}

int main() {
  crow::App<LoginRequiredMiddleware> app;

  std::unique_ptr<AuthDbAuth> auth = std::make_unique<AuthDbAuth>();
  auth->connection("");//"file://my_file.txt");
  auto userDB = auth->user_db();
  userDB->add_user(User{ "u1","u1_pw" });
  userDB->add_user(User{"u2","u2_pw"});
  userDB->add_user(User{"u3","u3_pw"});

  app.get_middleware<LoginRequiredMiddleware>().p_auth_delegate = std::move(auth);

  CROW_ROUTE(app, "/api/do_authenticated")
      .methods(crow::HTTPMethod::Post, crow::HTTPMethod::Get)
      .CROW_LOGIN_REQUIRED(app)
          ([]() {

            CROW_LOG_INFO << "do authenticated after successful authentication";
            return crow::response(crow::status::OK);
          });

  bp_user_registration user_registration(app,*userDB);
  app.register_blueprint(user_registration);
  app.port(18080).run();

}
