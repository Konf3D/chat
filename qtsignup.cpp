#include <QVBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QCloseEvent>
#include "qtsignup.h"

SignUpWindow::SignUpWindow(QWidget* parent, std::shared_ptr<ChatClient> cc)
    : QWidget(parent), cc(cc)
{
    setWindowTitle("Sign Up");
    setFixedSize(400, 200);

    QVBoxLayout* layout = new QVBoxLayout(this);

    loginLineEdit = new QLineEdit(this);
    loginLineEdit->setPlaceholderText("Login");

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Username");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Password");


    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QPushButton* signinButton = new QPushButton("Sign In", this);

    layout->addWidget(loginLineEdit);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(signinButton);

    connect(signinButton, &QPushButton::clicked, this, &SignUpWindow::onSignUpCompleted);
}
void SignUpWindow::closeEvent(QCloseEvent* event)
{
    if(true)
    {
        emit closed();
    }
}
void SignUpWindow::onSignUpCompleted()
{
    if (cc->Register(loginLineEdit->text().toStdString(), usernameLineEdit->text().toStdString(), passwordLineEdit->text().toStdString()))
    {
        emit authorized();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Registration Failed");
        msgBox.setText("Sorry, registration has failed.");
        msgBox.setIcon(QMessageBox::Warning); // Display an error icon
        msgBox.addButton(QMessageBox::Ok);     // Add an "OK" button

        // Show the message box and wait for the user to click "OK"
        msgBox.exec();
        return;
    }
    close();
}
