//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// rest_constants.h
///
/// miscellaneous utility functions/classes/wrappers used throughout the project.
///
/// Copyright (c) 2024 Jeff Kohn
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace our_charts::detail
{
   /// <summary>
   /// 'overloaded' utility class. Allows you to inherit from multiple classes (or lambdas),
   /// inheriting their operator() so that it's overloaded for multiple signatures in one class.
   /// </summary>
   template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
   template<class... Ts> overload(Ts...) -> overload<Ts...>;

}
