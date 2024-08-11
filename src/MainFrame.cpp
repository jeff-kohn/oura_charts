
#include "MainFrame.h"
#include "AboutDialog.h"
#include "OuraChartsApp.h"
#include "PreferencesDialog.h"

#include "oura_charts/chrono_helpers.h"
#include "oura_charts/DailySleepScore.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/UserProfile.h"

#include <wx/artprov.h>
#include <wx/stockitem.h>
#include <wx/menu.h>

#include <matplot/matplot.h>


namespace oura_charts
{
   // Disable this warning, non-virtual Create() is a pretty standard idiom since it's
   // called from ctor in most cases.
   // cppcheck-suppress duplInheritedMember
   bool MainFrame::Create(const std::weak_ptr<wxDocManager>& doc_mgr, wxFrame* parent,
                          const wxPoint& pos, const wxSize& size,
                          long style, wxWindowID id, const wxString& title)
   {
      auto doc = doc_mgr.lock();
      assert(doc);
      if (not wxDocParentFrame::Create(doc.get(), parent, id, title, pos, size, style))
         return false;

      initControls();

      return true;
   }


   void MainFrame::onMenuFilePreferences(wxCommandEvent&)
   {
      try
      {
         PreferencesDialog dlg(this);
         dlg.ShowModal();
      }
      catch (oura_exception& e)
      {
         wxLogError(e.message.c_str());
      }
      catch (std::exception& e)
      {
         wxLogError(e.what());
      }
   }


   void MainFrame::onMenuFileTestChart(wxCommandEvent&)
   {

      try
      {
         using namespace oura_charts;
         using namespace oura_charts::chrono;
         using namespace std::chrono_literals;
         using std::vector;
         using std::string;

         auto today = stripTimeOfDay(localNow());
         auto last_year = today - months{ 12 };
         auto token_res = wxGetApp().getRestToken();
         if (!token_res)
            throw oura_exception{ std::move(token_res.error()) };

         RestDataProvider rest_server{ token_res.value(), constants::REST_DEFAULT_BASE_URL };

         auto score_data = getDataSeries<DailySleepScore>(rest_server, getCalendarDate(last_year), getCalendarDate(today));
         auto score_by_weekday = group<SleepScoreByWeekday>(std::move(score_data), sleepScoreWeekday);

         // calculate various averages by day of week.
         constexpr auto weekdays = getWeekdays();
         vector<nullable_double> avg_score(weekdays.size());

         for (auto wd : weekdays)
         {
            // get the sub range representing all the data for the current weekday. could be an empty range,
            // but that won't cause any problems we'll just get null result.
            AvgCalc<uint32_t> calc_score{};
            auto [score_beg, score_end] = score_by_weekday.equal_range(wd);
            rg::subrange score_range{ score_beg, score_end };
            rg::for_each(score_range | vw::values, [&] (const DailySleepScore& score)
                                        {
                                              calc_score(score.score());
                                        });
            avg_score[wd.c_encoding()] = calc_score.result();
         }

         matplot::bar(avg_score | vw::transform([] (auto&& val) -> auto
                                                {
                                                   return val.value_or(0.0);
                                                })
                                | rg::to<vector>());

         matplot::gca()->x_axis().ticklabels(getWeekdayNames());
         matplot::show();


      }
      catch (std::exception& e)
      {
         wxLogError(e.what());
      }
   }


   void MainFrame::onMenuFileQuit(wxCommandEvent&)
   {
      Close(true);
   }


   void MainFrame::onMenuHelpAboutWx(wxCommandEvent&)
   {
      wxInfoMessageBox(this);
   }


   void MainFrame::onMenuHelpAbout(wxCommandEvent&)
   {
      try
      {
         auto auth_res = wxGetApp().getRestToken();
         if (auth_res)
         {
            RestDataProvider provider{ *auth_res, constants::REST_DEFAULT_BASE_URL };
            auto profile = getUserProfile(provider);
            AboutDialog dlg(profile, this);
            dlg.ShowModal();
         }
         else
         {
            AboutDialog dlg(auth_res.error().message.c_str(), this);
            dlg.ShowModal();
         }
      }
      catch (oura_exception& e)
      {
         wxLogError(e.message.c_str());
      }
      catch (std::exception& e)
      {
         wxLogError(e.what());
      }
   }


