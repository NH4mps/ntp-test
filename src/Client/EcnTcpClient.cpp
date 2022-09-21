#include "json.hpp"

#include "EcnTcpClient.hpp"

using boost::asio::ip::tcp;

EcnTcpClient::EcnTcpClient(const EcnTcpClient::Address& serverAddress, short serverPort)
    : m_serverAddress(serverAddress), m_serverPort(serverPort), m_socket(m_socketCtx)
{ }

std::string EcnTcpClient::SendRequest(
    const std::string& sessionId,
    const std::string& requestId,
    const std::string& message)
{
    tcp::endpoint endpoint(m_serverAddress, m_serverPort);
    m_socket.open(endpoint.protocol());
    m_socket.connect(endpoint);

    SendMessage(sessionId, requestId, message);
    std::string res = ReadMessage();

    m_socket.close();

    return res;
}

// Отправка сообщения на сервер по шаблону.
void EcnTcpClient::SendMessage(
    const std::string& sessionId,
    const std::string& requestId,
    const std::string& message)
{
    nlohmann::json req;
    req["SessionId"] = sessionId;
    req["RequestId"] = requestId;
    req["Message"] = message;

    std::string request = req.dump();
    boost::asio::write(m_socket, boost::asio::buffer(request, request.size()));
}

// Возвращает строку с ответом сервера на последний запрос.
std::string EcnTcpClient::ReadMessage()
{
    boost::asio::streambuf b;
    boost::asio::read_until(m_socket, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}