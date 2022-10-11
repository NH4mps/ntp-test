#include <ctime>

#include <functional>

#include "DataBase.hpp"

#include "Responds.hpp"

using namespace sqlite_orm;

namespace
{

Db::ActiveDeal GetNewDeal(int& userId, const nlohmann::json& requestJson, DealType type)
{
    Db::ActiveDeal res;

    res.id = -1;
    res.count = requestJson[Key::Message][Key::Count].get<unsigned long long>();
    res.price = requestJson[Key::Message][Key::Price].get<double>();
    res.placedAt = std::time(nullptr);
    res.placedBy = userId;
    res.type = type;

    return res;
}

void WriteNotLogginedRespond(
    const nlohmann::json& requestJson, 
    std::ostream& ostream)
{
    nlohmann::json respond;
    respond[Key::RequestId] = requestJson[Key::RequestId];
    respond[Key::Status] = Status::NotLogined;

    ostream << respond.dump();
}

void WriteAlreadyLoggined(
    const nlohmann::json& requestJson, 
    std::ostream& ostream)
{
    nlohmann::json respond;
    respond[Key::RequestId] = requestJson[Key::RequestId];
    respond[Key::RequestId] = Status::AlreadyLogined;

    ostream << respond.dump();
}

void UpdateUserBalances(int boughtId, int soldId, unsigned long long count, double price)
{
    Db::Storage storage = Db::InitStorage();

    storage.update_all(
        set(c(&Db::User::balanceUsd) = c(&Db::User::balanceUsd) + count),
        where(c(&Db::User::id) == boughtId));
    storage.update_all(
        set(c(&Db::User::balanceRub) = c(&Db::User::balanceRub) - price * count),
        where(c(&Db::User::id) == boughtId));
    
    storage.update_all(
        set(c(&Db::User::balanceUsd) = c(&Db::User::balanceUsd) - count),
        where(c(&Db::User::id) == soldId));
    storage.update_all(
        set(c(&Db::User::balanceRub) = c(&Db::User::balanceRub) + price * count),
        where(c(&Db::User::id) == soldId));
}

}

void Post::CloseDeals()
{
    Db::Storage storage = Db::InitStorage();

    auto buyDeals = storage.get_all<Db::ActiveDeal>(
        where(c(&Db::ActiveDeal::type) == static_cast<int>(BuyType)), 
        multi_order_by( order_by(&Db::ActiveDeal::price).desc(),
                        order_by(&Db::ActiveDeal::placedAt).asc()));
    auto sellDeals = storage.get_all<Db::ActiveDeal>(
        where(c(&Db::ActiveDeal::type) == static_cast<int>(SellType)), 
        multi_order_by( order_by(&Db::ActiveDeal::price).asc(),
                        order_by(&Db::ActiveDeal::placedAt).asc()));

    int i = 0;
    int j = 0;

    for ( ; i < buyDeals.size() && j < sellDeals.size(); )
    {
        if (buyDeals[i].price >= sellDeals[j].price)
        {
            auto count = std::min(buyDeals[i].count, sellDeals[j].count);
            buyDeals[i].count -= count;
            sellDeals[j].count -= count;

            storage.insert<Db::ClosedDeal>({
                -1, count, sellDeals[j].price, 
                buyDeals[i].placedBy, sellDeals[j].placedBy, 
                std::time(nullptr)});

            UpdateUserBalances(buyDeals[i].placedBy, sellDeals[j].placedBy, count, sellDeals[j].price);
        }
        else
        {
            break;
        }

        if(buyDeals[i].count == 0)
        {
            storage.remove<Db::ActiveDeal>(buyDeals[i].id);
            i++;
        }
        else
        {
            storage.update<Db::ActiveDeal>(buyDeals[i]);
        }
        if(sellDeals[j].count == 0)
        {
            storage.remove<Db::ActiveDeal>(sellDeals[j].id);
            j++;
        }
        else
        {
            storage.update<Db::ActiveDeal>(sellDeals[j]);
        }
    }
}


void Responds::LoginRequestHandler(
    int& userId, 
    const nlohmann::json& requestJson, 
    std::ostream& ostream)
{
    if (userId >= 0)
    {
        WriteAlreadyLoggined(requestJson, ostream);
        return;
    }

    std::string name = requestJson[Key::Message][Key::Login].get<std::string>();
    std::string pass = requestJson[Key::Message][Key::Password].get<std::string>();
    size_t passHash = std::hash<std::string>()(pass);
    
    Db::Storage storage = Db::InitStorage();

    auto selectedUsers = storage.get_all<Db::User>(where(c(&Db::User::name) == name));

    nlohmann::json respond;
    respond[Key::RequestId] = requestJson[Key::RequestId];
    if (selectedUsers.size())
    {
        if (passHash == selectedUsers[0].passwordHash)
        {
            userId = selectedUsers[0].id;
            respond[Key::Status] = Status::Logined;
        }
        else
        {
            respond[Key::Status] = Status::WrongPassword;
        }
    }
    else
    {
        userId = storage.insert(Db::User { -1, name, passHash, 0, 0 });
        respond[Key::Status] = Status::Registered;
    }
    ostream << respond.dump();
}

