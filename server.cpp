#pragma once
#include <fstream>
#include <iostream>
#include <random>

#include "server.h"

constexpr auto dbFileName = "server.db";
constexpr auto maxTries = 5;
ChatServer::ChatServer()
	:chatDatabase(dbFileName)
{

}

::grpc::Status ChatServer::AuthenticateUser(::grpc::ServerContext* context, const::chat::User* request, ::chat::Token* response)
{
	std::shared_ptr<User> usr(new User);
	chatDatabase.getUserByLogin(request->login(),usr);
	if (usr->status == "banned")
		return grpc::Status(grpc::StatusCode::PERMISSION_DENIED, "Banned by admin");

	if (usr->login.empty() || usr->password != request->password())
	{
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Invalid login/password");
	}
	std::string token;
	int attempt = 0;
	bool condition;
	do
	{
		token = generateToken(32);
		condition = (chatDatabase.insertOrUpdateToken(usr->login,token) || attempt > maxTries);
		++attempt;
	} while (condition == false);
	response->set_message(token);
	if (attempt > maxTries)
		return grpc::Status(grpc::StatusCode::INTERNAL, "Server failed");
	return grpc::Status::OK;
}

::grpc::Status ChatServer::RegisterUser(::grpc::ServerContext* context, const::chat::User* request, ::chat::Token* response)
{
	std::shared_ptr<User> usr(new User);
	chatDatabase.getUserByLogin(request->login(), usr);
	if(!usr->login.empty())
		return grpc::Status(grpc::StatusCode::ALREADY_EXISTS, "Username taken");

	
	chatDatabase.addUser(request->login(), request->username(), request->password());

	std::string token;
	int attempt = 0;
	bool condition;
	do
	{
		token = generateToken(32);
		condition = (chatDatabase.insertOrUpdateToken(request->login(), token) || attempt > maxTries);
		++attempt;
	} while (condition == false);
	response->set_message(token);
	if (attempt > maxTries)
		return grpc::Status(grpc::StatusCode::INTERNAL, "Server failed");
	return grpc::Status::OK;

}

::grpc::Status ChatServer::SendMessage(::grpc::ServerContext* context, const::chat::Message* request, ::chat::Token* response)
{
	auto senderUsernameTokenMeta = context->client_metadata().begin();
	std::shared_ptr<User> usr(new User);
	if(!chatDatabase.getUserByLogin(std::string(senderUsernameTokenMeta->first.begin(), senderUsernameTokenMeta->first.end()),usr))
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Sender credentials invalid");
	std::string token;

	if (!chatDatabase.getTokenByLogin(usr->login,token) 
		|| token.empty() 
		|| token != std::string(senderUsernameTokenMeta->second.begin(), senderUsernameTokenMeta->second.end()))
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "Sender access token invalid");

	usr->login = "";
	if (!chatDatabase.getUserByLogin(request->sender(), usr) || usr->login.empty())
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Sender invalid");

	usr->login = "";
	if (!chatDatabase.getUserByLogin(request->receiver(), usr) || usr->login.empty())
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Receiver invalid");

	if (!chatDatabase.addMessage(request->sender(), request->receiver(), request->content()))
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Server failed");

	return grpc::Status::OK;
}

::grpc::Status ChatServer::GetMessageStream(::grpc::ServerContext* context, const::chat::Token* request, ::grpc::ServerWriter<::chat::Message>* writer)
{
	std::string usr;
	if(!chatDatabase.getLoginByToken(request->message(),usr) || usr.empty())
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "User access token invalid");

	std::shared_ptr<std::vector<Message>> msgs(new std::vector<Message>);
	if(!chatDatabase.getMessagesByUser(usr, msgs))
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Messages not found");

	auto write = [&](const Message& msg)
	{
		chat::Message message;
		message.set_content(msg.content);
		message.set_sender(msg.sender);
		message.set_receiver(msg.content);
		message.set_date(msg.time);
		writer->Write(message);
	};
	std::for_each(msgs->begin(), msgs->end(), write);
	return grpc::Status::OK;
}

