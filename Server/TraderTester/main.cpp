#define BOOST_TEST_MODULE Trader Tests
#include <boost/test/included/unit_test.hpp>

#define private public

#include "../Trader.h"


BOOST_AUTO_TEST_CASE(creation_test)
{
    std::vector<std::string> vec{"123", "234", "345"};
    Trader t1("11", 10000000, vec);
    Trader t2("12", 9990000, std::move(vec));

    BOOST_TEST(t1.mId == "11");
    BOOST_TEST(t2.mId == "12");
    BOOST_TEST(t1.mBalance == 10000000);
    BOOST_TEST(t2.mBalance == 9990000);
    BOOST_TEST(t1.mOpenedOrders == t2.mOpenedOrders);
}

BOOST_AUTO_TEST_CASE(new_test)
{
    std::vector<std::string> vec{"123", "234", "345"};
    auto pt = new Trader("11", 10000, vec);
    BOOST_TEST(pt);
    BOOST_TEST(pt->mId == "11");
    BOOST_TEST(pt->mOpenedOrders.size() == 3);
    BOOST_TEST(pt->mOpenedOrders[1] == "234");
}

BOOST_AUTO_TEST_CASE(operations_test)
{
    std::vector<std::string> vec{"123", "234", "345"};
    Trader t1("11", 10000000, vec);

    t1 += "456";
    BOOST_TEST(t1.mOpenedOrders.size() == 4);
    BOOST_TEST(t1.mOpenedOrders[3] == "456");
    t1 += "456";
    BOOST_TEST(t1.mOpenedOrders.size() == 4);
    BOOST_TEST(t1.mOpenedOrders[3] == "456");

    t1 -= "123";
    BOOST_TEST(t1.mOpenedOrders.size() == 3);
    BOOST_TEST(t1.mOpenedOrders[1] == "345");
    t1 -= "123";
    BOOST_TEST(t1.mOpenedOrders.size() == 3);
    BOOST_TEST(t1.mOpenedOrders[1] == "345");

    t1 += "123";
    t1 += "567";
    t1 -= "456";
    BOOST_TEST(t1.mOpenedOrders.size() == 4);
    std::vector<std::string> vec2{"234", "345", "123", "567"};
    BOOST_TEST(t1.mOpenedOrders == vec2);
}

