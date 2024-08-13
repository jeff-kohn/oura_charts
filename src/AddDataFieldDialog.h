//---------------------------------------------------------------------------------------------------------------------
// AddDataFieldDialog.h
//
// header file for the AddDataFieldDialog class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "constants.h"
#include "generated/AddDataFieldBase.h"

#include "oura_charts/SchemaManager.h"


namespace oura_charts
{

   class AddDataFieldDialog : public oura_charts::AddDataFieldDlgBase
   {
   public:
      explicit AddDataFieldDialog(const schema::SchemaManager& schema_mgr);
      AddDataFieldDialog(wxWindow* parent, const schema::SchemaManager& schema_mgr);

      /// <summary>
      ///   Create a wxWidgets window object and associate it with this dialog class.
      ///   This gets called automatically from the constructor that takes a parent
      ///   window parameter, but can be called explicitly for two-phase window creation
      ///   or if you want to use any non-default values.
      /// </summary>
      // cppcheck-suppress duplInheritedMember
      bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = constants::DLG_TITLE_ADD_DATA_FIELD,
                  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                  long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxDialogNameStr);

   private:
      const schema::SchemaManager& m_schema_mgr;

      void onDataSourceSelected(wxCommandEvent& event) override;
      void onFieldSelected(wxCommandEvent& event) override;
      void onOkUpdateUI(wxUpdateUIEvent& event) override;

   };


}
