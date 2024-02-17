//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// RestAuth.h
///
/// Defines the RestAuth interface and implementation(s) used for access the Oura REST API.
///
/// Copyright (c) 2024 Jeff Kohn
///
//////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "oura_charts/oura_charts.h"
#include <string>
#include <cpr/bearer.h>

namespace oura_charts
{

   /// <summary>
   /// The wrapper object we use for authenticating with the Oura REST API.
   /// </summary>
   /// <todo>
   /// put some interface constraints on this class
   /// </todo>
   template <class Auth>
   class AuthWrapper
   {
   public:
      AuthWrapper(Auth auth) : m_auth(std::move(auth)) {} // intentionally not explicit

      auto getAuthorization() const
      {
         return m_auth.getAuthorization();
      }
   private:
      Auth m_auth;
   };


   /// <summary>
   /// Encapsulates a Personal Access Token (PAT) for connecting to the Oura REST API.
   /// </summary>
   /// <remarks>
   /// Oura REST API supports two authentication methods: Personal Access Token (PAT)
   /// and OAUTH. The PAT allows pulling data from a single Oura account, and is suitable
   /// for client apps. User must create a PAT on the OURA website to use with this
   /// authentication type.
   /// </remarks>
   class TokenAuth
   {
   public:
      explicit TokenAuth(std::string_view token) : m_token(token) {}
      explicit TokenAuth(std::string &&token) : m_token(std::move(token)) {}
      ~TokenAuth() = default;
      TokenAuth(const TokenAuth &other) = default;
      TokenAuth(TokenAuth &&other) = default;
      TokenAuth &operator=(TokenAuth &&other) = default;
      TokenAuth &operator=(const TokenAuth &other) = default;

      /// <summary>
      /// The PAT that the user generated on the Oura website.
      /// </summary>
      std::string getToken() const          { return m_token;                    }
      void setToken(std::string_view token) { m_token = std::string{ token };    }
      void setToken(std::string&& token)    { m_token = std::move(token);        }

      /// <summary>
      /// This is called by AuthWrapper to get the Bearer token used for authenticating
      /// with OURA REST
      /// </summary>
      cpr::Bearer getAuthorization() const;

   private:
      std::string m_token;
   };

} // namespace oura_charts
