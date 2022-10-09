#include <iostream>

#include "EcnTcpClient.hpp"

#include "LoginForm.hpp"
#include "MainMenuPage.hpp"

int main()
{
    LoginForm loginForm;

    MainMenuPage mainMenuPage;

    std::cin.exceptions(std::ios_base::failbit);

    try
    {
        EcnTcpClient::Instance().Start();

        loginForm.Exec();

        mainMenuPage.Exec();
    }
    catch(const std::istream::failure& e)
    {
        system("clear"); 

        std::cout << "Введен(ы) символ(ы), не являющиеся числом. Завершение работы." << std::endl;
    }
    catch(const std::out_of_range& e)
    {
        system("clear"); 
        
        std::cout << e.what() << ". Завершение работы." << std::endl;
    }
    catch(const std::exception& e)
    {
        system("clear");

        std::cout << e.what() << ". Завершение работы" << std::endl;
    }
    catch(...)
    {
        system("clear");

        std::cout << "Непредвиденная ошибка" << std::endl;
    }

    return 0;
}