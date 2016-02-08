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
