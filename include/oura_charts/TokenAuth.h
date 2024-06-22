//---------------------------------------------------------------------------------------------------------------------
// TokenAuth.h
//
// Defines the class for using Bearer authentication token with Oura REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include <cpr/bearer.h>
#include <string>
#include <string_view>

namespace oura_charts
{
   /// <summary>
   ///   Encapsulates a Personal Access Token (PAT) for connecting to the Oura REST API.
   /// </summary>
   /// <remarks>
   ///   Oura REST API supports two authentication methods: Personal Access Token (PAT)
   ///   and OAuth. The PAT allows pulling data from a single Oura account, and is suitable
   ///   for client apps. Users must create a PAT on the OURA website to use with this
   ///   authentication type.
   /// </remarks>
   class TokenAuth
   {
   public:
      /// <summary>
      ///   The PAT that the user generated on the Oura website.
      /// </summary>
      const std::string& getToken() const          { return m_token;                    }
      void setToken(std::string_view token) { m_token = std::string{ token };    }
      void setToken(std::string&& token)    { m_token = std::move(token);        }

      /// <summary>
      ///   This is called to get the Bearer token used for
      ///   authenticating with Oura's REST endpoint
      /// </summary>
      cpr::Bearer getAuthorization() const { return cpr::Bearer{ m_token }; }

      explicit TokenAuth(std::string_view token) : m_token(token) {}
      explicit TokenAuth(std::string&& token) : m_token(std::move(token)) {}
      ~TokenAuth() = default;
      TokenAuth(const TokenAuth& other) = default;
      TokenAuth(TokenAuth&& other) = default;
      TokenAuth& operator=(TokenAuth&& other) = default;
      TokenAuth& operator=(const TokenAuth& other) = default;

   private:
      std::string m_token;
   };

} // namespace oura_charts
