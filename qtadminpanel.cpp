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
#include <unordered_set>

#include <qstring.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <vector>
#include "client.h"


class ServerWrapper {
public:
    ServerWrapper(const std::string& serverAddress) : serverAddress(serverAddress) {
        StartServer();
    }

    ~ServerWrapper() {
        StopServer();
    }

private:
    void StartServer() {
        grpc::ServerBuilder builder;
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);

        server = builder.BuildAndStart();
        std::cout << "Server listening on " << serverAddress << std::endl;
    }

    void StopServer() {
        if (server) {
            server->Shutdown();
            server->Wait();
        }
    }

    std::string serverAddress;
    ChatServer service;
    std::unique_ptr<grpc::Server> server;
};

class AdminPanelWindow : public QWidget, ServerWrapper
{
public:
    AdminPanelWindow(QWidget* parent = nullptr);
    ~AdminPanelWindow();

private:
    void setupTableWidget();

private slots:
    void selectUser(int row, int column);
    void populateTableData();
    void banSelectedUser();

private:
    QTableWidget* tableWidget;
    QLineEdit* tableLineEdit;
    QPushButton* tableButton;
    QHBoxLayout* tableControlLayout;

    QGridLayout* mainLayout;


    QWidget* centralWidget;
    ChatClient cc;
    QTimer* timer;
};

AdminPanelWindow::AdminPanelWindow(QWidget* parent)
    : QWidget(parent), ServerWrapper("localhost:55777"), tableWidget(nullptr), cc()
{
    bool result = cc.Authenticate("localhost-admin", "asd123");
    setWindowTitle("Chat List");
    setMinimumSize(640, 480);
    setupTableWidget();


    tableLineEdit = new QLineEdit(this);
    tableLineEdit->setReadOnly(true);
    tableButton = new QPushButton("Ban", this);
    tableControlLayout = new QHBoxLayout();
    tableControlLayout->addWidget(tableLineEdit);
    tableControlLayout->addWidget(tableButton);
    tableControlLayout->setAlignment(Qt::AlignRight); // Align send button to the right

    // Create a layout to arrange the table widget and chat details frame side by side
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(tableWidget, 0, 0);
    mainLayout->addLayout(tableControlLayout, 1, 0);


    // Connect the double-click event to the slot
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &AdminPanelWindow::selectUser);
    //connect(messageSendButton, &QPushButton::clicked, this, &AdminPanelWindow::onSendButtonClicked);
    connect(tableButton, &QPushButton::clicked, this, &AdminPanelWindow::banSelectedUser);

    // Create a QTimer
    timer = new QTimer(this);

    // Connect the timer's timeout signal to your custom slot
    connect(timer, &QTimer::timeout, this, &AdminPanelWindow::populateTableData);

    // Set the timer interval to 1 minute (in milliseconds)
    timer->setInterval(1000); // 60,000 ms = 1 minute

    // Start the timer
    timer->start();
    populateTableData();
}

AdminPanelWindow::~AdminPanelWindow()
{
    delete tableWidget;
    delete tableLineEdit;
    delete tableButton;
    delete tableControlLayout;

    delete mainLayout;
}

void AdminPanelWindow::setupTableWidget()
{
    tableWidget = new QTableWidget(this);

    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);

    tableWidget->setColumnCount(2);
    tableWidget->setColumnWidth(0, 240);
    tableWidget->setColumnWidth(1, 240);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AdminPanelWindow::populateTableData()
{
    auto userList = cc.RetrieveUserList(UserType::User);
    auto bannedUserList = cc.RetrieveUserList(UserType::Banned);
    std::unordered_map <std::string, std::string> map;
    for (auto& user : userList)
    {
        auto it = std::find(bannedUserList.begin(), bannedUserList.end(), user);
        if (it == bannedUserList.end())
            map.emplace(std::move(user), std::string("Active"));
        else
            map.emplace(std::move(user), std::string("Banned"));

    }
    map.erase(cc.getUser());
    size_t row = 0;
    tableWidget->setRowCount(map.size());
    for (auto& data : map)
    {
        QTableWidgetItem* login = new QTableWidgetItem(data.first.c_str());
        QTableWidgetItem* status = new QTableWidgetItem(data.second.c_str());
        tableWidget->setItem(row, 0, login);
        tableWidget->setItem(row, 1, status);
        row++;
    }
}
void AdminPanelWindow::banSelectedUser()
{
    if(!tableLineEdit->text().toStdString().empty())
        cc.BlockUser(tableLineEdit->text().toStdString());
}
void AdminPanelWindow::selectUser(int row, int column)
{
    tableLineEdit->setText(tableWidget->item(row, 0)->text());
}

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    AdminPanelWindow w;
    w.show();
    return a.exec();
}
