#include "declarewindows.h"

#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <algorithm>


SearchUsersWindow::SearchUsersWindow(const wxString& title, const wxSize& size, AuthorizedWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size), parentWindow(parent)
{
    mainSizer = new wxBoxSizer(wxVERTICAL);

    searchBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    userListCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    userListCtrl->InsertColumn(0, "User", wxLIST_FORMAT_LEFT, 150); // User column
    userListCtrl->InsertColumn(1, "Status", wxLIST_FORMAT_LEFT, 150); // Status column


    mainSizer->Add(searchBox, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(userListCtrl, 1, wxEXPAND | wxALL, 10); // Expanding the list control
    userListCtrl->Bind(wxEVT_LIST_ITEM_ACTIVATED, &SearchUsersWindow::OnChatItemClick, this);
    // Bind events
    Bind(wxEVT_TEXT, &SearchUsersWindow::OnSearchBoxTextChanged, this, searchBox->GetId());

    // Populate user list (simplified)
    PopulateUserList();

    SetSizerAndFit(mainSizer); // Set the main sizer and adjust the window size
}

void SearchUsersWindow::OnClose(wxCloseEvent& event)
{
    parentWindow->DetachSearchUsers();
    event.Skip();
}
void SearchUsersWindow::OnChatItemClick(wxListEvent& event) {
    // Get the selected item index
    int selectedIdx = event.GetIndex();
    wxString name = userListCtrl->GetItemText(selectedIdx);

    // Open the chat window for the selected chat
    ChatWindow* chatWindow = new ChatWindow("Chat with " + name, name, parentWindow);
    chatWindow->Show(true);
}
void SearchUsersWindow::PopulateUserList() {
    // Simulated data
    auto msgs = std::make_shared<std::vector<std::string>>(parentWindow->getClient()->RetrieveUserList(UserType::User));

    // Add some sample data
    for (auto& data : *msgs)
    {
        allUsers.push_back({ std::move(data), "Unknown" });
    }
    filteredUsers = allUsers; // Initialize filteredUsers with all users
    UpdateUserList();
}

void SearchUsersWindow::UpdateUserList() {
    userListCtrl->DeleteAllItems();

    for (const auto& user : filteredUsers) {
        AddUserToList(user.username, user.status);
    }
}

void SearchUsersWindow::AddUserToList(const wxString& username, const wxString& status) {
    long itemIndex = userListCtrl->InsertItem(userListCtrl->GetItemCount(), username);
    userListCtrl->SetItem(itemIndex, 1, status);
}

void SearchUsersWindow::UpdateFilteredUserList(const wxString& filter) {
    filteredUsers.clear();

    for (const auto& user : allUsers) {
        if (filter.IsEmpty() || user.username.Lower().Contains(filter.Lower())) {
            filteredUsers.push_back(user);
        }
    }

    UpdateUserList();
}

void SearchUsersWindow::OnBanButtonClick(wxCommandEvent& event) {
    // ...
}

void SearchUsersWindow::OnSearchBoxTextChanged(wxCommandEvent& event) {
    wxString filter = searchBox->GetValue();
    UpdateFilteredUserList(filter);
}

wxBEGIN_EVENT_TABLE(SearchUsersWindow, wxFrame)
EVT_CLOSE(SearchUsersWindow::OnClose)
EVT_BUTTON(wxID_ANY, SearchUsersWindow::OnBanButtonClick)
EVT_TEXT(wxID_ANY, SearchUsersWindow::OnSearchBoxTextChanged)
wxEND_EVENT_TABLE()

