
#pragma once
#include <grpc++/grpc++.h>
#include <fstream>
#include <vector>
#include "server.h"
constexpr auto dbMessagesFileName = "usermessages.db";
enum class UserType
{
	All = 0,
	User = 1,
	Banned = 2,
	Friend = 3
};
class ChatClient
{
public:
	ChatClient(std::shared_ptr<grpc::Channel> channel);
	ChatClient();
	bool Register(const std::string& email, const std::string& username, const std::string& password);
	bool Authenticate(const std::string& username, const std::string& password);
	bool Message(const std::string& sender, const std::string& receiver, const std::string& content);
	bool BlockUser(const std::string& user);
	bool Adduser(const std::string & user);
	std::vector<chat::Message> RetrieveMessageStream(const std::string& username);
	std::vector<std::string> RetrieveUserList(const UserType type = UserType::User);
private:
	void logError(const std::string& errormsg, const grpc::Status& status);
private:
	std::unique_ptr<chat::ChatService::Stub> stub_;
	std::string token_;
};