   void MainFrame::initControls()
   {
      // NOLINTBEGIN(*)  generated code copy pasted from derived class, so don't lint
      // (we can get rid of this when code generation bug in wxUiDesigner is fixed).

      m_toolbar = CreateToolBar();
      m_toolbar->Realize();

      auto* menubar = new wxMenuBar();

      auto* m_file_menu = new wxMenu();
      auto* menu_item6 = new wxMenuItem(m_file_menu, wxID_NEW);
      menu_item6->SetBitmap(wxArtProvider::GetBitmapBundle(wxART_NEW, wxART_MENU));

      m_file_menu->Append(menu_item6);
      auto* menu_item7 = new wxMenuItem(m_file_menu, wxID_OPEN);
      menu_item7->SetBitmap(wxArtProvider::GetBitmapBundle(wxART_FILE_OPEN, wxART_MENU));

      m_file_menu->Append(menu_item7);
      m_file_menu->AppendSeparator();
      auto* menu_item5 = new wxMenuItem(m_file_menu, wxID_SAVE);
      menu_item5->SetBitmap(wxArtProvider::GetBitmapBundle(wxART_FILE_SAVE, wxART_MENU));

      m_file_menu->Append(menu_item5);
      auto* menu_item8 = new wxMenuItem(m_file_menu, wxID_SAVEAS);
      menu_item8->SetBitmap(wxArtProvider::GetBitmapBundle(wxART_FILE_SAVE_AS, wxART_MENU));

      m_file_menu->Append(menu_item8);
      m_file_menu->AppendSeparator();
      auto* menu_item2 = new wxMenuItem(m_file_menu, wxID_ANY, "&Test Chart");
      m_file_menu->Append(menu_item2);
      auto* menu_item9 = new wxMenuItem(m_file_menu, wxID_PREFERENCES);
      m_file_menu->Append(menu_item9);
      m_file_menu->AppendSeparator();
      auto* menu_item = new wxMenuItem(m_file_menu, wxID_EXIT);
      menu_item->SetBitmap(wxArtProvider::GetBitmapBundle(wxART_QUIT, wxART_MENU));

      m_file_menu->Append(menu_item);
      menubar->Append(m_file_menu, wxGetStockLabel(wxID_FILE));

      auto* m_menu = new wxMenu();
      auto* menu_item4 = new wxMenuItem(m_menu, wxID_ANY, "wx&Widgets Info");
      m_menu->Append(menu_item4);
      auto* menu_item3 = new wxMenuItem(m_menu, wxID_ABOUT);
      m_menu->Append(menu_item3);
      menubar->Append(m_menu, "&Help");

      SetMenuBar(menubar);

      m_statusBar = CreateStatusBar();

      m_sizer_horiz = new wxBoxSizer(wxHORIZONTAL);

      m_sizer_vert = new wxBoxSizer(wxVERTICAL);

      m_sizer_horiz->Add(m_sizer_vert, wxSizerFlags(1).Expand().Border(wxALL));
      SetSizerAndFit(m_sizer_horiz);

      Centre(wxBOTH);
      // NOLINTEND(*)

      // Event handlers
      Bind(wxEVT_MENU, &MainFrame::onMenuFilePreferences, this, wxID_PREFERENCES);
      Bind(wxEVT_MENU, &MainFrame::onMenuFileQuit, this, wxID_EXIT);
      Bind(wxEVT_MENU, &MainFrame::onMenuFileTestChart, this, menu_item2->GetId());
      Bind(wxEVT_MENU, &MainFrame::onMenuHelpAbout, this, wxID_ABOUT);
      Bind(wxEVT_MENU, &MainFrame::onMenuHelpAboutWx, this, menu_item4->GetId());
   }


}  // namespace oura_charts
