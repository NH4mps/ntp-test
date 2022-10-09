#pragma once

#include <vector>
#include <string>

#include "IMenuPage.hpp"

class MainMenuPage : public IMenuPage
{
public:
    MainMenuPage();

    ~MainMenuPage();

    void Exec() override;

private:
    struct MenuItem
    {
        IMenuPage* page;
        std::string name;    
    };

private:
    std::vector<MenuItem> m_items;
};
