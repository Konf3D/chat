#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include "qtmainwindow.h"
class SignInWindow : public QWidget
{
    Q_OBJECT
public:
    SignInWindow(QWidget* parent = nullptr, std::shared_ptr<ChatClient> cc = nullptr);

signals:
    void authorized();
    void closed();
private slots:
    void onSignInCompleted();
    void closeEvent(QCloseEvent* event);

private:
    std::shared_ptr<ChatClient> cc;
};

#endif // SIGNIN_H
