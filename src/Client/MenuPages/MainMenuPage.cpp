#include <iostream>

#include "EcnTcpClient.hpp"

#include "DealLogMenuPage.hpp"
#include "BalanceMenuPage.hpp"
#include "OpenDealMenuPage.hpp"

#include "MainMenuPage.hpp"

MainMenuPage::MainMenuPage()
{
    DealLogMenuPage* adbPage     = new DealLogMenuPage(ActiveStatus, BuyType) ;
    DealLogMenuPage* adsPage     = new DealLogMenuPage(ActiveStatus, SellType);
    DealLogMenuPage* cdbPage     = new DealLogMenuPage(ClosedStatus, BuyType) ;
    DealLogMenuPage* cdsPage     = new DealLogMenuPage(ClosedStatus, SellType);
    OpenDealMenuPage* buyPage    = new OpenDealMenuPage(BuyType)    ;
    OpenDealMenuPage* sellPage   = new OpenDealMenuPage(SellType)   ;
    BalanceMenuPage* balancePage = new BalanceMenuPage              ;

    m_items.push_back({ adbPage     , "Журнал активных заявок на покупку USD" });
    m_items.push_back({ adsPage     , "Журнал активных заявок на продажу USD" });
    m_items.push_back({ cdbPage     , "Журнал совершенных сделок покупку USD" });
    m_items.push_back({ cdsPage     , "Журнал совершенных сделок продажу USD" });
    m_items.push_back({ buyPage     , "Купить USD за RUB"                     });
    m_items.push_back({ sellPage    , "Продать USD за RUB"                    });
    m_items.push_back({ balancePage , "Баланс пользователя"                   });
}

MainMenuPage::~MainMenuPage()
{
    for (MenuItem& item : m_items)
    {
        delete item.page;
    }
}

void MainMenuPage::Exec()
{
    system("clear"); 

    while (true)
    {
        std::cout << "Введите число от 1 до " << m_items.size() << " для выбора пункта меню" << std::endl
                  << "или любой другой(ие) символ(ы) для выхода" << std::endl << std::endl;

        for (int i = 0; i < m_items.size(); i++)
        {
            std::cout << i + 1 << ". " << m_items[i].name << std::endl;
        }

        int itemNumber = -1;
        std::cin >> itemNumber;

        if (itemNumber < 1 || itemNumber > m_items.size())
        {
            throw std::out_of_range(
                "Можно выбрать только пункты меню от 1 до " + std::to_string(m_items.size()));
        }

        std::cin.exceptions(std::ios_base::failbit);
        try
        {
            m_items[itemNumber - 1].page->Exec();
        }
        catch(const std::istream::failure& e)
        {
            system("clear");

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Введен(ы) символ(ы), не являющиеся числом. Переход в главное меню." << std::endl << std::endl;
        }
        catch(const std::out_of_range& e)
        {
            system("clear"); 

            std::cout << e.what() << ". Переход в главное меню." << std::endl << std::endl;
        }
        
    }
}