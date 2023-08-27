#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>
#include "client.h"

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
    std::shared_ptr<ChatClient> getClient();
private:
    //Child windows
    std::shared_ptr<ChatClient> cc = std::make_shared<ChatClient>();
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
    //Child windows
    InitialWindow* parentWindow;
private:
    std::shared_ptr<ChatClient> cc;
    //Member objects
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
    //Child windows
    InitialWindow* parentWindow;
private:
    std::shared_ptr<ChatClient> cc;
    //Member objects
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxTextCtrl* loginTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxTextCtrl* passwordTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxButton* signInButton = new wxButton(panel, wxID_ANY, "Sign In", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* cancelButton = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
};
///
class AuthorizedWindow : public wxFrame
{
public:
    AuthorizedWindow(const wxString& title, const wxSize& size, InitialWindow* parent);
    std::shared_ptr<ChatClient> getClient();
    void OnViewChatsButtonClick(wxCommandEvent& event);
    void OnSearchUsersButtonClick(wxCommandEvent& event);
    void OnExitButtonClick(wxCommandEvent& event);
    void ShowMainWindow();
    void DetachChatList();
    void DetachSearchUsers();
    void OnClose(wxCloseEvent& event);

    wxDECLARE_EVENT_TABLE();
private:
    //Child windows
    InitialWindow* parentWindow = nullptr;
    ChatListWindow* chatWindow = nullptr;
    SearchUsersWindow* searchUsersWindow = nullptr;
private:
    //ChatClient stub
    std::shared_ptr<ChatClient> cc;
    //Member objects
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
    std::shared_ptr<ChatClient> cc;
    std::vector<Message> messages;
    std::string me,other,token;
    //Member objects
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
    std::shared_ptr<ChatClient> getClient();
private:
    std::shared_ptr<ChatClient> cc;
    //Member objects
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