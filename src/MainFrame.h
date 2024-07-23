
#pragma once

#include "generated/MainFrameBase.h"
#include "constants.h"

namespace oura_charts
{
    class MainFrame : public MainFrameBase
    {
    public:
        // If you use this constructor, you must call Create(parent)
        MainFrame() = default;
        explicit MainFrame(wxWindow* parent) : MainFrameBase(parent, wxID_ANY, constants::APP_NAME) {};

    protected:
        void onMenuHelpAbout(wxCommandEvent& event) override;
        void onMenuFilePreferences(wxCommandEvent& event) override;
    };


}  // namespace oura_charts
