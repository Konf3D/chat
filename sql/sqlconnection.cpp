#include "sqlconnection.h"
#include "sqlconnection.h"
#include "sqlconnection.h"
#include "sqlconnection.h"
#include <memory>
SqlConnection::SqlConnection(const std::string& databaseFile) : sql(std::make_shared<soci::session>(soci::sqlite3, databaseFile))
{
    // Optionally, you can specify other database options here
}

std::shared_ptr<soci::session> SqlConnection::getSession()
{
    return sql;
}

bool UserRepository::createFriendTable()
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Create the table with foreign key constraints
        *connection << "CREATE TABLE IF NOT EXISTS friendlist ("
            "user1 TEXT REFERENCES user(login),"
            "user2 TEXT REFERENCES user(login),"
            "PRIMARY KEY(user1, user2))";
        // Commit the transaction
        tr.commit();
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during table creation
        std::cerr << "Error creating friendlist table: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::createBannedTable()
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Create the table with foreign key constraints
        *connection << "CREATE TABLE IF NOT EXISTS bannedlist ("
            "user1 TEXT REFERENCES user(login),"
            "user2 TEXT REFERENCES user(login),"
            "PRIMARY KEY(user1, user2))";

        // Commit the transaction
        tr.commit();
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during table creation
        std::cerr << "Error creating bannedlist table: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::createUserTable()
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Create the table with the desired columns
        *connection << "CREATE TABLE IF NOT EXISTS user ("
            "login TEXT PRIMARY KEY,"
            "username TEXT,"
            "password TEXT,"
            "status TEXT)";

        // Commit the transaction
        tr.commit();
        addUser("localhost-admin", "localhost-admin", "54d5cb2d332dbdb4850293caae4559ce88b65163f1ea5d4e4b3ac49d772ded14", "admin");
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during table creation
        std::cerr << "Error creating user table: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::createTokenTable()
{
    try
    {
        // Create the "token" table with a foreign key referencing the "login" column in the "users" table
        *connection << "CREATE TABLE token ("
            "login TEXT PRIMARY KEY REFERENCES users(login),"
            "token TEXT NOT NULL"
            ")";

        std::cout << "Token table created successfully." << std::endl;

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error creating tables: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::insertOrUpdateToken(const std::string& login, const std::string& token)
{
    try
    {
        // Check if the login already exists in the "tokens" table
        int count = 0;
        *connection << "SELECT COUNT(*) FROM token WHERE login = :login", soci::use(login), soci::into(count);

        if (count == 0)
        {
            // The login does not exist, insert a new record
            *connection << "INSERT INTO token (login, token) VALUES (:login, :token)",
                soci::use(login), soci::use(token);
        }
        else
        {
            // The login exists, update the existing record
            *connection << "UPDATE token SET token = :token WHERE login = :login",
                soci::use(token), soci::use(login);
        }

        return true; // Insert or update successful
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error inserting or updating token: " << e.what() << std::endl;
        return false; // Insert or update failed
    }
}
bool UserRepository::getTokenByLogin(const std::string& login, std::string& response)
{
    {
        try
        {
            soci::rowset<soci::row> rs = ((*connection).prepare <<
                "SELECT token FROM token WHERE login = :login",
                soci::use(login));

            for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
            {
                soci::row const& row = *it;
                response = row.get<std::string>(0);
            }

            return true;
        }
        catch (const std::exception& e)
        {
            // Handle any exceptions thrown during the query
            std::cerr << "Error retrieving token data: " << e.what() << std::endl;
            return false; // Query failed
        }
    }
}
bool UserRepository::getLoginByToken(const std::string& token, std::string& response)
{
    {
        try
        {
            soci::rowset<soci::row> rs = ((*connection).prepare <<
                "SELECT login FROM token WHERE token = :token",
                soci::use(token));

            for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
            {
                soci::row const& row = *it;
                response = row.get<std::string>(0);
            }

            return true;
        }
        catch (const std::exception& e)
        {
            // Handle any exceptions thrown during the query
            std::cerr << "Error retrieving token data: " << e.what() << std::endl;
            return false; // Query failed
        }
    }
}
bool UserRepository::getUserByLogin(const std::string& login, std::shared_ptr<User> response)
{
    try
    {
        soci::rowset<soci::row> rs = ((*connection).prepare <<
            "SELECT login, username, password, status FROM user WHERE login = :login",
            soci::use(login));

        for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
        {
            soci::row const& row = *it;
            response->login = row.get<std::string>(0);
            response->username = row.get<std::string>(1);
            response->password = row.get<std::string>(2);
            response->status = row.get<std::string>(3);
        }

        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the query
        std::cerr << "Error retrieving user data: " << e.what() << std::endl;
        return false; // Query failed
    }
}
bool UserRepository::getUserFriendList(const std::string& userLogin, std::shared_ptr<std::unordered_set<std::string>> response)
{
    try
    {

        soci::rowset<soci::row> rs1 = ((*connection).prepare <<
            "SELECT user1 FROM friendlist WHERE user2 = :login",
            soci::use(userLogin));
        soci::rowset<soci::row> rs2 = ((*connection).prepare <<
            "SELECT user2 FROM friendlist WHERE user1 = :login",
            soci::use(userLogin));
        std::unordered_set<std::string> set;
        for (soci::rowset<soci::row>::const_iterator it = rs1.begin(); it != rs1.end(); ++it)
        {
            soci::row const& row = *it;
            set.insert(row.get<std::string>(0));
        }
        for (soci::rowset<soci::row>::const_iterator it = rs2.begin(); it != rs2.end(); ++it)
        {
            soci::row const& row = *it;
            set.insert(row.get<std::string>(0));
        }
        response = std::make_unique<std::unordered_set<std::string>>(set);
        // Check if any friends were retrieved
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the queries
        std::cerr << "Error retrieving user's friend list: " << e.what() << std::endl;
        return false; // Query failed
    }
}
bool UserRepository::getUserBannedList(const std::string& userLogin, std::shared_ptr<std::unordered_set<std::string>> response)
{
    try
    {

        soci::rowset<soci::row> rs1 = ((*connection).prepare <<
            "SELECT user1 FROM bannedlist WHERE user2 = :login",
            soci::use(userLogin));
        soci::rowset<soci::row> rs2 = ((*connection).prepare <<
            "SELECT user2 FROM bannedlist WHERE user1 = :login",
            soci::use(userLogin));
        for (soci::rowset<soci::row>::const_iterator it = rs1.begin(); it != rs1.end(); ++it)
        {
            soci::row const& row = *it;
            response->insert(row.get<std::string>(0));
        }
        for (soci::rowset<soci::row>::const_iterator it = rs2.begin(); it != rs2.end(); ++it)
        {
            soci::row const& row = *it;
            response->insert(row.get<std::string>(0));
        }
        // Check if any friends were retrieved
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the queries
        std::cerr << "Error retrieving user's friend list: " << e.what() << std::endl;
        return false; // Query failed
    }
}
bool UserRepository::getUserList(std::vector<std::string>& response)
{
    try
    {

        soci::rowset<soci::row> rs1 = ((*connection).prepare <<
            "SELECT login FROM user");
        for (soci::rowset<soci::row>::const_iterator it = rs1.begin(); it != rs1.end(); ++it)
        {
            soci::row const& row = *it;
            response.push_back(row.get<std::string>(0));
        }
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the queries
        std::cerr << "Error retrieving user's friend list: " << e.what() << std::endl;
        return false; // Query failed
    }
}
bool UserRepository::addUserBan(const std::string& login)
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Update the status column for the specified user login
        *connection << "UPDATE user SET status = 'banned' WHERE login = :login",
            soci::use(login);

        // Commit the transaction
        tr.commit();
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the update
        std::cerr << "Error banning user: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::removeBlock(const std::string& user1, const std::string& user2)
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Insert the new friend pair into the "friendlist" table
        *connection << "DELETE FROM bannedlist WHERE user1 = :user1 AND user2 = :user2",
            soci::use(user1),
            soci::use(user2);

        // Commit the transaction
        tr.commit();

        std::cout << "Users blocked successfully." << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the insertion
        std::cerr << "Error blocking users: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::removeBan(const std::string& login)
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Update the status column for the specified user login
        *connection << "UPDATE user SET status = 'active' WHERE login = :login",
            soci::use(login);

        // Commit the transaction
        tr.commit();
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the update
        std::cerr << "Error removing ban: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::addUser(const std::string& login, const std::string& username, const std::string& password, const std::string& status)
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Insert the new user into the "user" table
        *connection << "INSERT INTO user (login, username, password, status) "
            "VALUES (:login, :username, :password, :status)",
            soci::use(login), soci::use(username), soci::use(password), soci::use(status);

        // Commit the transaction
        tr.commit();

        std::cout << "User added successfully: " << login << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the insertion
        std::cerr << "Error adding user: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::addFriend(const std::string& user1, const std::string& user2)
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Insert the new friend pair into the "friendlist" table
        *connection << "INSERT INTO friendlist (user1, user2) "
            "VALUES (:user1, :user2)",
            soci::use(user1), soci::use(user2);

        // Commit the transaction
        tr.commit();

        std::cout << "Friend pair added successfully." << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the insertion
        std::cerr << "Error adding friend pair: " << e.what() << std::endl;
        return false;
    }
}
bool UserRepository::addBlock(const std::string& user1, const std::string& user2)
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Insert the new friend pair into the "friendlist" table
        *connection << "INSERT INTO bannedlist (user1, user2) "
            "VALUES (:user1, :user2)",
            soci::use(user1), soci::use(user2);

        // Commit the transaction
        tr.commit();

        std::cout << "Users blocked successfully." << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the insertion
        std::cerr << "Error blocking users: " << e.what() << std::endl;
        return false;
    }
}
UserRepository::UserRepository(std::shared_ptr<soci::session> conn) : connection(conn)
{
    createUserTable() && createFriendTable() && createBannedTable() && createTokenTable();
}


