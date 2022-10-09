#pragma once

#include <string>
#include <unordered_map>

#include "json.hpp"
#include "EcnCommon.hpp"

namespace Post
{

void CloseDeals();

}

namespace Responds
{

void LoginRequestHandler(int& userId, const nlohmann::json& requestJson, std::ostream& ostream);
void SellRequestHandler(int& userId, const nlohmann::json& requestJson, std::ostream& ostream);
void GetBalanceRequestHandler(int& userId, const nlohmann::json& requestJson, std::ostream& ostream);
void BuyRequestHandler(int& userId, const nlohmann::json& requestJson, std::ostream& ostream);
void ClosedDealsRequestHandler(int& userId, const nlohmann::json& requestJson, std::ostream& ostream);
void ActiveDealsRequestHandler(int& userId, const nlohmann::json& requestJson, std::ostream& ostream);

typedef void (*Handler)(int& userId, const nlohmann::json& requestJson, std::ostream& ostream);
inline const std::unordered_map<std::string, Handler> map = 
{
    { RequestId::Login       , LoginRequestHandler       }, 
    { RequestId::Balance     , GetBalanceRequestHandler  },
    { RequestId::Sell        , SellRequestHandler        },
    { RequestId::Buy         , BuyRequestHandler         },
    { RequestId::ClosedDeals , ClosedDealsRequestHandler },
    { RequestId::ActiveDeals , ActiveDealsRequestHandler } 
};

}