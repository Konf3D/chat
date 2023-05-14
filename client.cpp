// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "logger/logentry.h"
#include "logger/logger.h"
#include <filesystem>
#include <grpc++/grpc++.h>
#include "client.h"
#include "server.h"

constexpr auto grpcErrorLogFile = "grpc-client.log";
ChatClient::ChatClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(chat::ChatService::NewStub(channel))
{
#ifdef WIN32
    /* check if database files are present*/
    system("powershell ni usermessages.db");
#elif unix
    system("touch usermessages.db && chmod 600 usermessages.db");
#endif

}

bool ChatClient::Register(const std::string& email, const std::string& username, const std::string& password)
{
    chat::User user;
    user.set_email(email);
    user.set_username(username);
    user.set_password(password);
    chat::Token response;

    grpc::ClientContext context;
    grpc::Status status = stub_->RegisterUser(&context, user, &response);
    if (status.ok()) 
    {
        token_ = response.message();
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
    user.set_username(username);
    user.set_password(password);
    chat::Token response;
    
    grpc::ClientContext context;
    grpc::Status status = stub_->AuthenticateUser(&context, user, &response);
    if (status.ok()) 
    {
        token_ = response.message();
        return true;
    }
    else 
    {
        logError("Failed to log-in", status);
        return false;
    }
}

bool ChatClient::Message(const std::string& sender, const std::string& receiver, const std::string& content)
{
    chat::Message message;
    message.set_sender(sender);
    message.set_receiver(receiver);
    message.set_content(content);
    chat::Token response;

    grpc::ClientContext context;
    context.AddMetadata(sender, token_);
    grpc::Status status = stub_->SendMessage(&context, message, &response);
    if (!status.ok()) 
    {

        logError("Failed to send message", status);
        return false;
    }
    return true;
}

void ChatClient::RetrieveMessageStream(const std::string& username)
{
    chat::Token token;
    token.set_message(token_);
    grpc::ClientContext context;
    std::unique_ptr<grpc::ClientReader<chat::Message>> reader(stub_->GetMessageStream(&context, token));

    chat::Message message;

    std::ofstream messagesDBFile(dbMessagesFileName, std::ios::app);
    while (reader->Read(&message)) {
        messagesDBFile << message.sender() << '\n' << message.receiver() << '\n' << message.content() << '\n';
    }

    grpc::Status status = reader->Finish();
    if (!status.ok()) {
        logError("Failed to load messages", status);
    }
    return;
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
