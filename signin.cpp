#include "declarewindows.h"


SignInWindow::SignInWindow(const wxString& title, const wxSize& size, InitialWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size), parentWindow(parent)
{
    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* loginTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxTextCtrl* passwordTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    wxButton* signInButton = new wxButton(panel, wxID_ANY, "Sign In", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* cancelButton = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);

    mainSizer->Add(loginTextBox, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(passwordTextBox, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(signInButton, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(cancelButton, 0, wxALL | wxEXPAND, 10);

    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    signInButton->Bind(wxEVT_BUTTON, &SignInWindow::OnSignInButtonClick, this);
    cancelButton->Bind(wxEVT_BUTTON, &SignInWindow::OnCancelButtonClick, this);
}

void SignInWindow::OnCancelButtonClick(wxCommandEvent& event)
{
    parentWindow->ShowMainWindow();
}

void SignInWindow::OnSignInButtonClick(wxCommandEvent& event)
{
    // Implement sign-in logic here
    parentWindow->ShowAuthorizedWindow();
}

void SignInWindow::OnClose(wxCloseEvent& event)
{
    parentWindow->DetachSignIn();
    parentWindow->ShowMainWindow();
    event.Skip();
}

wxBEGIN_EVENT_TABLE(SignInWindow, wxFrame)
EVT_CLOSE(SignInWindow::OnClose)
wxEND_EVENT_TABLE()