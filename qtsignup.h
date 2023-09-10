#ifndef SIGNUP_H
#define SIGNUP_H

#include <qwidget.h>
#include <qlineedit.h>
#include "qtmainwindow.h"
class SignUpWindow : public QWidget
{
    Q_OBJECT
public:
    SignUpWindow(QWidget* parent = nullptr, std::shared_ptr<ChatClient> cc = nullptr);

signals:
    void authorized();
    void closed();
private slots:
    void onSignUpCompleted();
    void closeEvent(QCloseEvent* event);

private:
    std::shared_ptr<ChatClient> cc;
    QLineEdit* loginLineEdit = nullptr;
    QLineEdit* usernameLineEdit = nullptr;
    QLineEdit* passwordLineEdit = nullptr;
};

#endif // SIGNUP_H
