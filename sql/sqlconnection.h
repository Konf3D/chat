#pragma once
#define SOCI_DLL
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <iostream>
#include <unordered_set>

struct User
{
    std::string login, username, password, status;
};
struct Message
{
    std::string sender, receiver, content, time;
};

class SqlConnection
{
private:
    std::shared_ptr<soci::session> sql;

public:
    SqlConnection(const std::string& databaseFile);

    std::shared_ptr<soci::session> getSession();

    // You can also add other methods to handle database connections and transactions as needed.
};

class UserRepository
{//GOD OBJECT
private:
    std::shared_ptr<soci::session> connection;
private:
    bool createFriendTable();
    bool createBannedTable();
    bool createUserTable();
    bool createTokenTable();
public:
    bool insertOrUpdateToken(const std::string& login, const std::string& token);
    bool getTokenByLogin(const std::string& login, std::string& response);
    bool getLoginByToken(const std::string& token, std::string& response);
    bool getUserByLogin(const std::string& login, std::shared_ptr<User> response);
    bool getUserFriendList(const std::string& userLogin, std::shared_ptr<std::unordered_set<std::string>> response);
    bool getUserBannedList(const std::string& userLogin, std::shared_ptr<std::unordered_set<std::string>> response);
    bool getUserList(std::vector<std::string>& response);
    bool addUser(const std::string& login, const std::string& username, const std::string& password, const std::string& status = "active");
    bool addFriend(const std::string& user1, const std::string& user2);
    bool addBlock(const std::string& user1, const std::string& user2);
    bool addUserBan(const std::string& login);
    bool removeBlock(const std::string& user1, const std::string& user2);
    bool removeBan(const std::string& login);
public:
    UserRepository(std::shared_ptr<soci::session> conn);
};

class MessageRepository
{
private:
    std::shared_ptr<soci::session> connection;

    bool createMessageTable();
public:
    bool getMessagesByUser(const std::string& userLogin, std::shared_ptr<std::vector<Message>> response);
    bool addMessage(const std::string& sender, const std::string& receiver, const std::string& message);
    MessageRepository(std::shared_ptr<soci::session> conn);

};

class ChatRepository : public SqlConnection, public UserRepository, public MessageRepository
{
public:
    ChatRepository(const std::string& dbfile);
};