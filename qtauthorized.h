#ifndef AUTHORIZED_H
#define AUTHORIZED_H
#include <QMainWindow>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QDateTime>
#include <QFrame>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QSplitter>
#include "qtmainwindow.h"

class AuthorizedWindow : public QWidget
{
public:
    AuthorizedWindow(QWidget *parent = nullptr, std::shared_ptr<ChatClient> cc = nullptr);
    ~AuthorizedWindow();

private:
    void setupTableWidget();
    void populateTableData();
    void onSendButtonClicked();

private slots:
    void onTableRowDoubleClicked(int row, int column);

private:
    QTableWidget *tableWidget;
    QLineEdit *tableLineEdit;
    QPushButton *tableButton;
    QHBoxLayout *tableControlLayout;

    QFrame *chatDetailsFrame;
    QTextEdit *chatTextEdit;

    QLineEdit *messageLineEdit;
    QPushButton *messageSendButton;
    QVBoxLayout *chatDetailsLayout;
    QHBoxLayout *messageLayout;
    QGridLayout *mainLayout;


    QWidget *centralWidget; 
    std::shared_ptr<ChatClient> cc;
};
#endif // AUTHORIZED_H
