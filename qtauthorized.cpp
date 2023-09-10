#include "qtauthorized.h"
#include <qstring.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <unordered_map>
AuthorizedWindow::AuthorizedWindow(QWidget* parent, std::shared_ptr<ChatClient> cc)
    : QWidget(parent), tableWidget(nullptr), chatDetailsFrame(nullptr), chatTextEdit(nullptr),cc(cc)
{
    setWindowTitle("Chat List");
    setMinimumSize(1024, 640);
    setupTableWidget();

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
    connect(tableButton, &QPushButton::clicked, this, &AuthorizedWindow::newReciever);

    // Create a QTimer
    timer = new QTimer(this);

    // Connect the timer's timeout signal to your custom slot
    connect(timer, &QTimer::timeout, this, &AuthorizedWindow::populateTableData);

    // Set the timer interval to 1 minute (in milliseconds)
    timer->setInterval(1000); // 60,000 ms = 1 minute

    // Start the timer
    timer->start();
    populateTableData();
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

    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);

    tableWidget->setColumnCount(3);
    tableWidget->setColumnWidth(0, 80);
    tableWidget->setColumnWidth(1, 200);
    tableWidget->setColumnWidth(2, 200);
    tableWidget->setMinimumWidth(500);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AuthorizedWindow::populateTableData()
{
    msgs = std::make_unique<std::vector<Message>>(cc->RetrieveMessageStream());
    std::unordered_map<std::string, Message> preview;
    for (auto& data : *msgs)
    {
        std::string previewUser = data.sender == currentOtherUser ? data.sender : data.receiver;
        Message msg;
        msg.content = data.content;
        msg.sender = data.sender;
        msg.receiver = data.receiver;
        msg.time = data.time;
        preview.insert(std::make_pair(previewUser, msg));
    }
    tableWidget->setRowCount(preview.size());
    size_t row = 0;
    for (auto& data : preview)
    {
        QTableWidgetItem* usernameItem = new QTableWidgetItem(data.first.c_str());
        QTableWidgetItem* textItem = new QTableWidgetItem(data.second.content.c_str());
        QTableWidgetItem* dateItem = new QTableWidgetItem(data.second.time.c_str());
        tableWidget->setItem(row, 0, usernameItem);
        tableWidget->setItem(row, 1, textItem);
        tableWidget->setItem(row, 2, dateItem);
        row++;
    }
}
void AuthorizedWindow::newReciever()
{
    auto userList = cc->RetrieveUserList(UserType::User);
    auto it = std::find(userList.begin(), userList.end(), tableLineEdit->text().toStdString());
    if (it != userList.end())
    {
        chatTextEdit->clear();
        currentOtherUser = *it;
        auto header = std::string("Chat with User:");
        header.append(currentOtherUser.c_str());
        chatTextEdit->append(header.c_str());
    }
    else
    {
        chatTextEdit->append("User not found");
    }
    return;

}
void AuthorizedWindow::onSendButtonClicked()
{
    // Handle sending a message from the messageLineEdit
    QString message = messageLineEdit->text();
    // Append the message to the chatTextEdit or send it to the selected chat
    // (This part depends on your application logic)
    if (cc->Send(cc->getUser(), currentOtherUser, message.toStdString()))
    {
        chatTextEdit->append("You: " + message);
        messageLineEdit->clear();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Sending failed");
        msgBox.setText("Sorry, mesage was not sent for unknown reason.");
        msgBox.setIcon(QMessageBox::Warning); // Display an error icon
        msgBox.addButton(QMessageBox::Ok);     // Add an "OK" button

        // Show the message box and wait for the user to click "OK"
        msgBox.exec();
    }
    return;
}
void AuthorizedWindow::onTableRowDoubleClicked(int row, int column)
{
    chatTextEdit->clear();
    QWidget* usernameMessageWidget = qobject_cast<QWidget*>(tableWidget->cellWidget(row, 1));
   
    currentOtherUser = tableWidget->item(row, 0)->text().toStdString();
    auto header = std::string("Chat with User:");
    header.append(currentOtherUser.c_str());
    chatTextEdit->append(header.c_str());
    std::unordered_map<std::string, Message> preview;
    for (auto& data : *msgs)
    {
        if ((data.receiver == currentOtherUser || data.sender == currentOtherUser) && (data.receiver == cc->getUser() || data.sender == cc->getUser()))
        {
            if (data.sender == cc->getUser())
            {
                std::string text = "(" + data.time + ") " + cc->getUser() + " : " + data.content;
                QString qtext(text.c_str());
                chatTextEdit->append(qtext);
            }
            if(data.receiver == cc->getUser())
            {
                std::string text = "(" + data.time + ") " + currentOtherUser + " : " + data.content;
                QString qtext(text.c_str());
                chatTextEdit->append(qtext);
            }
        }
    }
}
