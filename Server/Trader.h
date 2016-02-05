#ifndef TRADER_H
#define TRADER_H

#include <string>
#include <vector>
#include <algorithm>

#include "Order.h"

class Trader
{
public:
    template <class std_string_type, class std_vector_type>
    Trader(std_string_type&& id, long long balance, std_vector_type&& orders_ids)
        : mId(id), mBalance(balance), mOpenedOrders(orders_ids)
    {}

    Trader() = delete;
    Trader(const Trader&) = delete;
    Trader& operator=(const Trader&) = delete;

private:
    Trader& operator+=(const std::string& order_id)
    {
        if(std::find(mOpenedOrders.begin(), mOpenedOrders.end(), order_id) == mOpenedOrders.end())
            mOpenedOrders.push_back(order_id);
        return *this;
    }

    Trader& operator-=(const std::string& order_id)
    {
        auto it = std::find(mOpenedOrders.begin(), mOpenedOrders.end(), order_id);
        if(it != mOpenedOrders.end())
            mOpenedOrders.erase(it);
        return *this;
    }

    std::string mId;
    long long mBalance; // 123456.7890
    std::vector<std::string> mOpenedOrders; // opened orders ids
};

#endif // TRADER_H
