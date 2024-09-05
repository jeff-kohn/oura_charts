//---------------------------------------------------------------------------------------------------------------------
// oura_charts.h
//
// Main header for the oura_charts library. 
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/constants.h"
#include "oura_charts/oura_exception.h"
#include "oura_charts/concepts.h"


// no using alias because clang doesn't fully support them (no template argument deduction,
// which makes the syntax more trouble than it's worth)
#if __cplusplus > 202002L && __cpp_concepts < 202002L // missing functionality in libc++
   #include <tl/expected.hpp>
   namespace oura_charts
   {
      using tl::expected;
      using tl::unexpected;
   }
#else
   #include <expected>
   namespace oura_charts
   {
      using std::expected;
      using std::unexpected;
   }
#endif
