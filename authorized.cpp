#include "declarewindows.h"

AuthorizedWindow::AuthorizedWindow(const wxString& title, const wxSize& size, InitialWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size), parentWindow(parent)
{
    /*
    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* viewChatsButton = new wxButton(panel, wxID_ANY, "View Chats", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* searchUsersButton = new wxButton(panel, wxID_ANY, "Search Users", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* exitButton = new wxButton(panel, wxID_EXIT, "Exit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
*/
    mainSizer->Add(viewChatsButton, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(searchUsersButton, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(exitButton, 0, wxALL | wxEXPAND, 10);

    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    viewChatsButton->Bind(wxEVT_BUTTON, &AuthorizedWindow::OnViewChatsButtonClick, this);
    searchUsersButton->Bind(wxEVT_BUTTON, &AuthorizedWindow::OnSearchUsersButtonClick, this);
    exitButton->Bind(wxEVT_BUTTON, &AuthorizedWindow::OnExitButtonClick, this);
}

void AuthorizedWindow::OnViewChatsButtonClick(wxCommandEvent& event)
{
    if (!chatWindow)
    {
        chatWindow = new ChatListWindow("Title",this);
    }
    chatWindow->Show(true);
}

void AuthorizedWindow::OnSearchUsersButtonClick(wxCommandEvent& event)
{
    if (!searchUsersWindow)
    {
        searchUsersWindow = new SearchUsersWindow("Search Users", wxSize(300, 200), this);
    }
    searchUsersWindow->Show(true);
}

void AuthorizedWindow::OnExitButtonClick(wxCommandEvent& event)
{
    parentWindow->ShowMainWindow();
}

void AuthorizedWindow::ShowMainWindow()
{
    this->Show();
    if (chatWindow)
    {
        chatWindow->Close();
        chatWindow = nullptr;
    }
    if (searchUsersWindow)
    {
        searchUsersWindow->Close();
        searchUsersWindow = nullptr;
    }
}

void AuthorizedWindow::DetachChatList()
{
    chatWindow = nullptr;
}

void AuthorizedWindow::DetachSearchUsers()
{
    searchUsersWindow = nullptr;
}

void AuthorizedWindow::OnClose(wxCloseEvent& event)
{
    parentWindow->DetachAuthorized();
    parentWindow->ShowMainWindow();
    event.Skip();
}

wxBEGIN_EVENT_TABLE(AuthorizedWindow, wxFrame)
EVT_CLOSE(AuthorizedWindow::OnClose)
wxEND_EVENT_TABLE()