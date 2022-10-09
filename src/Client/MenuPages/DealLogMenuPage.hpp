#pragma once

#include <chrono>
#include <iostream>

#include "EcnCommon.hpp"

#include "IMenuPage.hpp"

class DealLogMenuPage : public IMenuPage
{
public:
    DealLogMenuPage(DealStatus dealStatus, DealType dealType) 
        : m_dealStatus(dealStatus), m_dealType(dealType) {}

    void Exec() override 
    {
        std::cout << "Для выхода введите любую последовательность символов" << std::endl << std::endl;

        std::string rawRows = EcnTcpClient::Instance().SendDealLogRequest(m_dealStatus, m_dealType);
        nlohmann::json rows = nlohmann::json::parse(rawRows);

        for (auto& row : rows)
        {
            std::time_t temp = row["placed_at"].get<std::time_t>();
            std::tm* t = std::gmtime(&temp);

            std::cout << "ID: "           << row["id"].get<int>()                     << '\t'     ;
            std::cout << "Объём: "        << row["count"].get<unsigned long long>()   << '\t'     ;
            std::cout << "Цена: "         << row["price"].get<double>()               << '\t'     ;
            std::cout << "Дата и время: " << std::put_time(t, "%Y-%m-%d %I:%M:%S %p") << std::endl;
        }
        
        std::string str;
        std::cin >> str;
        return;
    }

private:
    DealStatus m_dealStatus;
    DealType m_dealType;
};