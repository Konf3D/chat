#include "declarewindows.h"


SignUpWindow::SignUpWindow(const wxString& title, const wxSize& size, InitialWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size), parentWindow(parent)
{
    /*
    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* loginTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxTextCtrl* usernameTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxTextCtrl* passwordTextBox = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    wxButton* signUpButton = new wxButton(panel, wxID_ANY, "Sign Up", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* cancelButton = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
*/
    mainSizer->Add(loginTextBox, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(usernameTextBox, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(passwordTextBox, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(signUpButton, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(cancelButton, 0, wxALL | wxEXPAND, 10);

    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    signUpButton->Bind(wxEVT_BUTTON, &SignUpWindow::OnSignUpButtonClick, this);
    cancelButton->Bind(wxEVT_BUTTON, &SignUpWindow::OnCancelButtonClick, this);
}

void SignUpWindow::OnCancelButtonClick(wxCommandEvent& event)
{
    parentWindow->ShowMainWindow();
}

void SignUpWindow::OnSignUpButtonClick(wxCommandEvent& event)
{
    // Implement sign-up logic here
    parentWindow->ShowAuthorizedWindow();
}

void SignUpWindow::OnClose(wxCloseEvent& event)
{
    parentWindow->DetachSignUp();
    parentWindow->ShowMainWindow();
    event.Skip();
}
wxBEGIN_EVENT_TABLE(SignUpWindow, wxFrame)
EVT_CLOSE(SignUpWindow::OnClose)
wxEND_EVENT_TABLE()