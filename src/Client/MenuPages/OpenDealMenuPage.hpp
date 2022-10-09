#pragma once

#include "EcnCommon.hpp"

#include "IMenuPage.hpp"

class OpenDealMenuPage : public IMenuPage
{
public:
    OpenDealMenuPage(DealType dealType) : m_dealType(dealType) {}

    void Exec() override;

private:
    DealType m_dealType;
};