#include "qtauthorized.h"

AuthorizedWindow::AuthorizedWindow(QWidget* parent, std::shared_ptr<ChatClient> cc)
    : QWidget(parent), tableWidget(nullptr), chatDetailsFrame(nullptr), chatTextEdit(nullptr),cc(cc)
{
    setWindowTitle("Chat List");
    setMinimumSize(1024, 640);
    setupTableWidget();
    populateTableData();

    // Create and configure the chat details frame
    chatDetailsFrame = new QFrame(this);
    chatDetailsFrame->setFrameShape(QFrame::StyledPanel);
    chatDetailsFrame->setFrameShadow(QFrame::Sunken);
    chatDetailsFrame->setMinimumWidth(200); // Set the width of the chat details frame

    // Create a text edit to display chat details
    chatTextEdit = new QTextEdit(chatDetailsFrame);
    chatTextEdit->setReadOnly(true);

    // Create a layout for the chat details frame and add the text edit
    chatDetailsLayout = new QVBoxLayout(chatDetailsFrame);
    chatDetailsLayout->addWidget(chatTextEdit);

    // Create a layout for message input area
    messageLayout = new QHBoxLayout();
    messageLineEdit = new QLineEdit(this);
    messageLayout->addWidget(messageLineEdit);
    messageSendButton = new QPushButton("Send", this);
    messageLayout->addWidget(messageSendButton);
    messageLayout->setAlignment(Qt::AlignRight); // Align send button to the right

    tableLineEdit = new QLineEdit(this);
    tableButton= new QPushButton("New chat", this);
    tableControlLayout = new QHBoxLayout();
    tableControlLayout->addWidget(tableLineEdit);
    tableControlLayout->addWidget(tableButton);
    tableControlLayout->setAlignment(Qt::AlignRight); // Align send button to the right

    // Create a layout to arrange the table widget and chat details frame side by side
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(tableWidget,0,0);
    mainLayout->addLayout(tableControlLayout,1,0);
    mainLayout->addWidget(chatDetailsFrame,0,1);
    mainLayout->addLayout(messageLayout,1,1);


    // Connect the double-click event to the slot
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &AuthorizedWindow::onTableRowDoubleClicked);
    connect(messageSendButton, &QPushButton::clicked, this, &AuthorizedWindow::onSendButtonClicked);
}

AuthorizedWindow::~AuthorizedWindow()
{
    delete tableWidget;
    delete tableLineEdit;
    delete tableButton;
    delete tableControlLayout;

    delete chatDetailsFrame;

    delete messageLineEdit;
    delete messageSendButton;
    delete messageLayout;
    delete mainLayout;
}

void AuthorizedWindow::setupTableWidget()
{
    tableWidget = new QTableWidget(this);
    tableWidget->setRowCount(10);
    tableWidget->setColumnCount(3);

    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);

    tableWidget->setColumnWidth(0, 80);
    tableWidget->setColumnWidth(1, 300);
    tableWidget->setColumnWidth(2, 100);
    tableWidget->setMinimumWidth(420);
}

void AuthorizedWindow::populateTableData()
{
    //mainwindow->hide();
    for (int row = 0; row < 10; ++row) {
        QLabel *avatarLabel = new QLabel();
        avatarLabel->setPixmap(QPixmap(":/icons/avatar.png"));
        tableWidget->setCellWidget(row, 0, avatarLabel);

        QWidget *usernameMessageWidget = new QWidget();
        QVBoxLayout *usernameMessageLayout = new QVBoxLayout(usernameMessageWidget);
        QLabel *usernameLabel = new QLabel("User " + QString::number(row));
        QLabel *lastMessageLabel = new QLabel("Last message " + QString::number(row));
        usernameMessageLayout->addWidget(usernameLabel);
        usernameMessageLayout->addWidget(lastMessageLabel);
        tableWidget->setCellWidget(row, 1, usernameMessageWidget);
        tableWidget->setRowHeight(row,64);

        QTableWidgetItem *timestampItem = new QTableWidgetItem(QDateTime::currentDateTime().toString("hh:mm"));
        tableWidget->setItem(row, 2, timestampItem);
    }
}

void AuthorizedWindow::onSendButtonClicked()
{
    // Handle sending a message from the messageLineEdit
    QString message = messageLineEdit->text();
    // Append the message to the chatTextEdit or send it to the selected chat
    // (This part depends on your application logic)
    chatTextEdit->append("You: " + message);
    messageLineEdit->clear();
}
void AuthorizedWindow::onTableRowDoubleClicked(int row, int column)
{
    if (column == 1) {
        // When a row is double-clicked in the "Username/Last Message" column
        // You can set the chat details in the QTextEdit here
        chatTextEdit->setPlainText("Chat details for User " + QString::number(row));
    }
}
