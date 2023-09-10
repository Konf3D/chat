#ifndef SIGNIN_H
#define SIGNIN_H

#include <qwidget.h>
#include <qlineedit.h>
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
    QLineEdit* loginLineEdit = nullptr;

    QLineEdit* passwordLineEdit = nullptr;
};

#endif // SIGNIN_H
