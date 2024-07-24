
#include "MainFrame.h"
#include "AboutDialog.h"
#include "OuraChartsApp.h"
#include "PreferencesDialog.h"

#include "oura_charts/chrono_helpers.h"
#include "oura_charts/DailySleepScore.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/UserProfile.h"

#include <wxcharts/wx/charts/wxcharts.h> // fix this dumb include path

#include <algorithm>
#include <chrono>
#include <map>
#include <ranges>

namespace oura_charts
{


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

         auto pat{ wxGetApp().getRestToken().transform_error([] (auto&& e) -> std::decay_t<decltype(e)> { throw e; }).value() };
      
         // Get sleep scores for the past year
         auto today = stripTimeOfDay(localNow());
         auto last_week = today - months{ 12 };
         RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
         auto score_data = getDataSeries<DailySleepScore>(rest_server, getCalendarDate(last_week), getCalendarDate(today));

         // group by day of week. in case of sleep we filter for only "long" sleep (no naps)
         auto score_by_weekday = group<SleepScoreByWeekday>(std::move(score_data), sleepScoreWeekday);

         // calculate average score per weekday.
         constexpr auto weekdays = getWeekdays();
         vector<nullable_double> avg_score(weekdays.size() );

         for (auto wd : weekdays)
         {
            // get the sub range representing all the data for the current weekday. could be an empty range,
            // but that won't cause any problems we'll just get null result.
            auto [score_beg, score_end] = score_by_weekday.equal_range(wd);
            std::ranges::subrange score_range{ score_beg, score_end };

            AvgCalc<uint32_t> calc_score{};
            rg::for_each(score_range | vw::values, [&] (const DailySleepScore& score)
                         {
                           calc_score(score.score());
                         });

            avg_score[wd.c_encoding()] = calc_score.result();
         }

         // transform the nullable scores into double's, using 0 for null (for now).
         auto sleep_scores = vw::all(avg_score) | vw::transform([] (const nullable_double& score) -> double
                                                                {
                                                                   return score.value_or(0);
                                                                })
                                                | rg::to<vector>();

         // get the weekday names for our chart label.
         auto weekday_labels = getWeekdayNames() | vw::transform([] (const std::string& str) { return wxString{ str }; })
                                                 | rg::to<std::vector>();

         // create the chart
         wxChartsCategoricalData::ptr chartData = wxChartsCategoricalData::make_shared(weekday_labels);
         chartData->AddDataset(wxChartsDoubleDataset::ptr{ new wxChartsDoubleDataset("Avg. Sleep Score", sleep_scores) });

         // Create the chart control and add it to our sizer in our main panel.
         m_chart_sizer->Add(new wxColumnChartCtrl(m_chart_panel, wxID_ANY, chartData), wxSizerFlags(1).Expand().Border(wxALL));
         m_chart_sizer->Layout();
      }
      catch (std::exception& e)
      {
         wxLogError(e.what());
      }
   }


   void MainFrame::OnMenuFileQuit(wxCommandEvent&)
   {
      Close(true);
   }


   void MainFrame::OnMenuHelpAboutWx(wxCommandEvent&)
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
