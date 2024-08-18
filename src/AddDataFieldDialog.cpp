//---------------------------------------------------------------------------------------------------------------------
// AddDataFieldDialog.cpp
//
// source file for the AddDataFieldDialog class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "AddDataFieldDialog.h"
#include "adapters.h"
#include <wx/log.h>
#include <boost/lambda2.hpp>

namespace oura_charts
{
   using namespace schema;
   namespace vw = rg::views;



   AddDataFieldDialog::AddDataFieldDialog(wxWindow* parent, const SchemaManager& schema_mgr)
      : m_schema_mgr{ schema_mgr }
   {
      Create(parent);
   }


   AddDataFieldDialog::AddDataFieldDialog(const SchemaManager& schema_mgr) : m_schema_mgr{ schema_mgr }
   {
   }


   // cppcheck-suppress duplInheritedMember
   bool AddDataFieldDialog::Create(wxWindow* parent, const wxString& title)
   {
      if (!AddDataFieldDlgBase::Create(parent, wxID_ANY, title))
         return false;

      // populate combo box with list of data source classes.
      for (auto& schema : vw::values(m_schema_mgr.getSchemaRange()))
      {
         m_data_source_cbo->AppendString(schema.class_name);
      }

      return true;
   }


   void AddDataFieldDialog::onDataSourceSelected([[maybe_unused]] wxCommandEvent& event)
   {
      try
      {
         const auto& field_names = getSelectedSchemaClass().fields | vw::transform([] (const FieldSchema& fs)
                                                                          {
                                                                             return fs.long_display_name;
                                                                          });

         m_field_list->Clear();
         m_field_list->InsertItems(wxToArrayString(field_names), 0);

      }
      catch (std::exception& e)
      {
         wxLogError(e.what());
      }
   }


   void oura_charts::AddDataFieldDialog::onFieldSelected([[maybe_unused]] wxCommandEvent&  event)
   {
      try
      {


      }
      catch (std::exception& e)
      {
         wxLogError(e.what());
      }
   }


   void oura_charts::AddDataFieldDialog::onOkUpdateUI([[maybe_unused]] wxUpdateUIEvent& event)
   {
      assert(m_data_source_cbo and m_field_list and m_function_list);

      event.Enable(m_data_source_cbo->GetSelection() >= 0
                   and m_field_list->GetSelection() >= 0
                   and m_function_list->GetSelection() >= 0);
   }


   const schema::ClassSchema& AddDataFieldDialog::getSelectedSchemaClass() const
   {
      std::string data_class = m_data_source_cbo->GetStringSelection().ToStdString();
      return m_schema_mgr.getClassSchema(data_class);
   }


} // namespaced oura_charts
