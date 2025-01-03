//
// Created by Gulliver on 12.08.24.
//
#include "auth.hpp"

bool validate_authentication(const crow::request &request,
                             crow::response &response,
                             IAuthenticate* p_auth_delegate) {
  std::string myauth = request.get_header_value("Authorization");
  if (myauth.empty()) {
    response.code = crow::status::UNAUTHORIZED;
    response.set_header("WWW-Authenticate",
                        "Basic realm=\"User Visible Realm\", charset=\"UTF-8\"");
    return false;
  }
  if (myauth.find("Basic ") == 0) {
    // Cut off starting "Basic "
    std::string mycreds = myauth.substr(6);
    // decode base64
    std::string d_mycreds = crow::utility::base64decode(mycreds, mycreds.size());
    // Now that we have our username:password string,
    // we only need to separate it into 2 different strings
    // and verify their validity:
    size_t found = d_mycreds.find(':');
    if (found == std::string::npos) {
      response.code = crow::status::UNAUTHORIZED;
      response.set_header("WWW-Authenticate",
                          "Basic realm=\"User Visible Realm\", charset=\"UTF-8\"");
      return false;
    } else {
      std::string username = d_mycreds.substr(0, found);
      std::string password = d_mycreds.substr(found + 1);
      if (p_auth_delegate !=nullptr && p_auth_delegate->is_user_authenticated(username, password)) {
        return true;
      } else {
        response.code = crow::status::UNAUTHORIZED;
        response.set_header("WWW-Authenticate",
                            "Basic realm=\"User Visible Realm\", charset=\"UTF-8\"");
        return false;
      }
    }
  } else if (myauth.find("Bearer ") == 0) {
    // Cut off starting "Bearer "
    std::string bearer = myauth.substr(7);
    if (p_auth_delegate != nullptr && p_auth_delegate->is_bearer_authenticated(bearer)) {
      return true;
    } else {
      response.code = crow::status::UNAUTHORIZED;
      response.set_header("WWW-Authenticate",
                          "Bearer realm=\"User Visible Realm\", charset=\"UTF-8\"");
      return false;
    }
  } else {
    response.code = crow::status::UNAUTHORIZED;
    response.set_header("WWW-Authenticate",
                        "Basic realm=\"User Visible Realm\", charset=\"UTF-8\"");
    return false;
  }
}


void LoginRequiredMiddlewareBase::before_handle(crow::request &req,
                                            crow::response &res,
                                            LoginRequiredMiddlewareBase::context &ctx) const {
  CROW_LOG_DEBUG << "check auth for " << req.url;
  if (!validate_authentication(req, res, p_auth_delegate.get())) {
    res.end();
  }
}


void LoginRequiredMiddlewareBase::after_handle(crow::request &/*req*/,
                                           crow::response &/*res*/,
                                           context &/*ctx*/) const {
}
