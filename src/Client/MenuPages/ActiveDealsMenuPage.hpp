#pragma once

#include "EcnCommon.hpp"

#include "IMenuPage.hpp"

class ActiveDealsMenuPage : public IMenuPage
{
public:
    ActiveDealsMenuPage(DealType dealType) : m_dealType(dealType) {}

    void Exec() override { }

private:
    DealType m_dealType;
};