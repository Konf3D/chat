#include "declarewindows.h"
#include <wx/listctrl.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

ChatWindow::ChatWindow(const wxString& title, const wxString& chatName, ChatListWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 500)), chatName(chatName),parent(parent) {

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    chatBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    sizer->Add(chatBox, 1, wxEXPAND | wxALL, 5);

    messageEntry = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        wxDefaultSize, wxTE_PROCESS_ENTER);
    messageEntry->Bind(wxEVT_TEXT_ENTER, &ChatWindow::OnSendMessage, this);

    sendButton = new wxButton(this, wxID_ANY, "Send");
    sendButton->Bind(wxEVT_BUTTON, &ChatWindow::OnSendMessage, this);

    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    inputSizer->Add(messageEntry, 1, wxEXPAND | wxALL, 5);
    inputSizer->Add(sendButton, 0, wxALL, 5);

    sizer->Add(inputSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    this->SetSizer(sizer);
    this->Layout();
}

std::string ChatWindow::GetTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&time);

    std::stringstream ss;
    ss << "[" << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << "]";
    return ss.str();
}


void ChatWindow::OnSendMessage(wxCommandEvent& event) {
    wxString message = messageEntry->GetValue();
    if (!message.IsEmpty()) {
        std::string timestamp = GetTimestamp();
        chatBox->AppendText(wxString::Format("%s %s: \"%s\"\n", timestamp, "You", message));
        messageEntry->Clear();
    }
}