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

#if __cplusplus > 202002L && __cpp_concepts < 202002L // missing functionality in libc++
   #include <tl/expected.hpp>
   namespace oura_charts
   {
      using expected = tl::expected;
      using unexpected = tl::unexpected;
   }
#else
   #include <expected>
   namespace oura_charts
   {
      using std::expected;
      using std::unexpected;
   }
#endif
