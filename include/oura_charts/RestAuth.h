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

#include <restc-cpp/RequestBuilder.h>
#include <string>


namespace oura_charts
{

   /// <summary>
   /// The interface we use for authenticating with the Oura REST API. 
   /// </summary>
   class RestAuth
   {
   public:
      RestAuth() = default;
      virtual ~RestAuth() noexcept = default;

      virtual void setHeaders(restc_cpp::RequestBuilder &req) = 0;
   };


   /// <summary>
   /// Encapsulates a Personal Access Token (PAT) for connecing to the Oura REST API.
   /// </summary>
   /// 
   /// <remarks>
   /// Oura REST API supports two authentication methods: Personal Access Token (PAT)
   /// and OAUTH. The PAT allows pulling data from a single Oura account, and is suitable
   /// for client apps. User must create a PAT on the OURA website to use with this
   /// authentication type.
   /// </remarks>
   class TokenAuth final : public RestAuth
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
      std::string token() const { return m_token; }

      /// <summary>
      /// Adds the auth header(s) to the HTTP Request
      /// </summary>
      void setHeaders(restc_cpp::RequestBuilder &req) override;

   private:
      std::string m_token;
   };

} // namespace oura_charts
