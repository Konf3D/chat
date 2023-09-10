#include <QVBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QCloseEvent>
#include "qtsignin.h"

SignInWindow::SignInWindow(QWidget* parent, std::shared_ptr<ChatClient> cc)
    : QWidget(parent), cc(cc)
{
    setWindowTitle("Sign In");
    setFixedSize(400, 200);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLineEdit* usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Login");

    QLineEdit* passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QPushButton* signinButton = new QPushButton("Sign In", this);

    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(signinButton);

    connect(signinButton, &QPushButton::clicked, this, &SignInWindow::onSignInCompleted);
}
void SignInWindow::closeEvent(QCloseEvent* event)
{
    if(true)
    {
        emit closed();
    }
}
void SignInWindow::onSignInCompleted()
{
    if(true)
    {
        emit authorized();
    }
    close();
}
