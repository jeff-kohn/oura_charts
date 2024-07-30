
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

//#include <wxcharts/wx/charts/wxcharts.h>

//#include <algorithm>
//#include <chrono>
//#include <map>
//#include <ranges>

namespace oura_charts
{
   // Disable this warning, non-virtual Create() is a pretty standard idiom since it's
   // called from ctor in most cases.
   // cppcheck-suppress duplInheritedMember
   bool MainFrame::Create(std::weak_ptr<wxDocManager> doc_mgr, wxFrame* parent,
                          wxWindowID id, const wxString& title)
   {
      auto doc = doc_mgr.lock();
      assert(doc);
      if (not wxDocParentFrame::Create(doc.get(), parent, id, title))
         return false;

      initControls();

      //// get weekday names and use to create dataset for our chart label.
      //auto weekday_names = getWeekdayNames() | vw::transform([] (const std::string& str) { return wxString{ str }; })
      //                                       | rg::to<std::vector>();

      //assert(!m_data);
      //m_data = wxChartsCategoricalData::make_shared(weekday_names);

      //// Create the column chart widget
      //auto* columnChartCtrl = new wxColumnChartCtrl(this, wxID_ANY, m_data);  // NOLINT(cppcoreguidelines-owning-memory)
      //m_chart_sizer->Add(columnChartCtrl, wxSizerFlags(1).Expand().Border(wxALL));
      //m_chart_sizer->Layout();
      //m_chart_sizer->SetSizeHints(this);

      return true;
   }

   void MainFrame::initControls()
   {
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
      auto* menu_item2 = new wxMenuItem(m_file_menu, wxID_PREFERENCES);
      m_file_menu->Append(menu_item2);
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

      Centre(wxBOTH);

      // Event handlers
      Bind(wxEVT_MENU, &MainFrame::onMenuFilePreferences, this, wxID_PREFERENCES);
      Bind(wxEVT_MENU, &MainFrame::onMenuFileQuit, this, wxID_EXIT);
      Bind(wxEVT_MENU, &MainFrame::onMenuHelpAbout, this, wxID_ABOUT);
      Bind(wxEVT_MENU, &MainFrame::onMenuHelpAboutWx, this, menu_item4->GetId());
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
         using std::vector;
         using std::optional;

         //auto pat{ wxGetApp().getRestToken().transform_error([] (auto&& e) -> std::decay_t<decltype(e)> { throw e; }).value() };
      
         //// Get sleep scores for the past year
         //auto today = stripTimeOfDay(localNow());
         //auto last_year = today - months{ 12 };
         //RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
         //auto score_data = getDataSeries<DailySleepScore>(rest_server, getCalendarDate(last_year), getCalendarDate(today));

         //// group by day of week. in case of sleep we filter for only "long" sleep (no naps)
         //auto score_by_weekday = group<SleepScoreByWeekday>(std::move(score_data), sleepScoreWeekday);

         //// calculate average score per weekday.
         //constexpr auto weekdays = getWeekdays();
         //vector<nullable_double> avg_score(weekdays.size() );

         //for (auto wd : weekdays)
         //{
         //   // get the sub range representing all the data for the current weekday. could be an empty range,
         //   // but that won't cause any problems we'll just get null result.
         //   auto [score_beg, score_end] = score_by_weekday.equal_range(wd);
         //   std::ranges::subrange score_range{ score_beg, score_end };

         //   AvgCalc<uint32_t> calc_score{};
         //   rg::for_each(score_range | vw::values, [&] (const DailySleepScore& score)
         //                {
         //                  calc_score(score.score());
         //                });

         //   avg_score[wd.c_encoding()] = calc_score.result();
         //}

         //// transform the nullable scores into double's, using 0 for null (for now).
         //auto sleep_scores = vw::all(avg_score) | vw::transform([] (const nullable_double& score) -> double
         //                                                       {
         //                                                          return score.value_or(0);
         //                                                       })
         //                                       | rg::to<vector>();

         //// get the weekday names for our chart label.
         //auto weekday_labels = getWeekdayNames() | vw::transform([] (const std::string& str) { return wxString{ str }; })
         //                                        | rg::to<std::vector>();

         //// Add the data to the chart
         //m_data->AddDataset(wxChartsDoubleDataset::ptr{ new wxChartsDoubleDataset("Avg. Sleep Score", sleep_scores) });;
         
         CallAfter([this] { this->Refresh(); this->Update(); });

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
}  // namespace oura_charts
