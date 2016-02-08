#ifndef TRADER_H
#define TRADER_H

#include <string>
#include <vector>
#include <algorithm>

#include <quickfix/Message.h>
#include <quickfix/fix42/Message.h>
#include "quickfix/fix42/NewOrderSingle.h"

#include "Order.h"

class Trader
{
public:

    struct Order
    {
        enum Side { buy, sell };

        unsigned long long id;
        std::string symbol;
        Side side;
        unsigned short lot;
        unsigned long long price;
    };

    static Order::Side Convert(char ch);

    Trader(const std::vector<std::string>& ini_strings);

    class InitionException {};
    class BadConvertionException {};

    Trader() = delete;
    Trader(const Trader&) = delete;
    Trader& operator=(const Trader&) = delete;

    // add an executed order
    Trader& operator<<(const FIX42::NewOrderSingle& order);
    // remove executed order
    Trader& operator>>(unsigned long long order_id);

private:
    static unsigned long long GetOrderId()
    {
        static unsigned long long i = 0;
        return ++i;
    }

    std::string mId;
    long long mBalance; // 1234567890 == 123456$ 78.90c
    std::vector<Trader::Order> mOpenedOrders; // opened orders
};

#endif // TRADER_H
