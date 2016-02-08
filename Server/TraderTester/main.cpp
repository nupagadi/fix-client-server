#define BOOST_TEST_MODULE Trader Tests
#include <boost/test/included/unit_test.hpp>

#include "quickfix/fix42/NewOrderSingle.h"

#define private public
#include "../Trader.h"


BOOST_AUTO_TEST_CASE(hollow_inition_test1)
{
    std::vector<std::string> ini_string;
    ini_string.push_back("123 10000000"); // id:123, balance:1k$
    BOOST_REQUIRE_NO_THROW(Trader trader1(ini_string));
    Trader trader1(ini_string);
    BOOST_TEST(trader1.mId == "123");
    BOOST_TEST(trader1.mBalance == 10000000);
    BOOST_TEST(trader1.mOpenedOrders.empty());
}


BOOST_AUTO_TEST_CASE(hollow_inition_test2)
{
    std::vector<std::string> ini_string;
    ini_string.push_back("myid 1k_bucks"); // id:123, balance:1k$
    BOOST_CHECK_THROW(Trader trader1(ini_string), Trader::InitionException);
    ini_string[0] = "ididid ";
    BOOST_CHECK_THROW(Trader trader1(ini_string), Trader::InitionException);
    ini_string[0] = "ididid 123123 213";
    BOOST_CHECK_THROW(Trader trader1(ini_string), Trader::InitionException);
}

BOOST_AUTO_TEST_CASE(inition_test1)
{
    std::vector<std::string> ini_string;
    ini_string.push_back("123 10000000"); // id:123, balance:1k$
    ini_string.push_back("1001 EURUSD 0 1 11234"); // order: id:1001, symbol, side, lot, price:1.1234
    BOOST_REQUIRE_NO_THROW(Trader trader(ini_string));
    Trader trader1(ini_string);
    BOOST_TEST_REQUIRE(trader1.mOpenedOrders.size() == 1);
    auto& order_ref = trader1.mOpenedOrders[0];
    BOOST_TEST(order_ref.id == 1001);
    BOOST_TEST(order_ref.symbol == "EURUSD");
    BOOST_TEST(order_ref.lot == 1);
    BOOST_TEST(order_ref.side == Trader::Order::Side::buy);
    BOOST_TEST(order_ref.price == 11234);

    ini_string.push_back("1002 EURUSD 1 2 1223");
    BOOST_REQUIRE_NO_THROW(Trader trader2(ini_string));
    Trader trader2(ini_string);
    BOOST_TEST_REQUIRE(trader2.mOpenedOrders.size() == 2);
    auto& order_ref2 = trader2.mOpenedOrders[1];
    BOOST_TEST(order_ref2.id == 1002);
    BOOST_TEST(order_ref2.symbol == "EURUSD");
    BOOST_TEST(order_ref2.lot == 2);
    BOOST_TEST(order_ref2.side == Trader::Order::Side::sell);
    BOOST_TEST(order_ref2.price == 1223);
}

BOOST_AUTO_TEST_CASE(inition_test2)
{
    std::vector<std::string> ini_string;
    ini_string.push_back("123 10000000"); // id:123, balance:1k$
    ini_string.push_back("1001 EURUSD 0 1 11234"); // order: id:1001, symbol, side, lot, price:1.1234
    BOOST_REQUIRE_NO_THROW(Trader trader(ini_string));
    Trader trader1(ini_string);
    BOOST_TEST_REQUIRE(trader1.mOpenedOrders.size() == 1);
    auto& order_ref = trader1.mOpenedOrders[0];
    BOOST_TEST(order_ref.id == 1001);
    BOOST_TEST(order_ref.symbol == "EURUSD");
    BOOST_TEST(order_ref.lot == 1);
    BOOST_TEST(order_ref.side == Trader::Order::Side::buy);
    BOOST_TEST(order_ref.price == 11234);

}

BOOST_AUTO_TEST_CASE(order_add_test)
{
    std::ifstream order1_file("res/order1");
    BOOST_TEST(order1_file);
    std::string order_str;
    order1_file >> order_str;
    BOOST_TEST(!order_str.empty());
    FIX42::NewOrderSingle order1;
    order1.setString(order_str);

    std::vector<std::string> ini_string;
    ini_string.push_back("123 10000000"); // id:123, balance:1k$

    Trader trader1(ini_string);
    trader1 << order1;
    BOOST_TEST(trader1.mId == "123");
    BOOST_TEST(trader1.mBalance == 10000000);
    BOOST_TEST(trader1.mOpenedOrders.size() == 1);
    auto& order_ref = trader1.mOpenedOrders[0];
    BOOST_TEST(order_ref.id == 1);
    BOOST_TEST(order_ref.symbol == "EURUSD");
    BOOST_TEST(order_ref.side == Trader::Order::Side::sell);
    BOOST_TEST(order_ref.lot == 1);
//    BOOST_TEST(order_ref.price == );

}

//BOOST_AUTO_TEST_CASE(creation_test)
//{
//    std::vector<std::string> vec{"123", "234", "345"};
//    Trader t1("11", 10000000, vec);
//    Trader t2("12", 9990000, std::move(vec));

//    BOOST_TEST(t1.mId == "11");
//    BOOST_TEST(t2.mId == "12");
//    BOOST_TEST(t1.mBalance == 10000000);
//    BOOST_TEST(t2.mBalance == 9990000);
//    BOOST_TEST(t1.mOpenedOrders == t2.mOpenedOrders);
//}

//BOOST_AUTO_TEST_CASE(new_test)
//{
//    std::vector<std::string> vec{"123", "234", "345"};
//    auto pt = new Trader("11", 10000, vec);
//    BOOST_TEST(pt);
//    BOOST_TEST(pt->mId == "11");
//    BOOST_TEST(pt->mOpenedOrders.size() == 3);
//    BOOST_TEST(pt->mOpenedOrders[1] == "234");
//}

//BOOST_AUTO_TEST_CASE(operations_test)
//{
//    std::vector<std::string> vec{"123", "234", "345"};
//    Trader t1("11", 10000000, vec);

//    t1 += "456";
//    BOOST_TEST(t1.mOpenedOrders.size() == 4);
//    BOOST_TEST(t1.mOpenedOrders[3] == "456");
//    t1 += "456";
//    BOOST_TEST(t1.mOpenedOrders.size() == 4);
//    BOOST_TEST(t1.mOpenedOrders[3] == "456");

//    t1 -= "123";
//    BOOST_TEST(t1.mOpenedOrders.size() == 3);
//    BOOST_TEST(t1.mOpenedOrders[1] == "345");
//    t1 -= "123";
//    BOOST_TEST(t1.mOpenedOrders.size() == 3);
//    BOOST_TEST(t1.mOpenedOrders[1] == "345");

//    t1 += "123";
//    t1 += "567";
//    t1 -= "456";
//    BOOST_TEST(t1.mOpenedOrders.size() == 4);
//    std::vector<std::string> vec2{"234", "345", "123", "567"};
//    BOOST_TEST(t1.mOpenedOrders == vec2);
//}

