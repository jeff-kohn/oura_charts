
#include "AboutDialog.h"
#include "oura_charts/UserProfile.h"
#include <format>

namespace oura_charts
{

   AboutDialog::AboutDialog(const UserProfile& profile, wxWindow* parent)
   {
      auto profile_text = std::format("{}\r\n{}, age {}", profile.email(), profile.biologicalSex(), profile.age());
      
   }

   AboutDialog::AboutDialog(wxWindow* parent) 
   { 
      Create(parent); 
   }




}  // namespace oura_charts