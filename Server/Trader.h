#ifndef TRADER_H
#define TRADER_H

#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <map>

class Order;

constexpr unsigned long PRICE_COMMA = 10000;
constexpr unsigned long long PRICE_ACCURACY = 1000*1000*1000;
constexpr unsigned long BUCKS_PER_LOT = 100*1000;

class Trader
{
public:

    struct Order
    {
        enum Side { buy, sell };

        unsigned long long id;
        std::string symbol;
        Side side;
        unsigned short lot;     // 655.35 - max 655 lots
        unsigned long long price;   // 1844674.4073709551615
    };

    static Order::Side Convert(char ch);

    Trader(const std::vector<std::string>& ini_strings);

    class InitionException {};
    class BadConvertionException {};

    Trader() = delete;
    Trader(const Trader&) = delete;
    Trader& operator=(const Trader&) = delete;

    // add an executed order
    Trader& operator<<(const ::Order& order);
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


constexpr char TRADER_TABLE_FILENAME[] = "res/trader_table";
class TraderSingleton
{

public:
    static TraderSingleton* Instance()
    {
        static TraderSingleton traders;
        return &traders;
    }

    // throws:
    // TraderSingleton::TraderObtainingException - no such a trader
    // Trader::InitionException - probably DB is corrupted
    Trader& GetTrader(const std::string& id);

//    TraderSingleton& operator<<(const Order& order);


    class TraderObtainingException {};

    TraderSingleton(const TraderSingleton&) = delete;
    TraderSingleton& operator=(const TraderSingleton&) = delete;

private:
    TraderSingleton() {}

//    std::unique_ptr<Trader>& TryGetTraderFromOnline(const std::string& id);

    std::unique_ptr<Trader> TryGetTraderFromDB(const std::string& id);


    std::map<std::string, std::unique_ptr<Trader>> mInstance;
};

#endif // TRADER_H
