//---------------------------------------------------------------------------------------------------------------------
// AboutDialog.h
//
// header file for the AboutDialog class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "generated/AboutDialogBase.h"

#include "oura_charts/UserProfile.h"


namespace oura_charts
{
   class AboutDialog : public AboutDlgBase
   {
   public:
      AboutDialog(const UserProfile& profile, wxWindow* parent);
      AboutDialog(const std::string& profile_text, wxWindow* parent);

   private:
      void Init(const std::string& profile_text, wxWindow* parent);
   };

}  // namespace oura_charts
