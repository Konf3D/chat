#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>

class SignUpWindow; // Forward declaration
class SignInWindow; // Forward declaration
class AuthorizedWindow; // Forward declaration
class ChatWindow;
class SearchUsersWindow;
class ChatListWindow;
///
class InitialWindow : public wxFrame
{
public:
    InitialWindow(const wxString& title, const wxSize& size);

    void OnSignInButtonClick(wxCommandEvent& event);
    void OnSignUpButtonClick(wxCommandEvent& event);
    void OnExitButtonClick(wxCommandEvent& event);

    void ShowSignInWindow();
    void ShowSignUpWindow();
    void ShowAuthorizedWindow();
    void ShowMainWindow();
    void DetachAuthorized();
    void DetachSignIn();
    void DetachSignUp();
 
private:
    //Child windows
    SignUpWindow* signUpWindow = nullptr;
    SignInWindow* signInWindow = nullptr;
    AuthorizedWindow* authorizedWindow = nullptr;
private:
    //Member objects
    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* signInButton = new wxButton(panel, wxID_ANY, "Sign In", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* signUpButton = new wxButton(panel, wxID_ANY, "Sign Up", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* exitButton = new wxButton(panel, wxID_EXIT, "Exit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
};
///

class SignUpWindow : public wxFrame
{
public:
    SignUpWindow(const wxString& title, const wxSize& size, InitialWindow* parent);
    void OnCancelButtonClick(wxCommandEvent& event);
    void OnSignUpButtonClick(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    wxDECLARE_EVENT_TABLE();
private:
    InitialWindow* parentWindow;
private:
    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* loginTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxTextCtrl* usernameTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxTextCtrl* passwordTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    wxButton* signUpButton = new wxButton(panel, wxID_ANY, "Sign Up", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* cancelButton = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
};
///

class SignInWindow : public wxFrame
{
public:
    SignInWindow(const wxString& title, const wxSize& size, InitialWindow* parent);
    void OnSignInButtonClick(wxCommandEvent& event);
    void OnCancelButtonClick(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    wxDECLARE_EVENT_TABLE();
private:
    InitialWindow* parentWindow;
};
///
class AuthorizedWindow : public wxFrame
{
public:
    AuthorizedWindow(const wxString& title, const wxSize& size, InitialWindow* parent);
    void OnViewChatsButtonClick(wxCommandEvent& event);
    void OnSearchUsersButtonClick(wxCommandEvent& event);
    void OnExitButtonClick(wxCommandEvent& event);
    void ShowMainWindow();
    void DetachChatList();
    void DetachSearchUsers();
    void OnClose(wxCloseEvent& event);
    wxDECLARE_EVENT_TABLE();
private:
    InitialWindow* parentWindow = nullptr;
    ChatListWindow* chatWindow = nullptr;
    SearchUsersWindow* searchUsersWindow = nullptr;
private:
    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* viewChatsButton = new wxButton(panel, wxID_ANY, "View Chats", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* searchUsersButton = new wxButton(panel, wxID_ANY, "Search Users", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* exitButton = new wxButton(panel, wxID_EXIT, "Exit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
};
///
class ChatWindow : public wxFrame {
public:
    ChatWindow(const wxString& title, const wxString& chatName, ChatListWindow* parent);

private:
    ChatListWindow* parent;
    wxString chatName;
    wxTextCtrl* chatBox;
    wxTextCtrl* messageEntry;
    wxButton* sendButton;
    std::string GetTimestamp();
    void OnSendMessage(wxCommandEvent& event);
};
///
class SearchUsersWindow : public wxFrame
{
public:
    SearchUsersWindow(const wxString& title, const wxSize& size, AuthorizedWindow* parent);
private:
    void OnClose(wxCloseEvent& event);
    AuthorizedWindow* parentWindow;

    wxDECLARE_EVENT_TABLE();
};

class ChatListWindow : public wxFrame {
public:
    ChatListWindow(const wxString& title, AuthorizedWindow* parent);

private:
    wxListCtrl* chatList;
    wxButton* blockButton;
    wxButton* addButton;
    AuthorizedWindow* parent;

    void OnClose(wxCloseEvent& event);

    void AddChat(const wxString& name, const wxString& lastMessage, const wxString& timestamp);

    void OnChatItemClick(wxListEvent& event);

    void OnBlockButtonClick(wxCommandEvent& event);

    void OnAddButtonClick(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};