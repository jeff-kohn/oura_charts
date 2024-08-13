//---------------------------------------------------------------------------------------------------------------------
// AddDataFieldDialog.cpp
//
// source file for the AddDataFieldDialog class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "AddDataFieldDialog.h"


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
   bool AddDataFieldDialog::Create(wxWindow* parent, wxWindowID id, const wxString& title,
                                   const wxPoint& pos, const wxSize& size, long style, const wxString& name)
   {
      if (!AddDataFieldDlgBase::Create(parent, id, title, pos, size, style, name))
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
   }


   void oura_charts::AddDataFieldDialog::onFieldSelected([[maybe_unused]] wxCommandEvent&  event)
   {
   }


   void oura_charts::AddDataFieldDialog::onOkUpdateUI([[maybe_unused]] wxUpdateUIEvent& event)
   {
   }


} // namespaced oura_charts
