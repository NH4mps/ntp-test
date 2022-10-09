#include <iostream>

#include "EcnTcpClient.hpp"

#include "OpenDealMenuPage.hpp"

static const std::string actions[] = { "купить" "продать"};

void OpenDealMenuPage::Exec()
{
    system("clear"); 

    std::cout << "Для выхода введите любую последовательность символов, "
                 "не подпадающую под условия ввода" << std::endl << std::endl;

    long long count = 0;
    double price = 0;

    while (true)
    {
        std::cout << "Введите объем валюты (USD), который вы хотите " + actions[m_dealType] + ": ";
        std::cin  >> count;
        if (count <= 0)
        {
            throw std::out_of_range("Введен некорректный объем");
        }

        std::cout << std::endl;
        
        std::cout << "Введите цену (RUB): ";
        std::cin  >> price;
        if (price <= 0)
        {
            throw std::out_of_range("Введенна некорретная цена");
        }

        std::cout << EcnTcpClient::Instance().SendOpenDealRequest(m_dealType, count, price) << std::endl << std::endl;
    }
}