void Responds::GetBalanceRequestHandler(int& userId, const nlohmann::json& requestJson, std::ostream& ostream)
{
    Db::Storage storage = Db::InitStorage();

    auto user = storage.get<Db::User>(userId);

    nlohmann::json respond;
    respond[Key::RequestId] = requestJson[Key::RequestId];
    respond[Key::RubBalance] = user.balanceRub;
    respond[Key::UsdBalance] = user.balanceUsd;

    ostream << respond.dump();
}

void Responds::SellRequestHandler(
    int& userId,
    const nlohmann::json& requestJson, 
    std::ostream& ostream)
{
    if (userId < 0) 
    {
        WriteNotLogginedRespond(requestJson, ostream);
        return;
    }    

    Db::Storage storage = Db::InitStorage();

    storage.insert(GetNewDeal(userId, requestJson, SellType));
    
    nlohmann::json respond;
    respond[Key::RequestId] = requestJson[Key::RequestId];
    respond[Key::Status] = Status::Success;

    ostream << respond.dump();
}

void Responds::BuyRequestHandler(
    int& userId,
    const nlohmann::json& requestJson, 
    std::ostream& ostream)
{
    if (userId < 0) 
    {
        WriteNotLogginedRespond(requestJson, ostream);
        return;
    }    

    Db::Storage storage = Db::InitStorage();

    storage.insert(GetNewDeal(userId, requestJson, BuyType));
    
    
    nlohmann::json respond;
    respond[Key::RequestId] = requestJson[Key::RequestId];
    respond[Key::Status] = Status::Success;

    ostream << respond.dump();
}

void Responds::ActiveDealsRequestHandler(
    int& userId,
    const nlohmann::json& requestJson,
    std::ostream& ostream)
{
    if (userId < 0) 
    {
        WriteNotLogginedRespond(requestJson, ostream);
        return;
    }    

    Db::Storage storage = Db::InitStorage();

    std::vector<Db::ActiveDeal> rows;
    if (requestJson[Key::Message][Key::Deal].get<std::string>() == Deal::Buy)
    {
        rows = storage.get_all<Db::ActiveDeal>(
            where(c(&Db::ActiveDeal::type) == static_cast<int>(BuyType) && 
                  c(&Db::ActiveDeal::placedBy) == userId));
    }
    else
    {
        rows = storage.get_all<Db::ActiveDeal>(
            where(c(&Db::ActiveDeal::type) == static_cast<int>(SellType) &&
                  c(&Db::ActiveDeal::placedBy) == userId));
    }

    nlohmann::json respond;
    respond[Key::RequestId] = requestJson[Key::RequestId];
    respond[Key::Status] = Status::Success;
    respond[Key::Rows] = nlohmann::json::array();
    for (auto& row : rows)
    {
        nlohmann::json rowObj;
        rowObj[Key::Log::Id] = row.id;
        rowObj[Key::Log::Price] = row.price;
        rowObj[Key::Log::Count] = row.count;
        rowObj[Key::Log::PlacedAt] = row.placedAt;

        respond[Key::Rows].push_back(rowObj);
    }

    ostream << respond.dump();
}

void Responds::ClosedDealsRequestHandler(
    int& userId,
    const nlohmann::json& requestJson, 
    std::ostream& ostream)
{
    if (userId < 0) 
    {
        WriteNotLogginedRespond(requestJson, ostream);
        return;
    }    

    Db::Storage storage = Db::InitStorage();

    std::vector<Db::ClosedDeal> rows;
    if (requestJson[Key::Message][Key::Deal].get<std::string>() == Deal::Buy)
    {
        rows = storage.get_all<Db::ClosedDeal>(where(c(&Db::ClosedDeal::boughtBy) == userId));
    }
    else
    {
        rows = storage.get_all<Db::ClosedDeal>(where(c(&Db::ClosedDeal::soldBy) == userId));
    }

    nlohmann::json respond;
    respond[Key::RequestId] = requestJson[Key::RequestId];
    respond[Key::Status] = Status::Success;
    respond[Key::Rows] = nlohmann::json::array();
    for (auto& row : rows)
    {
        nlohmann::json rowObj;
        rowObj[Key::Log::Id] = row.id;
        rowObj[Key::Log::Price] = row.price;
        rowObj[Key::Log::Count] = row.count;
        rowObj[Key::Log::PlacedAt] = row.placedAt;

        respond[Key::Rows].push_back(rowObj);
    }

    ostream << respond.dump();
}

