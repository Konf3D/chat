#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include "mainwindow.h"
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
};

#endif // SIGNUP_H
