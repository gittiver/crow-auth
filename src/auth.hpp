//
// Created by Gulliver on 12.08.24.
//

#ifndef CROW_AUTH_HPP
#define CROW_AUTH_HPP

#include <crow/middleware.h>
#include <crow/http_response.h>

#include <string>

/** interface of Authentication implementations */
struct IAuthenticate {
  virtual ~IAuthenticate() = default;
  virtual bool is_user_authenticated(const std::string &username, const std::string &password) = 0;
  virtual bool is_bearer_authenticated(const std::string &bearer) = 0;
};

struct LoginRequiredMiddleware :  crow::ILocalMiddleware {
  struct context {
  };

  LoginRequiredMiddleware(): p_auth_delegate() {};
  explicit LoginRequiredMiddleware(std::unique_ptr<IAuthenticate> auth_delegate) {
    p_auth_delegate = std::move(auth_delegate);
  }

  virtual ~LoginRequiredMiddleware() = default;

  void before_handle(crow::request &req, crow::response &res, context &ctx) const;

  void after_handle(crow::request &req, crow::response &res, context &ctx) const;

  std::unique_ptr<IAuthenticate> p_auth_delegate;
};

#define CROW_LOGIN_REQUIRED(app) CROW_MIDDLEWARES(app, LoginRequiredMiddleware)

// End of Authentication utilities

#endif //CROW_AUTH_HPP
