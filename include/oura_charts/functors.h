//---------------------------------------------------------------------------------------------------------------------
// functors.h
//
// some functors and algorithm adapters used in the project
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include <oura_charts/oura_charts.h>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <optional>
#include <variant>

namespace oura_charts
{

   /// <summary>
   ///   helper template for deducing return type of a class method.
   ///   example usage: MemberFunctionReturnType_t<decltype(&IObject::foo)>
   /// </summary>
   template<typename T>
   struct MemberFunctionReturnType;

   template<typename R, typename C, typename... Args>
   struct MemberFunctionReturnType<R(C::*)(Args...)>
   {
      using type = R;
   };

   template<typename MemberFunctionT>
   using MemberFunctionReturnType_t = typename MemberFunctionReturnType<MemberFunctionT>::type;


   /// <summary>
   ///   helper template for deducing class type of a class method.
   ///   example usage: MemberFunctionClassType_t<decltype(&IObject::foo)>
   /// </summary>
   template<typename T>
   struct MemberFunctionClassType;

   template<typename R, typename C, typename... Args>
   struct MemberFunctionClassType<R(C::*)(Args...)>
   {
      using type = C;
   };

   template<typename MemberFunctionT>
   using MemberFunctionClassType_t = typename MemberFunctionClassType<MemberFunctionT>::type;


   /// <summary>
   ///   template that combines a sequence of lambda's into a functor that overloads
   ///   function operator() once for each lambda.
   /// </summary>
   /// <remarks>
   ///   useful for creating creating visitors from lambdas when working with variants, etc.
   /// </remarks>
   template < typename... Ts >
   struct Overloaded : Ts...
   {
      using Ts:: operator()...;
   };




}  // namespace oura_charts
