#pragma once

#include "generated/PreferencesDialogBase.h"

namespace oura_charts
{
    
    class PreferencesDialog : public PreferencesDlgBase
    {
    public:
        // If you use this constructor, you must call Create(parent)
        PreferencesDialog();  

        // construct the dialog as a child window of the specified parent
        explicit PreferencesDialog(wxWindow* parent)  { Create(parent); }
    };


}  // namespace oura_charts