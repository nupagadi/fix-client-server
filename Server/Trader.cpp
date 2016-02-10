#include "Trader.h"

#include <fstream>


Trader::Order::Side Trader::Convert(char ch)
{
    switch(ch)
    {
    case '1':   return Order::Side::buy;
    case '2':   return Order::Side::sell;
    default:    throw BadConvertionException();
    }
}

Trader::Trader(const std::vector<std::string> &ini_strings)
{
    if(ini_strings.empty()) throw InitionException();

    std::istringstream stream(ini_strings[0]);
    stream >> mId;
    stream >> mBalance;
    if(!stream.eof() || stream.fail()) throw InitionException();

    mOpenedOrders.reserve(ini_strings.size()-1);
    for(size_t i = 1; i < ini_strings.size(); ++i)
    {
        Trader::Order order;
        std::istringstream stream(ini_strings[i]);
        stream >> order.id;
        stream >> order.symbol;
        int en;
        stream >> en;
        order.side = static_cast<Order::Side>(en);
        stream >> order.lot;
        stream >> order.price;
        if(!stream.eof() || stream.fail()) throw InitionException();
        mOpenedOrders.push_back(std::move(order));
    }
}

Trader& Trader::operator<<(const FIX42::NewOrderSingle& order)
{
    FIX::SenderCompID senderCompID;
    FIX::Symbol symbol;
    FIX::Side side;
    FIX::OrdType ordType;
    FIX::OrderQty orderQty;
    FIX::Price price(0);

    order.getHeader().get( senderCompID );
    order.get( symbol );
    order.get( side );
    order.get( ordType );
    order.get( orderQty );
    // when NewOrderSingle comes, it's not executed yet, so there is no price
//    order.get( price );

    mOpenedOrders.emplace_back(
        Trader::Order{ GetOrderId(), symbol, Convert(side), orderQty, price }
    );

    return *this;
}


Trader& TraderSingleton::GetTrader(const std::string &id)
{
    auto trader_it = mInstance.find(id);
    if(trader_it !=  mInstance.end())
        return *trader_it->second;

    if(std::unique_ptr<Trader> trader_ptr = TryGetTraderFromDB(id))
    {
        auto pair = mInstance.emplace(id, std::move(trader_ptr));
        return *pair.first->second;
    }

    throw TraderObtainingException();
}

std::unique_ptr<Trader> TraderSingleton::TryGetTraderFromDB(const std::string &id)
{
    std::ifstream db(TRADER_TABLE_FILENAME);
    if(!db) return nullptr;

    std::string beginning, line;
    std::vector<std::string> ini_strings;

    auto task = [&]()
    {
        if(!beginning.empty())
        {
            line = beginning + ' ' + line;
            beginning.clear();
        }
        ini_strings.push_back( line );
    };
    auto skip = [](){};
    std::function<void()> func_ptr = skip;

    while(db)
    {
        if(db >> beginning && beginning == id)
            func_ptr = task;
        while(std::getline(db, line) && line != "##")
            func_ptr();
        if(!ini_strings.empty())
            return std::make_unique<Trader>(ini_strings);
    }
    return nullptr;
}
