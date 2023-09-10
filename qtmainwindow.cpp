#include "qtmainwindow.h"
#include "qtsignup.h"
#include "qtsignin.h"
#include "qtauthorized.h"

MainWindow::MainWindow()
    :currentChild(nullptr),cc(new ChatClient)
{
    setWindowTitle("Main Window");
    setFixedSize(300, 300);

    signUpButton = new QPushButton("Sign Up", this);
    signUpButton->move(100, 100);
    connect(signUpButton, &QPushButton::clicked, this, &MainWindow::onSignUpClicked);

    signInButton = new QPushButton("Sign In", this);
    signInButton->move(100, 200);
    connect(signInButton, &QPushButton::clicked, this, &MainWindow::onSignInClicked);
}

void MainWindow::onSignUpClicked()
{
    auto child = new SignUpWindow(nullptr, cc);
    currentChild = child;
    child->show();
    child->setAttribute(Qt::WA_DeleteOnClose);
    connect(child, &SignUpWindow::closed, this, &MainWindow::onChildWindowClosed);
    connect(child, &SignUpWindow::authorized,this, &MainWindow::onAuthorizedSignal);
    hide();
}

void MainWindow::onSignInClicked()
{
    auto child = new SignInWindow(nullptr, cc);
    currentChild = child;
    child->show();
    child->setAttribute(Qt::WA_DeleteOnClose);
    connect(child, &SignInWindow::closed, this, &MainWindow::onChildWindowClosed);
    connect(child, &SignInWindow::authorized,this, &MainWindow::onAuthorizedSignal);
    hide();
}
void MainWindow::onAuthorizedSignal()
{
    onChildWindowClosed();
    auto child = new AuthorizedWindow(nullptr, cc);
    currentChild = child;
    child->show();
    child->setAttribute(Qt::WA_DeleteOnClose);
    connect(child, &AuthorizedWindow::destroyed, this, &MainWindow::onChildWindowClosed);
    close();
}
void MainWindow::onChildWindowClosed()
{
    show();
    //delete currentChild;
    currentChild = nullptr;
}
