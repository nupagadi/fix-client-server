#ifndef TRADER_H
#define TRADER_H

#include <string>
#include <vector>
#include <algorithm>

#include "Order.h"

class Trader
{
    struct Order
    {
        enum Side { buy, sell };

        std::string id;
        std::string symbol;
        Side side;
        unsigned short lot;
        unsigned long long price;
    };

public:
    Trader(const std::vector<std::string>& ini_strings){}
//    Trader(const std_string_type& id, long long balance)
//        : mId(id), mBalance(balance)
//    {}

    Trader() = delete;
    Trader(const Trader&) = delete;
    Trader& operator=(const Trader&) = delete;

private:
//    Trader& operator+=(const std::string& order_id)
//    {
//        if(std::find(mOpenedOrders.begin(), mOpenedOrders.end(), order_id) == mOpenedOrders.end())
//            mOpenedOrders.push_back(order_id);
//        return *this;
//    }

//    Trader& operator-=(const std::string& order_id)
//    {
//        auto it = std::find(mOpenedOrders.begin(), mOpenedOrders.end(), order_id);
//        if(it != mOpenedOrders.end())
//            mOpenedOrders.erase(it);
//        return *this;
//    }

    std::string mId;
    long long mBalance; // 1234567890 == 123456$ 78.90c
    std::vector<Trader::Order> mOpenedOrders; // opened orders ids
};

#endif // TRADER_H
