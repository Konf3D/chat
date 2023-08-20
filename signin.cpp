#include "declarewindows.h"
#include "client.h"

SignInWindow::SignInWindow(const wxString& title, const wxSize& size, InitialWindow* parent)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size), parentWindow(parent), cc(parent->getClient())
{
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
    if (cc->Authenticate(loginTextBox->GetValue().ToStdString(), passwordTextBox->GetValue().ToStdString()))
        parentWindow->ShowAuthorizedWindow();
    else
    {
        wxMessageBox("Failed to log in", "Info", wxOK | wxICON_INFORMATION);
    }
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