// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "logger/logentry.h"
#include "logger/logger.h"
#include <filesystem>
#include <grpc++/grpc++.h>
#include "client.h"
#include "server.h"
#include "crypto/crypto.h"
#include "sql/sqlconnection.h"

constexpr auto grpcErrorLogFile = "grpc-client.log";
ChatClient::ChatClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(chat::ChatService::NewStub(channel))
{
#ifdef WIN32
    /* check if database files are present*/
   // system("powershell ni usermessages.db");
#elif unix
   // system("touch usermessages.db && chmod 600 usermessages.db");
#endif

}

ChatClient::ChatClient()
    :stub_(chat::ChatService::NewStub(grpc::CreateChannel("localhost:55777", grpc::InsecureChannelCredentials())))
{
}

bool ChatClient::Register(const std::string& email, const std::string& username, const std::string& password)
{
    chat::User user;
    user.set_login(email);
    user.set_username(username);
    user.set_password(HashGenerator::sha256(password));
    chat::Token response;

    grpc::ClientContext context;
    grpc::Status status = stub_->RegisterUser(&context, user, &response);
    if (status.ok()) 
    {
        token_ = response.message();
        this->user_ = email;
        return true;
    }
    else 
    {
        logError("Failed to register", status);
        //grpcLog(grpcErrorLogFile, status);
        return false;
    }
}

bool ChatClient::Authenticate(const std::string& username, const std::string& password)
{
    chat::User user;
    user.set_login(username);
    auto kek = HashGenerator::sha256(password);

    user.set_password(HashGenerator::sha256(password));
    chat::Token response;
    
    grpc::ClientContext context;
    grpc::Status status = stub_->AuthenticateUser(&context, user, &response);
    if (status.ok()) 
    {
        token_ = response.message();
        this->user_ = username;
        return true;
    }
    else 
    {
        logError("Failed to log-in", status);
        return false;
    }
}

bool ChatClient::Send(const std::string& sender, const std::string& receiver, const std::string& content)
{
    chat::Message message;
    message.set_sender(sender);
    message.set_receiver(receiver);
    message.set_content(content);
    chat::Token response;

    grpc::ClientContext context;
    context.AddMetadata(sender, token_);//TODO grpc::AuthContext
    grpc::Status status = stub_->SendMessage(&context, message, &response);
    if (!status.ok()) 
    {

        logError("Failed to send message", status);
        return false;
    }
    return true;
}

bool ChatClient::BlockUser(const std::string& user)
{
    chat::Token token;
    token.set_message(user);
    chat::Token response;
    grpc::ClientContext context;
    context.AddMetadata(user_, token_);
    grpc::Status status = stub_->BlockUser(&context, token, &response);

    if (!status.ok())
    {

        logError("Failed to block user", status);
        return false;
    }
    return true;
}

bool ChatClient::Adduser(const std::string& user)
{
    chat::Token token;
    chat::Token response;
    grpc::ClientContext context;
    context.AddMetadata(token_, user);
    grpc::Status status = stub_->AddFriend(&context, token, &response);

    if (!status.ok())
    {

        logError("Failed to add friend", status);
        return false;
    }
    return true;
}

std::vector<Message> ChatClient::RetrieveMessageStream()
{
    //std::string username = user_;
    chat::Token token;
    token.set_message(token_);
    grpc::ClientContext context;
    std::unique_ptr<grpc::ClientReader<chat::Message>> reader(stub_->GetMessageStream(&context, token));

    chat::Message message;
    std::vector<Message> msgs;
    std::ofstream messagesDBFile(dbMessagesFileName);
    while (reader->Read(&message)) {
        messagesDBFile << message.sender() << '\n' << message.receiver() << '\n' << message.content() << '\n';
        msgs.push_back({ message.sender(),message.receiver(),message.content(),message.date() });
    }

    grpc::Status status = reader->Finish();
    if (!status.ok()) {
        logError("Failed to load messages", status);
    }
    return msgs;
}

std::vector<std::string> ChatClient::RetrieveUserList(const UserType type)
{
    chat::Token token;
    token.set_message(token_);
    grpc::ClientContext context;
    std::unique_ptr<grpc::ClientReader<chat::Token>> reader;
    switch (type)
    {
    case UserType::User:
        reader = stub_->GetUsersList(&context, token);
        break;
    case UserType::Banned:
        reader = stub_->GetUserBannedList(&context, token);
        break;
    case UserType::Friend:
        reader = stub_->GetUserFriendList(&context, token);
        break;
    default:
        break;
    }
    std::vector<std::string> response;
    while (reader->Read(&token))
    {
        response.push_back(token.message());
    }
    grpc::Status status = reader->Finish();
    if (!status.ok()) {
        logError("Failed to load users", status);
    }
    return response;
}

std::string ChatClient::getUser()
{
    return user_;
}

void ChatClient::logError(const std::string& errormsg, const grpc::Status& status)
{
    LogEntry le(errormsg, SeverityLevel::error);
    le.setAdditionalMetadata("Service", "gRPC");
    le.setAdditionalMetadata("Error code", std::to_string(status.error_code()));
    le.setAdditionalMetadata("Error message", status.error_message());
    Logger log(grpcErrorLogFile);
    log.writeLog(le);
}
