#include "declarewindows.h"

InitialWindow::InitialWindow(const wxString& title, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size), signUpWindow(nullptr), signInWindow(nullptr), authorizedWindow(nullptr)
{
    /*
    wxPanel* panel = new wxPanel(this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* signInButton = new wxButton(panel, wxID_ANY, "Sign In", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* signUpButton = new wxButton(panel, wxID_ANY, "Sign Up", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
    wxButton* exitButton = new wxButton(panel, wxID_EXIT, "Exit", wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
*/
    mainSizer->Add(signInButton, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(signUpButton, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(exitButton, 0, wxALL | wxEXPAND, 10);

    panel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    signInButton->Bind(wxEVT_BUTTON, &InitialWindow::OnSignInButtonClick, this);
    signUpButton->Bind(wxEVT_BUTTON, &InitialWindow::OnSignUpButtonClick, this);
    exitButton->Bind(wxEVT_BUTTON, &InitialWindow::OnExitButtonClick, this);
}

void InitialWindow::OnSignInButtonClick(wxCommandEvent& event)
{
    ShowSignInWindow();
}

void InitialWindow::OnSignUpButtonClick(wxCommandEvent& event)
{
    ShowSignUpWindow();
}

void InitialWindow::OnExitButtonClick(wxCommandEvent& event)
{
    Close(true);
}

void InitialWindow::ShowAuthorizedWindow()
{
    if (signInWindow)
    {
        signInWindow->Close();
        signInWindow = nullptr;
    }
    if (signUpWindow)
    {
        signUpWindow->Close();
        signUpWindow = nullptr;
    }
    if (!authorizedWindow)
    {
        authorizedWindow = new AuthorizedWindow("Authorized Window", wxSize(250, 150), this);
    }
    authorizedWindow->Show(true);
    this->Hide();
}

void InitialWindow::ShowSignInWindow()
{
    if (!signInWindow)
    {
        signInWindow = new SignInWindow("Sign In", wxSize(250, 150), this);
    }
    signInWindow->Show(true);
    this->Hide();
}

void InitialWindow::ShowSignUpWindow()
{
    if (!signUpWindow)
    {
        signUpWindow = new SignUpWindow("Sign Up", wxSize(250, 180), this);
    }
    signUpWindow->Show(true);
    this->Hide();
}

void InitialWindow::ShowMainWindow()
{
    this->Show();
    if (signInWindow)
    {
        signInWindow->Close();
        signInWindow = nullptr;
    }
    if (signUpWindow)
    {
        signUpWindow->Close();
        signUpWindow = nullptr;
    }
    if (authorizedWindow)
    {
        authorizedWindow->Close();
        authorizedWindow = nullptr;
    }
}

void InitialWindow::DetachAuthorized()
{
    authorizedWindow = nullptr;
}

void InitialWindow::DetachSignIn()
{
    signInWindow = nullptr;
}

void InitialWindow::DetachSignUp()
{
    signUpWindow = nullptr;
}

std::shared_ptr<ChatClient> InitialWindow::getClient()
{
    return cc;
}
