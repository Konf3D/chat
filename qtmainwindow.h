#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QSharedPointer>
#include "client.h"

class SignInWindow;
class SignUpWindow;
class AuthorizedWindow;
class ChatClient;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private slots:
    void onSignUpClicked();
    void onSignInClicked();
    void onChildWindowClosed();
    void onAuthorizedSignal();

private:
    QPushButton *signUpButton;
    QPushButton *signInButton;
    std::shared_ptr<ChatClient> cc;
    QWidget* currentChild;

    void showChildWindow(SignInWindow *child);
    void showChildWindow(SignUpWindow *child);
    void showChildWindow(AuthorizedWindow *child);
};
#endif // MAINWINDOW_H