::grpc::Status ChatServer::GetUsersList(::grpc::ServerContext* context, const::chat::Token* request, ::grpc::ServerWriter<::chat::Token>* writer)
{
	auto write = [&](const std::string& msg)
	{
		chat::Token message;
		message.set_message(msg);
		writer->Write(message);
	};

	std::string usr;
	if (!chatDatabase.getLoginByToken(request->message(), usr) || usr.empty())
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "User access token invalid");

	std::vector<std::string> data;
	if (!chatDatabase.getUserList(data))
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Messages not found");

	std::for_each(data.begin(), data.end(), write);

	return grpc::Status::OK;
}

::grpc::Status ChatServer::GetUserFriendList(::grpc::ServerContext* context, const::chat::Token* request, ::grpc::ServerWriter<::chat::Token>* writer)
{
	auto write = [&](const std::string& msg)
	{
		chat::Token message;
		message.set_message(msg);
		writer->Write(message);
	};

	std::string usr;
	if (!chatDatabase.getLoginByToken(request->message(), usr) || usr.empty())
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "User access token invalid");

	std::shared_ptr<std::unordered_set<std::string>> data(new std::unordered_set<std::string>);
	if (!chatDatabase.getUserFriendList(usr, data))
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Messages not found");

	std::for_each(data->begin(), data->end(), write);
	return grpc::Status::OK;
}

::grpc::Status ChatServer::GetUserBannedList(::grpc::ServerContext* context, const::chat::Token* request, ::grpc::ServerWriter<::chat::Token>* writer)
{
	auto write = [&](const std::string& msg)
	{
		chat::Token message;
		message.set_message(msg);
		writer->Write(message);
	};

	std::string usr;
	if (!chatDatabase.getLoginByToken(request->message(), usr) || usr.empty())
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "User access token invalid");

	std::shared_ptr<std::unordered_set<std::string>> data(new std::unordered_set<std::string>);
	if (!chatDatabase.getUserBannedList(usr, data))
		return grpc::Status(grpc::StatusCode::NOT_FOUND, "Messages not found");

	std::for_each(data->begin(), data->end(), write);
	return grpc::Status::OK;
}

::grpc::Status ChatServer::BlockUser(::grpc::ServerContext* context, const::chat::Token* request, ::chat::Token* response)
{
	auto senderUsernameTokenMeta = context->client_metadata().begin();
	std::string user1;
	if (!chatDatabase.getLoginByToken(std::string(senderUsernameTokenMeta->first.begin(), senderUsernameTokenMeta->first.end()), user1))
	{
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "User access token invalid");
	}
	std::string user2 = std::string(senderUsernameTokenMeta->second.begin(), senderUsernameTokenMeta->second.end());
	if (!chatDatabase.addBlock(user1, user2))
	{
		return grpc::Status(grpc::StatusCode::UNKNOWN, "Server Failed");
	}
	return grpc::Status::OK;
}

::grpc::Status ChatServer::AddFriend(::grpc::ServerContext* context, const::chat::Token* request, ::chat::Token* response)
{
	auto senderUsernameTokenMeta = context->client_metadata().begin();
	std::string user1;
	if (!chatDatabase.getLoginByToken(std::string(senderUsernameTokenMeta->first.begin(), senderUsernameTokenMeta->first.end()), user1))
	{
		return grpc::Status(grpc::StatusCode::UNAUTHENTICATED, "User access token invalid");
	}
	std::string user2 = std::string(senderUsernameTokenMeta->second.begin(), senderUsernameTokenMeta->second.end());
	if (!chatDatabase.addFriend(user1, user2))
	{
		return grpc::Status(grpc::StatusCode::UNKNOWN, "Server Failed");
	}
	return grpc::Status::OK;
}
