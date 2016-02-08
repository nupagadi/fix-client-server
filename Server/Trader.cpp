#include "Trader.h"



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
    if(!stream.eof()) throw InitionException();

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
        if(!stream.eof()) throw InitionException();
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
//    order.get( price );

    mOpenedOrders.emplace_back(
        Trader::Order{ GetOrderId(), symbol, Convert(side), orderQty, price }
    );

    return *this;
}
