#include "declarewindows.h"


ChatListWindow::ChatListWindow(const wxString& title, AuthorizedWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 300)),parent(parent) {

    chatList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_SINGLE_SEL);

    // Add columns to the chat list
    chatList->InsertColumn(0, "Name");
    chatList->InsertColumn(1, "Last Message");
    chatList->InsertColumn(2, "Timestamp"); // New column for timestamp

    // Set column widths
    chatList->SetColumnWidth(0, 120);
    chatList->SetColumnWidth(1, 300);
    chatList->SetColumnWidth(2, 120); // Adjust the width for the timestamp column

    // Add some sample data
    AddChat("User1", "Hello there!", "2 minutes ago");
    AddChat("User2", "How are you?", "5 minutes ago");
    AddChat("User3", "Chat preview example", "10 minutes ago");

    // Bind the event to the handler function
    chatList->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ChatListWindow::OnChatItemClick, this);

    // Add buttons for blocking and adding as a friend
    blockButton = new wxButton(this, wxID_ANY, "Block");
    blockButton->Bind(wxEVT_BUTTON, &ChatListWindow::OnBlockButtonClick, this);

    addButton = new wxButton(this, wxID_ANY, "Add as Friend");
    addButton->Bind(wxEVT_BUTTON, &ChatListWindow::OnAddButtonClick, this);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(blockButton, 0, wxALL, 5);
    buttonSizer->Add(addButton, 0, wxALL, 5);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(chatList, 1, wxEXPAND | wxALL, 5);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    this->SetSizer(sizer);
    this->Layout();
}

void ChatListWindow::OnClose(wxCloseEvent& event)
{
    parent->DetachChatList();
    event.Skip();
}

void ChatListWindow::AddChat(const wxString& name, const wxString& lastMessage, const wxString& timestamp) {
    int itemIndex = chatList->InsertItem(chatList->GetItemCount(), name);
    chatList->SetItem(itemIndex, 1, lastMessage);
    chatList->SetItem(itemIndex, 2, timestamp); // Set timestamp in the third column
}

void ChatListWindow::OnChatItemClick(wxListEvent & event) {
    // Get the selected item index
    int selectedIdx = event.GetIndex();
    wxString name = chatList->GetItemText(selectedIdx);

    // Open the chat window for the selected chat
    ChatWindow* chatWindow = new ChatWindow("Chat with " + name, name, this);
    chatWindow->Show(true);
}

void ChatListWindow::OnBlockButtonClick(wxCommandEvent& event) {
    // Handle the block button click event here
    int selectedIdx = chatList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIdx != wxNOT_FOUND) {
        wxString name = chatList->GetItemText(selectedIdx);
        wxMessageBox("Blocked: " + name, "Block User", wxOK | wxICON_INFORMATION, this);
    }
}

void ChatListWindow::OnAddButtonClick(wxCommandEvent& event) {
    // Handle the add button click event here
    int selectedIdx = chatList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIdx != wxNOT_FOUND) {
        wxString name = chatList->GetItemText(selectedIdx);
        wxMessageBox("Added as Friend: " + name, "Add Friend", wxOK | wxICON_INFORMATION, this);
    }
}

wxBEGIN_EVENT_TABLE(ChatListWindow, wxFrame)
EVT_CLOSE(ChatListWindow::OnClose)
wxEND_EVENT_TABLE()