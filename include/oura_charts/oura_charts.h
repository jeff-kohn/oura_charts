#pragma once

#include "oura_charts/constants.h"
#include "oura_charts/oura_exception.h"


#if __cplusplus > 202002L && __cpp_concepts < 202002L
   #include <tl/expected.hpp>
   namespace oura_charts{
      using tl::expected;
      using tl::unexpected;
   }
#else
   #include <expected>
   namespace ourca_charts{
      using std::expected;
      using std::unexpected;
   }
#endif