bool MessageRepository::createMessageTable()
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Create the table with foreign key constraints
        *connection << "CREATE TABLE IF NOT EXISTS message ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "receiver TEXT REFERENCES user(login),"
            "sender TEXT REFERENCES user(login),"
            "timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
            "message TEXT)";

        // Commit the transaction
        tr.commit();
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during table creation
        std::cerr << "Error creating message table: " << e.what() << std::endl;
        return false;
    }
}
bool MessageRepository::getMessagesByUser(const std::string& userLogin, std::shared_ptr<std::vector<Message>> response)
{
    try
    {
        soci::rowset<soci::row> rs = ((*connection).prepare <<
            "SELECT sender, receiver, message, timestamp FROM message WHERE sender = :login OR receiver = :login ",
            soci::use(userLogin));

        // Fetch the rows and populate the messages vector
        Message message;
        for (soci::rowset<soci::row>::const_iterator it = rs.begin(); it != rs.end(); ++it)
        {
            soci::row const& row = *it;
            message.sender = row.get<std::string>(0);
            message.receiver = row.get<std::string>(1);
            message.content = row.get<std::string>(2);
            message.time = row.get<std::string>(3);
            response->push_back(message);
        }

        // Check if any messages were retrieved
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the query
        std::cerr << "Error retrieving messages: " << e.what() << std::endl;
        return false; // Query failed
    }
}
bool MessageRepository::addMessage(const std::string& sender, const std::string& receiver, const std::string& message)
{
    try
    {
        // Begin a transaction
        soci::transaction tr(*connection);

        // Insert the new message into the "message" table
        *connection << "INSERT INTO message (sender, receiver, message) "
            "VALUES (:sender, :receiver, :message)",
            soci::use(sender), soci::use(receiver), soci::use(message);

        // Commit the transaction
        tr.commit();

        std::cout << "Message added successfully." << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions thrown during the insertion
        std::cerr << "Error adding message: " << e.what() << std::endl;
        return false;
    }
}
MessageRepository::MessageRepository(std::shared_ptr<soci::session> conn) : connection(conn)
{
    createMessageTable();
}
ChatRepository::ChatRepository(const std::string& dbfile)
    :SqlConnection(dbfile), UserRepository(SqlConnection::getSession()), MessageRepository(SqlConnection::getSession())
{
    // Optionally, you can specify other database options here
}