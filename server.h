#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <utility>
#include <grpc++/grpc++.h>
#include "chat-grpc-submodule/chat.pb.h"
#include "chat-grpc-submodule/chat.grpc.pb.h"
#include "./sql/sqlconnection.h"

class ChatServer : public chat::ChatService::Service
{
private:
    //SQLcon sql;
    ChatRepository chatDatabase;
public:
    ChatServer();
    ~ChatServer() = default;
    virtual ::grpc::Status AuthenticateUser(::grpc::ServerContext* context, const ::chat::User* request, ::chat::Token* response) override;
    virtual ::grpc::Status RegisterUser(::grpc::ServerContext* context, const ::chat::User* request, ::chat::Token* response) override;
    virtual ::grpc::Status SendMessage(::grpc::ServerContext* context, const ::chat::Message* request, ::chat::Token* response) override;
    virtual ::grpc::Status GetMessageStream(::grpc::ServerContext* context, const ::chat::Token* request, ::grpc::ServerWriter< ::chat::Message>* writer) override;
    virtual ::grpc::Status GetUsersList(::grpc::ServerContext* context, const ::chat::Token* request, ::grpc::ServerWriter< ::chat::Token>* writer) override;
    virtual ::grpc::Status GetUserFriendList(::grpc::ServerContext* context, const ::chat::Token* request, ::grpc::ServerWriter< ::chat::Token>* writer) override;
    virtual ::grpc::Status GetUserBannedList(::grpc::ServerContext* context, const ::chat::Token* request, ::grpc::ServerWriter< ::chat::Token>* writer) override;
    virtual ::grpc::Status BlockUser(::grpc::ServerContext* context, const ::chat::Token* request, ::chat::Token* response);
    virtual ::grpc::Status AddFriend(::grpc::ServerContext* context, const ::chat::Token* request, ::chat::Token* response);
};

inline std::string generateToken(std::size_t length = 64)
{
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, int(CHARACTERS.size()) - 1);

    std::string random_string;

    for (size_t i = 0; i < length; ++i)
    {
        random_string += CHARACTERS[distribution(generator)];
    }
    return random_string;
}
inline void server()
{
    std::string server_address("localhost:55777");
    ChatServer service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}