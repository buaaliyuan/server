#ifndef _TASK_CHAT_HPP_
#define _TASK_CHAT_HPP_

#include <iostream>
#include <sstream>
#include <string>

#include "./myconnection.hpp"

class TaskChat
{
public:
    TaskChat( MyConnection * connection )
        : m_connection( connection )
    {
    }

    static MyConnection::Action start()
    {
        return MyConnection::Action::Read;
    }

    MyConnection::Action parse(
        char const * const buffer,
        std::size_t const bytesTransferred
    );

    void parseError();

    void process();

private:
    MyConnection * m_connection;
};

typename MyConnection::Action TaskChat::parse(
    char const * const buffer,
    std::size_t const bytesTransferred
)
{
    std::istringstream iss( std::string( buffer, bytesTransferred ) );

    char command, space, cr, lf;
    iss >> command;

    if( command == 'h' )
    {
        std::string id;
        iss >> id;
        m_connection->setId( id );

        const std::string message = "Hello '" + id + "'.";

        m_connection->response( message.c_str(), message.size() );
        m_connection->read();
    }
    else if( command == 'b' )
    {
        std::string message;
        iss >> message;

        m_connection->broadcast( message.c_str(), message.size() );
        m_connection->read();
    }
    else if( command == 'u' )
    {
        std::string id;
        std::string message;
        iss >> id >> message;

        m_connection->unicast( id, message.c_str(), message.size() );
        m_connection->read();
    }
    else if( command == 'd' )
    {
        m_connection->disconnect();
    }
    else if( command == 'l' )
    {
        std::string id;
        std::string message;
        iss >> message;

        m_connection->log( message.c_str(), message.size() );
        m_connection->read();
    }
    else if( command == 'e' )
    {
        std::string message;
        iss >> message;

        m_connection->response( message.c_str(), message.size() );
        m_connection->read();
    }
    else
    {
        std::string const message = "Unknown command '" + std::string( 1, command ) + "'.";

        m_connection->response( message.c_str(), message.size() );
        m_connection->read();
    }

    return MyConnection::Action::Process;
}

void TaskChat::parseError()
{
    std::cerr
        << "Parse error"
        << std::endl;
}

void TaskChat::process()
{
}

#endif

