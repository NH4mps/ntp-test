#pragma once

#include <boost/asio.hpp>

#define stringify(Text) Text = #Text

inline const short g_serverPort = 5555;
inline const boost::asio::ip::address g_serverAddress = boost::asio::ip::address::from_string("127.0.0.1");

enum DealType
{
    BuyType = 0,
    SellType = 1
};

enum DealStatus
{
    ActiveStatus = 0,
    ClosedStatus
};

namespace Key
{
inline const std::string stringify(RequestId);
inline const std::string stringify(Status);
inline const std::string stringify(Message);
inline const std::string stringify(Count);
inline const std::string stringify(Price);
inline const std::string stringify(Login);
inline const std::string stringify(Password);
inline const std::string stringify(Rows);
inline const std::string stringify(Deal);
inline const std::string stringify(RubBalance);
inline const std::string stringify(UsdBalance);

namespace Log
{
inline const std::string stringify(Id);
inline const std::string stringify(Count);
inline const std::string stringify(Price);
inline const std::string stringify(PlacedAt);
}
};

namespace RequestId
{
inline const std::string stringify(Login);
inline const std::string stringify(Sell);
inline const std::string stringify(Buy);
inline const std::string stringify(ActiveDeals);
inline const std::string stringify(ClosedDeals);
inline const std::string stringify(Balance);
};

namespace Deal
{
inline const std::string stringify(Sell);
inline const std::string stringify(Buy);
};

namespace Status
{
inline const std::string stringify(Success);
inline const std::string stringify(WrongPassword);
inline const std::string stringify(Logined);
inline const std::string stringify(Registered);
inline const std::string stringify(NotLogined);
inline const std::string stringify(AlreadyLogined);
}