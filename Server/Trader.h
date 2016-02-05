#ifndef TRADER_H
#define TRADER_H

#include <string>
#include <vector>

#include "Order.h"

class Trader
{
public:
    Trader() = delete;
    Trader(const Trader&) = delete;
    Trader& operator=(const Trader&) = delete;

private:
    std::string mId;
    long long mBalance;
    std::vector<std::string> mOpenedOrders; // opened orders ids
};

#endif // TRADER_H
