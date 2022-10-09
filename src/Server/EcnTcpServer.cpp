#include "EcnTcpServer.hpp"

using boost::asio::ip::tcp;

Session::Session(boost::asio::io_service& ioService) 
    : m_socket(ioService), m_ioService(ioService), m_userId(-1)
{ 
    m_buffer.prepare(min_length);
}

tcp::socket& Session::Socket()
{
    return m_socket;
}

void Session::Start()
{
    boost::asio::async_read_until(m_socket, m_buffer, '\0',
        boost::bind(&Session::HandleRead, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

// Обработка полученного сообщения.
void Session::HandleRead(
    const boost::system::error_code& error,
    size_t bytes_transferred)
{
    if (!error)
    {
        std::istream istream(&m_buffer);

        nlohmann::json j = nlohmann::json::parse(istream);
        std::string requestId = j[Key::RequestId];
        
        std::ostream ostream(&m_buffer);
        Responds::Handler handleRequest = Responds::map.at(requestId);
        handleRequest(m_userId, j, ostream);
        m_buffer.sputc('\0');

        if (requestId == RequestId::Buy || requestId == RequestId::Sell)
        {
            m_ioService.post(Post::CloseDeals);
        }

        boost::asio::async_write(m_socket, m_buffer,
            boost::bind(&Session::HandleWrite, this, 
                boost::asio::placeholders::error));
    }
    else
    {
        delete this;
    }
}

void Session::HandleWrite(const boost::system::error_code& error)
{
    if (!error)
    {
        boost::asio::async_read_until(m_socket, m_buffer, '\0',
            boost::bind(&Session::HandleRead, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        delete this;
    }
}

Server::Server(boost::asio::io_service& io_service)
    : m_ioService(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), g_serverPort))
{
    std::cout << "Server started! Listen " << g_serverPort << " port" << std::endl;

    Session* newSession = new Session(m_ioService);
    acceptor_.async_accept(newSession->Socket(),
        boost::bind(&Server::HandleAccept, this, newSession,
            boost::asio::placeholders::error));
}

void Server::HandleAccept(Session* newSession,
    const boost::system::error_code& error)
{
    if (!error)
    {
        newSession->Start();
        newSession = new Session(m_ioService);
        acceptor_.async_accept(newSession->Socket(),
            boost::bind(&Server::HandleAccept, this, newSession,
                boost::asio::placeholders::error));
    }
    else
    {
        delete newSession;
    }
}