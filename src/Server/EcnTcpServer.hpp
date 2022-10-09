#pragma once

#include <cstdlib>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include "json.hpp"


#include "EcnCommon.hpp"
#include "Responds.hpp"

using boost::asio::ip::tcp;

class Session
{
public:
    Session(boost::asio::io_service& ioService);

    tcp::socket& Socket();

    void Start();

    // Обработка полученного сообщения.
    void HandleRead(
        const boost::system::error_code& error,
        size_t bytes_transferred);

    void HandleWrite(const boost::system::error_code& error);

private:
    enum { min_length = 1024 };
    tcp::socket m_socket;
    boost::asio::io_service& m_ioService;
    boost::asio::streambuf m_buffer;
    int m_userId;
};

class Server
{
public:
    Server(boost::asio::io_service& io_service);

    void HandleAccept(Session* newSession,
        const boost::system::error_code& error);

private:
    boost::asio::io_service& m_ioService;
    tcp::acceptor acceptor_;
};
