#pragma once

#include <string>
#include <vector>

#include <sqlite_orm/sqlite_orm.h>

namespace Db
{

struct User
{
    int id;
    std::string name;
    size_t passwordHash;
    double balanceRub;
    double balanceUsd;
};

struct ActiveDeal
{
    int id;
    unsigned long long count;
    double price;
    int placedBy;
    std::time_t placedAt;
    int type;
};

struct ClosedDeal 
{
    int id;
    unsigned long long count;
    double price;
    int boughtBy;
    int soldBy;
    std::time_t placedAt;
};

inline const std::string fileName = "ntp.db";

inline auto InitStorage()
{
    using namespace sqlite_orm;
    return make_storage(fileName,
        make_table("users",
            make_column("id", &User::id, autoincrement(), primary_key()),
            make_column("name", &User::name, unique()),
            make_column("password_hash", &User::passwordHash),
            make_column("balance_rub", &User::balanceRub),
            make_column("balance_usd", &User::balanceUsd)),
        make_table("opened_deals",
            make_column("id", &ActiveDeal::id, autoincrement(), primary_key()),
            make_column("count", &ActiveDeal::count),
            make_column("price", &ActiveDeal::price),
            make_column("placed_by", &ActiveDeal::placedBy),
            make_column("placed_at", &ActiveDeal::placedAt),
            make_column("type", &ActiveDeal::type),
            foreign_key(&ActiveDeal::placedBy).references(&User::id)),
        make_table("closed_deals",
            make_column("id", &ClosedDeal::id, autoincrement(), primary_key()),
            make_column("count", &ClosedDeal::count),
            make_column("price", &ClosedDeal::price),
            make_column("bought_by", &ClosedDeal::boughtBy),    
            make_column("sold_by", &ClosedDeal::soldBy),
            make_column("placed_at", &ClosedDeal::placedAt),
            foreign_key(&ClosedDeal::boughtBy).references(&User::id),
            foreign_key(&ClosedDeal::soldBy).references(&User::id))
    );
}           
using Storage = decltype(InitStorage());

};