#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "EcnCommon.hpp"

class EcnTcpClient
{
public:
    typedef boost::asio::ip::address Address;
    typedef boost::asio::ip::tcp::socket Socket;
    typedef boost::asio::io_service SocketCtx;

public:
    static EcnTcpClient& Instance()
    {
        static EcnTcpClient inst(g_serverAddress, g_serverPort);
        return inst;
    }

private:
    EcnTcpClient(const Address& serverAddress, short serverPort);

    std::string SendRequest(
        const std::string& sessionId,
        const std::string& requestId,
        const std::string& message);

    // Отправка сообщения на сервер по шаблону.
    void SendMessage(
        const std::string& sessionId,
        const std::string& requestId,
        const std::string& message);

    // Возвращает строку с ответом сервера на последний запрос.
    std::string ReadMessage();

private:
    Address m_serverAddress;
    short   m_serverPort;

    SocketCtx m_socketCtx;
    Socket    m_socket;
};
