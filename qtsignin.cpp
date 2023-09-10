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

    loginLineEdit = new QLineEdit(this);
    loginLineEdit->setPlaceholderText("Login");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QPushButton* signinButton = new QPushButton("Sign In", this);

    layout->addWidget(loginLineEdit);
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
    if(cc->Authenticate(loginLineEdit->text().toStdString(), passwordLineEdit->text().toStdString()))
    {
        emit authorized();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Authorization Failed");
        msgBox.setText("Sorry, authorization has failed.");
        msgBox.setIcon(QMessageBox::Warning); // Display an error icon
        msgBox.addButton(QMessageBox::Ok);     // Add an "OK" button

        // Show the message box and wait for the user to click "OK"
        msgBox.exec();
        return;
    }
    close();
}
