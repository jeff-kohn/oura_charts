//---------------------------------------------------------------------------------------------------------------------
// test_oura_exception.cpp
//
// unit tests for oura_exception
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/oura_charts.h"
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <cpr/error.h>

namespace oura_charts::test
{
   using namespace std::literals;

   TEST_CASE("test_getCategoryName")
   {
      using namespace constants;
      using enum ErrorCategory;
      REQUIRE(getCategoryName(Generic) == ERROR_CATEGORY_GENERIC);
      REQUIRE(getCategoryName(Success) == ERROR_CATEGORY_SUCCESS);
      REQUIRE(getCategoryName(REST) == ERROR_CATEGORY_REST);
      REQUIRE(getCategoryName(Parse) == ERROR_CATEGORY_PARSE);
      REQUIRE(getCategoryName(FileIO) == ERROR_CATEGORY_FILE_IO);

   }

   TEST_CASE("test_format_as")
   {
      REQUIRE(fmt::format("{}", ErrorCategory::REST) == constants::ERROR_CATEGORY_REST);
   }

   TEST_CASE("test_oura_exception_ctor")
   {
      const std::string err_msg{ "error" };
      constexpr int64_t err_code{ -1 };
      auto cat = ErrorCategory::Generic;

      oura_exception ex{  err_code, err_msg, cat };

      REQUIRE(ex.error_code == err_code);
      REQUIRE(ex.message == err_msg);
      REQUIRE(ex.category == ErrorCategory::Generic);
      REQUIRE(ex.categoryName() == getCategoryName(cat));
      REQUIRE(ex.message == ex.what());

      oura_exception ex1{ err_msg, cat };
      oura_exception ex2{ std::move(ex1) };
      oura_exception ex3{ ex2 };
      REQUIRE(ex3.error_code == err_code);
      REQUIRE(ex3.message == err_msg);
      REQUIRE(ex3.category == ErrorCategory::Generic);
      REQUIRE(ex3.categoryName() == getCategoryName(cat));
      REQUIRE(ex3.message == ex.what());

      cpr::Error cpr_err{ int32_t{0}, std::string{err_msg} };
      oura_exception cpr_ex{ cpr_err };
      ex = cpr_ex;
      REQUIRE(ex.error_code == static_cast<int>(cpr_err.code));
      REQUIRE(ex.message == cpr_err.message);

      const std::string expected_msg{ fmt::format("Error message is: {}", err_msg) };
      ex = oura_exception{ cat, "Error message is: {}", err_msg };
      REQUIRE(ex.message == expected_msg);
   }


} // namespace oura_charts::test
