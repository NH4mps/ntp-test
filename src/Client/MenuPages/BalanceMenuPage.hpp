#pragma once

#include <iostream>
#include "EcnTcpClient.hpp"

#include "IMenuPage.hpp"

class BalanceMenuPage : public IMenuPage
{
public:
    BalanceMenuPage() {}

    void Exec() override 
    {
        system("clear"); 
        auto balances = EcnTcpClient::Instance().SendGetBalanceRequest();

        std::cout << "Для выхода введите любую последовательность символов" << std::endl << std::endl;
        std::cout << "RUB: " << balances.first << std::endl;
        std::cout << "USD: " << balances.second << std::endl;

        std::string str;
        std::cin >> str;
        system("clear"); 
        return;
    }
};