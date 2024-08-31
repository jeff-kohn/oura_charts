#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace oura_charts::detail
{
   template <typename T>
   using Nullable = std::optional<T>;

   using NullableInt       = Nullable<int32_t>;
   using NullableInt64     = Nullable<int64_t>;
   using NullableUInt      = Nullable<uint32_t>;
   using NullableUInt64    = Nullable<uint64_t>;
   using NullableFloat     = Nullable<float>;
   using NullableDouble    = Nullable<double>;
   using NullableString    = Nullable<std::string>;

} // namespace oura_charts::detail
