#include <cstdio>
#include <iostream>

#include <gtest/gtest.h>

#include "DataBase.hpp"
#include "EcnCommon.hpp"
#include "Responds.hpp"

namespace std
{

bool operator==(const Db::User& a, const Db::User& b)
{
	return	(a.id == b.id) &&
			(a.balanceRub == b.balanceRub) &&
			(a.balanceUsd == b.balanceUsd) &&
			(a.name == b.name) &&
			(a.passwordHash == b.passwordHash);
}

bool operator==(const Db::ActiveDeal& a, const Db::ActiveDeal& b)
{
	return	(a.id == b.id) &&
			(a.count == b.count) &&
			(a.price == b.price) &&
			(a.type == b.type) &&
			(a.placedBy == b.placedBy);
}

bool operator==(const Db::ClosedDeal& a, const Db::ClosedDeal& b)
{
	return	(a.id == b.id) &&
			(a.count == b.count) &&
			(a.price == b.price) &&
			(a.soldBy == b.soldBy) &&
			(a.boughtBy == b.boughtBy);
}

}
class DealsClosing : public ::testing::Test
{
protected:
    void SetUp()
	{
        auto storage = Db::InitStorage();
        storage.sync_schema();
        storage.insert<Db::User>({ 1, "user1", 1234, 0, 0 });
        storage.insert<Db::User>({ 2, "user2", 1234, 0, 0 });
        storage.insert<Db::User>({ 3, "user3", 1234, 0, 0 });
	}

	void TearDown()
	{
        std::remove(Db::fileName.data());
	}
	
	void InitResult()
	{
		auto storage = Db::InitStorage();
		current.activeDeals = 
			storage.get_all<Db::ActiveDeal>(sqlite_orm::order_by(&Db::ActiveDeal::id));
		current.closedDeals = 
			storage.get_all<Db::ClosedDeal>(sqlite_orm::order_by(&Db::ClosedDeal::id));
		current.users =
			storage.get_all<Db::User>(sqlite_orm::order_by(&Db::User::id));
		//int i = 0;
		//for (auto& deal : current.closedDeals)
		//{
			//deal.placedAt = ++i;
		//}
	}

	struct Config
	{
		bool operator==(const Config& b) const
		{
			return 	(this->activeDeals == b.activeDeals) && 
					(this->closedDeals == b.closedDeals) && 
					(this->users == b.users);
		}

		std::vector<Db::ActiveDeal> activeDeals;
		std::vector<Db::ClosedDeal> closedDeals;
		std::vector<Db::User> users;
	};
	
	Config current;
};

TEST_F(DealsClosing, Simple)
{
	auto storage = Db::InitStorage();
	storage.insert<Db::ActiveDeal>({ 1, 5 , 50, 1, 1, BuyType  });
	storage.insert<Db::ActiveDeal>({ 2, 5 , 60, 2, 2, BuyType  });
	storage.insert<Db::ActiveDeal>({ 3, 10, 55, 3, 3, SellType });
	storage.insert<Db::ActiveDeal>({ 4, 10, 65, 2, 4, SellType });

	Config expected;
	expected.users.push_back({1, "user1", 1234, 0   , 0 });
	expected.users.push_back({2, "user2", 1234, -275, 5 });
	expected.users.push_back({3, "user3", 1234, 275 , -5});

	expected.activeDeals.push_back({ 1, 5, 50, 1, 1, BuyType  });
	expected.activeDeals.push_back({ 3, 5, 55, 3, 3, SellType });
	expected.activeDeals.push_back({ 4, 10, 65, 2, 4, SellType });

	expected.closedDeals.push_back({ 1, 5, 55, 2, 3, 1 });

	Post::CloseDeals();

	InitResult();
	
	ASSERT_EQ(current, expected);
}

TEST_F(DealsClosing, RightBuyOrder)
{
	auto storage = Db::InitStorage();

	storage.insert<Db::ActiveDeal>({ 1, 2, 45, 1, 1, SellType });
	storage.insert<Db::ActiveDeal>({ 2, 2, 45, 2, 2, SellType });
	storage.insert<Db::ActiveDeal>({ 3, 2, 45, 1, 3, SellType });
	storage.insert<Db::ActiveDeal>({ 4, 2, 45, 2, 4, SellType });
	storage.insert<Db::ActiveDeal>({ 5, 8, 45, 3, 5, BuyType });

	Config expected;
	expected.users.push_back({1, "user1", 1234, 180 , -4 });
	expected.users.push_back({2, "user2", 1234, 180 , -4 });
	expected.users.push_back({3, "user3", 1234, -360, 8  });

	expected.closedDeals.push_back({ 1, 2, 45, 3, 1, 1 });
	expected.closedDeals.push_back({ 2, 2, 45, 3, 2, 1 });
	expected.closedDeals.push_back({ 3, 2, 45, 3, 1, 1 });
	expected.closedDeals.push_back({ 4, 2, 45, 3, 2, 1 });

	Post::CloseDeals();

	InitResult();
	//for (auto& deal : current.closedDeals)
	//{
	//	std::cout << storage.dump(deal) << std::endl;
	//}
	
	ASSERT_EQ(current, expected);
}

TEST_F(DealsClosing, RightSellOrder)
{
	auto storage = Db::InitStorage();

	storage.insert<Db::ActiveDeal>({ 1, 2, 45, 1, 1, BuyType  });
	storage.insert<Db::ActiveDeal>({ 2, 2, 45, 2, 2, BuyType  });
	storage.insert<Db::ActiveDeal>({ 3, 2, 45, 1, 3, BuyType  });
	storage.insert<Db::ActiveDeal>({ 4, 2, 45, 2, 4, BuyType  });
	storage.insert<Db::ActiveDeal>({ 5, 8, 45, 3, 5, SellType });

	Config expected;
	expected.users.push_back({1, "user1", 1234, -180 , 4 });
	expected.users.push_back({2, "user2", 1234, -180 , 4 });
	expected.users.push_back({3, "user3", 1234, 360, -8  });

	expected.closedDeals.push_back({ 1, 2, 45, 1, 3, 1 });
	expected.closedDeals.push_back({ 2, 2, 45, 2, 3, 1 });
	expected.closedDeals.push_back({ 3, 2, 45, 1, 3, 1 });
	expected.closedDeals.push_back({ 4, 2, 45, 2, 3, 1 });

	Post::CloseDeals();

	InitResult();
	
	ASSERT_EQ(current, expected);
}

TEST_F(DealsClosing, NoClosedDeals)
{
	auto storage = Db::InitStorage();

	storage.insert<Db::ActiveDeal>({ 1, 2, 45, 1, 1, BuyType  });
	storage.insert<Db::ActiveDeal>({ 2, 2, 45, 2, 2, BuyType  });
	storage.insert<Db::ActiveDeal>({ 3, 2, 45, 1, 3, BuyType  });
	storage.insert<Db::ActiveDeal>({ 4, 2, 45, 2, 4, BuyType  });
	storage.insert<Db::ActiveDeal>({ 5, 8, 46, 3, 5, SellType });

	InitResult();
	Config expected;
	expected.users = current.users;
	expected.activeDeals = current.activeDeals;
	expected.closedDeals = current.closedDeals;

	Post::CloseDeals();

	InitResult();
	
	ASSERT_EQ(current, expected);
}

int main(int argc, char* argv[]) 
{
    ::testing::InitGoogleTest(&argc, argv);
    // gtest takes ownership of the TestEnvironment ptr - we don't delete it.
    return RUN_ALL_TESTS();
}