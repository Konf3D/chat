#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <algorithm>
#include <grpcpp/server_builder.h>
#include <grpcpp/server.h>
#include <iostream>
#include "server.h"
#include "client.h"

class ServerWrapper {
public:
    ServerWrapper(const std::string& serverAddress) : serverAddress(serverAddress) {
        StartServer();
    }

    ~ServerWrapper() {
        StopServer();
    }
    
private:
    void StartServer() {
        grpc::ServerBuilder builder;
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        server = builder.BuildAndStart();
        std::cout << "Server listening on " << serverAddress << std::endl;
    }

    void StopServer() {
        if (server) {
            server->Shutdown();
            server->Wait();
        }
    }

    std::string serverAddress;
    ChatServer service;
    std::unique_ptr<grpc::Server> server;
};

class AdminWindow : public wxFrame {
public:
    AdminWindow(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)),serverWrapper("localhost:55777"),clientApi(),m_timer(this) {
        mainSizer = new wxBoxSizer(wxVERTICAL);

        searchBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        userListCtrl = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
        userListCtrl->InsertColumn(0, "User", wxLIST_FORMAT_LEFT, 150); // User column
        userListCtrl->InsertColumn(1, "Status", wxLIST_FORMAT_LEFT, 150); // Status column

        banButton = new wxButton(this, wxID_ANY, "Ban User");

        mainSizer->Add(searchBox, 0, wxEXPAND | wxALL, 10);
        mainSizer->Add(userListCtrl, 1, wxEXPAND | wxALL, 10); // Expanding the list control
        mainSizer->Add(banButton, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

        // Bind events
        Bind(wxEVT_BUTTON, &AdminWindow::OnBanButtonClick, this, banButton->GetId());
        Bind(wxEVT_TEXT, &AdminWindow::OnSearchBoxTextChanged, this, searchBox->GetId());
        PopulateUserList();
        m_timer.Start(5000);

        // Connect the timer event handler
        Bind(wxEVT_TIMER, &AdminWindow::OnTimer, this, m_timer.GetId());
        SetSizerAndFit(mainSizer); // Set the main sizer and adjust the window size
        bool result = clientApi.Authenticate("localhost-admin", "asd123");

    }

    void PopulateUserList() {
        auto msgs = std::make_shared<std::vector<std::string>>(clientApi.RetrieveUserList(UserType::User));
        allUsers.clear();
        // Add some sample data
        for (auto& data : *msgs)
        {
            allUsers.push_back({ std::move(data), "Unknown" });
        }
        filteredUsers = allUsers; // Initialize filteredUsers with all users
        UpdateFilteredUserList(searchBox->GetValue());
    }

    void UpdateUserList() {
        userListCtrl->DeleteAllItems();

        for (const auto& user : filteredUsers) {
            AddUserToList(user.username, user.status);
        }
    }

    void AddUserToList(const wxString& username, const wxString& status) {
        long itemIndex = userListCtrl->InsertItem(userListCtrl->GetItemCount(), username);
        userListCtrl->SetItem(itemIndex, 1, status);
    }

    void UpdateFilteredUserList(const wxString& filter) {
        filteredUsers.clear();

        for (const auto& user : allUsers) {
            if (filter.IsEmpty() || user.username.Lower().Contains(filter.Lower())) {
                filteredUsers.push_back(user);
            }
        }

        UpdateUserList();
    }

    void OnBanButtonClick(wxCommandEvent& event) {
        m_timer.Stop();
        long selectedItem = userListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        m_timer.Start(5000);
        std::string banTarget = userListCtrl->GetItemText(selectedItem).ToStdString();
        clientApi.BlockUser(banTarget);
    }

    void OnSearchBoxTextChanged(wxCommandEvent& event) {
        wxString filter = searchBox->GetValue();
        UpdateFilteredUserList(filter);
    }
    void OnTimer(wxTimerEvent& event)
    {
        PopulateUserList();
    }

private:
    struct UserData {
        wxString username;
        wxString status;
    };
    wxTimer m_timer; // Declare a wxTimer instance
    ChatClient clientApi;
    ServerWrapper serverWrapper;
    wxBoxSizer* mainSizer; // Main sizer to manage layout
    wxTextCtrl* searchBox;
    wxListCtrl* userListCtrl;
    wxButton* banButton;
    std::vector<UserData> allUsers; // List of all users
    std::vector<UserData> filteredUsers; // List of filtered users

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(AdminWindow, wxFrame)
EVT_BUTTON(wxID_ANY, AdminWindow::OnBanButtonClick)
EVT_TEXT(wxID_ANY, AdminWindow::OnSearchBoxTextChanged)
EVT_TIMER(wxID_ANY, AdminWindow::OnTimer)
wxEND_EVENT_TABLE()

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        AdminWindow* adminWindow = new AdminWindow("Admin Panel");
        adminWindow->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
