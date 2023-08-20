#include "declarewindows.h"


SearchUsersWindow::SearchUsersWindow(const wxString& title, const wxSize& size, AuthorizedWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size), parentWindow(parent)
{
    // Implement search users window UI here
    // This is just a placeholder for demonstration purposes
    wxPanel* panel = new wxPanel(this);
    wxStaticText* searchLabel = new wxStaticText(panel, wxID_ANY, "Search Users Window");
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(searchLabel, 0, wxALL | wxEXPAND, 10);
    panel->SetSizer(mainSizer);
}

void SearchUsersWindow::OnClose(wxCloseEvent& event)
{
    parentWindow->DetachSearchUsers();
    event.Skip();
}

wxBEGIN_EVENT_TABLE(SearchUsersWindow, wxFrame)
EVT_CLOSE(SearchUsersWindow::OnClose)
wxEND_EVENT_TABLE()