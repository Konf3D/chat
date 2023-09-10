#include "wxdeclarewindows.h"

SignUpWindow::SignUpWindow(const wxString& title, const wxSize& size, InitialWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size), parentWindow(parent),cc(parent->getClient())
{

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
    if(cc->Register(loginTextBox->GetValue().ToStdString(), usernameTextBox->GetValue().ToStdString(), passwordTextBox->GetValue().ToStdString()))
        parentWindow->ShowAuthorizedWindow();
    else
    {
        wxMessageBox("Failed to sign up", "Info", wxOK | wxICON_INFORMATION);
    }
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