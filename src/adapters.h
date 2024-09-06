#pragma once

#include "oura_charts/concepts.h"
#include "oura_charts/detail/functors.h"

#include <wx/arrstr.h>

#include <string>
#include <ranges>


namespace oura_charts
{
   template <rg::input_range Rg> requires StringViewCompatible<rg::range_value_t<Rg> >
   wxArrayString wxToArrayString(Rg&& strings)
   {
      using namespace oura_charts::detail;
      Overloaded overloaded{ [](std::string&& str)
                         {
                            return wxString{std::move(str)};
                         },
                         [] (auto&& str)
                         {
                            return wxString{std::forward<decltype(str)>(str)};
                         }};

      return std::forward<decltype(strings)>(strings) | vw::transform(overloaded)
                                                      | rg::to<wxArrayString>();
   }


} // namespace oura_charts
