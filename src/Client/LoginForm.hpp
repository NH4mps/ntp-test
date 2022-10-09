#pragma once

#include <exception> 
#include <string>
#include <iostream>

#include "EcnTcpClient.hpp"

class LoginForm
{
public:
    class WrongPassException :  public std::exception
    {
    public:
        WrongPassException() : m_what("Пользователь ввел неверный пароль") { }

        const char* what() const noexcept override { return m_what.data(); }
    private:
        std::string m_what;   
    };

public:
    LoginForm() { }

    void Exec()
    {
        std::cout << "Введите логин пользователя:" << std::endl;
        std::cin >> m_login;
        
        std::cout << "Введите пароль пользователя:" << std::endl;
        std::cin >> m_password;

        std::string status = EcnTcpClient::Instance().SendLoginRequest(m_login, m_password);

        if (status != Status::Logined && status != Status::Registered)
        {
            auto errStr = "LoginForm вернула неожиданный статус: " + status;
            throw std::logic_error(errStr.data());
        }
    }

private:
    std::string m_login;
    std::string m_password;
};