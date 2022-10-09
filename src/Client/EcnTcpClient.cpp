#include "EcnTcpClient.hpp"

using boost::asio::ip::tcp;

EcnTcpClient::EcnTcpClient(const EcnTcpClient::Address& serverAddress, short serverPort)
    : m_serverAddress(serverAddress), m_serverPort(serverPort), m_socket(m_socketCtx)
{ }

void EcnTcpClient::Start()
{
    tcp::endpoint ep(m_serverAddress, m_serverPort);
    m_socket.connect(ep);
}

std::string EcnTcpClient::SendRequest(
    const std::string& requestId,
    const nlohmann::json& message)
{
    SendMessage(requestId, message);
    return ReadMessage();
}

// Отправка сообщения на сервер по шаблону.
void EcnTcpClient::SendMessage(
    const std::string& requestId,
    const nlohmann::json& message)
{
    nlohmann::json req;
    req[Key::RequestId] = requestId;
    req[Key::Message] = message;

    std::string request = req.dump();
    boost::asio::write(m_socket, boost::asio::buffer(request.data(), request.size() + 1));
}

// Возвращает строку с ответом сервера на последний запрос.
std::string EcnTcpClient::ReadMessage()
{
    std::string resp;
    boost::asio::read_until(m_socket, boost::asio::dynamic_string_buffer(resp), '\0');
    
    return resp;
}
    
std::string EcnTcpClient::SendOpenDealRequest(DealType dealType, unsigned long long count, double price)
{
    nlohmann::json message;
    message[Key::Count] = count;
    message[Key::Price] = price;

    std::string reqId;
    if (dealType == BuyType)
    {
        reqId = RequestId::Buy;
    }
    else
    {
        reqId = RequestId::Sell;
    }

    nlohmann::json respond = nlohmann::json::parse(SendRequest(reqId, message));

    return respond[Key::Status].get<std::string>();
}

std::string EcnTcpClient::SendDealLogRequest(DealStatus dealStatus,  DealType dealType)
{
    nlohmann::json message;
    if (dealType == BuyType)
    {
        message[Key::Deal] = Deal::Buy;
    }
    else
    {
        message[Key::Deal] = Deal::Sell;
    }

    std::string reqId;
    if (dealStatus == ActiveStatus)
    {
        reqId = RequestId::ActiveDeals;
    }
    else
    {
        reqId = RequestId::ClosedDeals;
    }

    nlohmann::json respond = nlohmann::json::parse(SendRequest(reqId, message));

    return respond[Key::Rows].dump();
}

std::string EcnTcpClient::SendLoginRequest(std::string name, std::string password)
{
    nlohmann::json message;
    message[Key::Login] = name;
    message[Key::Password] = password;
    
    nlohmann::json respond = nlohmann::json::parse(SendRequest(RequestId::Login, message));

    return respond[Key::Status].get<std::string>();
}

std::pair<double, double> EcnTcpClient::SendGetBalanceRequest()
{
    nlohmann::json respond = nlohmann::json::parse(SendRequest(RequestId::Balance));
    
    auto rubBalance = respond[Key::RubBalance].get<double>();
    auto usdBalance = respond[Key::UsdBalance].get<double>();
    return { rubBalance, usdBalance };
}