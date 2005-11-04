#ifndef STUDIO_OVERVIEW_H
#define STUDIO_OVERVIEW_H

#include <set>
#include <map>

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/treectrl.h>

#include "project_manager.h"
#include "tool_manager.h"

class StudioOverview : public wxFrame {
  DECLARE_CLASS(StudioOverview)
  DECLARE_EVENT_TABLE()

  public:
    StudioOverview();
    ~StudioOverview();
    StudioOverview(ToolManager&, wxWindow* parent, wxWindowID id);

    void SetToolManager(ToolManager&);

  private:
    wxTreeCtrl*  specifications;
    wxPanel*     progress;
    wxMenu*      tree_popup_menu;

    /* The tool categories in the system, sorted lexicographically */
    std::set < std::string > tool_categories;

    /* Pre-created context menus per format and per category for all specification formats */
    std::map < std::pair < std::string, std::string >, wxMenu* > context_menus;

    /* The tool manager */
    ToolManager&   tool_manager;

    /* The project manager */
    ProjectManager project_manager;

    /* Convenience functions */
    inline void GenerateMenuBar();
    inline void GenerateContextMenus();
    inline void GenerateToolBar();

    /* Handler for tool selection */
    void ToolSelected(wxCommandEvent &event);

    /* Adds a new specification tot the project manager and specification tree view */
    inline wxTreeItemId CreateSpecification(wxTreeItemId parent, Specification& specification);

    /* Handlers for operations of project level */
    void NewProject(wxCommandEvent &event);
    void CloseProject(wxCommandEvent &event);
    void LoadProject(wxCommandEvent &event);
    void StoreProject(wxCommandEvent &event);

    /* Handlers for operations on models */
    void NewSpecification(wxCommandEvent &event);
    void EditSpecification(wxCommandEvent &event);
    void AddSpecification(wxCommandEvent &event);
    void EditSpecificationProperties(wxCommandEvent &event);
    void RemoveSpecification(wxCommandEvent &event);
    void RenameSpecification(wxTreeEvent &event);

    /* Handlers for operations on progress */
    void AddAnalysis(wxCommandEvent &event);
    void RemoveAnalysis(wxCommandEvent &event);
    void PerformAnalysis(wxCommandEvent &event);

    /* Event handler for all context menus */
    void SpawnContextMenu(wxTreeEvent &event);

    /* Activate rename label from context menu */
    void ActivateRename(wxCommandEvent &event);
    void MarkDirty(wxCommandEvent &event);

    /* Generic GUI only operations */
    void Quit(wxCommandEvent &event);
};

/* To connect Specification objects to elements in the tree view */
class SpecificationData : public wxTreeItemData {
  public:
    Specification* specification; /* Pointer to a Specification */

    SpecificationData(Specification* specification) : specification(specification) {
    };
};

#endif /* STUDIO_OVERVIEW_H_ */
