//---------------------------------------------------------------------------------------------------------------------
// AboutDialog.cpp
//
// header file for the AboutDialog class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "AboutDialog.h"

#include "oura_charts/UserProfile.h"

#include <format>

namespace oura_charts
{

   AboutDialog::AboutDialog(const UserProfile& profile, wxWindow* parent)
   {
      auto profile_text = std::format("{}\r\n{}, age {}", profile.email(), profile.biologicalSex(), profile.age());
      Init(profile_text, parent);
   }

   oura_charts::AboutDialog::AboutDialog(const std::string& profile_text, wxWindow* parent)
   {
      Init(profile_text, parent);
   }

   void AboutDialog::Init(const std::string& profile_text, wxWindow* parent)
   {
      Create(parent);
      setProfileText(profile_text);
      m_main_sizer->Layout();
      m_main_sizer->Fit(this);
      m_main_sizer->SetSizeHints(this);
   }

}  // namespace oura_charts

