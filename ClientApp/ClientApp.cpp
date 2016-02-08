#include "ClientApp.h"

#include <string>
#include <quickfix/FixFields.h>
#include <quickfix/fix42/NewOrderSingle.h>
#include <quickfix/fix42/MarketDataRequest.h>
#include <quickfix/fix42/MarketDataIncrementalRefresh.h>

#include <quickfix/FileStore.h>
#include <quickfix/SocketInitiator.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/Log.h>


#include <thread>
#include <chrono>


void ClientApplication::onLogon(const FIX::SessionID& sessionID)
{
    std::cout << std::endl << "Logon - " << sessionID << std::endl;
}

void ClientApplication::onLogout(const FIX::SessionID& sessionID)
{
    std::cout << std::endl << "Logout - " << sessionID << std::endl;
}

void ClientApplication::toApp(FIX::Message &message, const FIX::SessionID &)
throw( FIX::DoNotSend )
{
    try
    {
      FIX::PossDupFlag possDupFlag;
      message.getHeader().getField( possDupFlag );
      if ( possDupFlag ) throw FIX::DoNotSend();
    }
    catch ( FIX::FieldNotFound& ) {}

    std::cout << std::endl
              << "OUT: " << message << std::endl;
}

void ClientApplication::fromApp(const FIX::Message &message, const FIX::SessionID &sessionID)
throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType )
{
    crack( message, sessionID );
    std::cout << std::endl << "IN: " << message << std::endl;
}

void ClientApplication::onMessage(const FIX42::MarketDataIncrementalRefresh &message, const FIX::SessionID &)
{
    std::cout << "MarketDataIncrementalRefresh was recieved" << std::endl;

    FIX::MDEntryPx top1, top2;
    FIX::MDEntryType type1, type2;
    FIX::Symbol symbol;

    FIX42::MarketDataIncrementalRefresh::NoMDEntries noMDEntries;
    message.getGroup(1, noMDEntries);
    noMDEntries.get(type1);
    noMDEntries.get(symbol);
    noMDEntries.get(top1);
    message.getGroup(2, noMDEntries);
    noMDEntries.get(type2);
    noMDEntries.get(symbol);
    noMDEntries.get(top2);

//    std::this_thread::sleep_for(std::chrono::seconds(5));
    try
    {
        if(type1 == '0' && type2 == '1') // bid && offer
            mTask(top2, top1);
        else if(type1 == '1' && type2 == '0')
            mTask(top1, top2);
        else throw std::logic_error("");
    }
    catch(std::bad_function_call&)
    {
        std::cerr << "Bad function call in SenderApplication::onMessage" << std::endl;
    }
    catch(std::logic_error&)
    {
        std::cerr << "Wrong entry types" << std::endl;
    }

}


Client::Client(const char *settings_file)
    : mApp(), mInitiator()
{
    mSettings = FIX::SessionSettings( settings_file );
    auto pstoreFactory = new FIX::FileStoreFactory( mSettings );
    auto plogFactory = new FIX::ScreenLogFactory ( mSettings );
    mInitiator.reset( new FIX::SocketInitiator(mApp, *pstoreFactory, mSettings, *plogFactory) );

    mInitiator.get_deleter() = SocketInitiatorDeleter{ /*psettings, */pstoreFactory, plogFactory };

    mInitiator->start();
}

Client::~Client()
{
    mInitiator->stop();
}

void Client::Send(char ord_type, char side, const std::string &symbol, unsigned lot, double price /*= 0*/)
{
    if(!ord_type || !side || !lot || symbol.empty())
    {
        std::cout << "Message was not sent" <<std::endl;
        return;
    }

    FIX::OrdType f_ord_type(ord_type);

    FIX42::NewOrderSingle newOrderSingle(
          FIX::ClOrdID( std::string(GenOrderId()) ), FIX::HandlInst( '1' ), FIX::Symbol(symbol),
          FIX::Side( side ), FIX::TransactTime(), f_ord_type
    );

    newOrderSingle.set( FIX::OrderQty(lot) );
    newOrderSingle.set( FIX::TimeInForce( FIX::TimeInForce_DAY ) );
    if(f_ord_type == FIX::OrdType_LIMIT)
    {
        newOrderSingle.set( FIX::Price(price) );
        // treating quickfix double conversion bug
        auto price_str = std::to_string(price);
        std::replace(price_str.begin(), price_str.end(), ',', '.');
        newOrderSingle.setField(44, price_str);
    }

    // getting sender and target from settings
    if(mSettings.size() == 0)   return;
    auto& dic = mSettings.get(*mSettings.getSessions().begin());
    std::string sender = dic.getString("SenderCompID");
    std::string target = dic.getString("TargetCompID");
    auto& header = newOrderSingle.getHeader();
    header.setField( FIX::SenderCompID(sender) );
    header.setField( FIX::TargetCompID(target) );

    std::ofstream os("client_mes");
    os << newOrderSingle.toString();

    if(
            FIX::Session::sendToTarget( newOrderSingle )
    )
        std::cout << "NewOrderSingle was sent" << std::endl;

}

void Client::AskForMarketData(std::function<void(double, double)>&& task)
{
    FIX42::MarketDataRequest marketDataRequest;
    marketDataRequest.set(FIX::MDReqID(GenOrderId()));
    marketDataRequest.set(FIX::SubscriptionRequestType('1'));
    marketDataRequest.set(FIX::MarketDepth(1));
    FIX42::MarketDataRequest::NoMDEntryTypes noMDEntryTypes;
    FIX::MDEntryType mdEntryType_bid('0'), mdEntryType_offer('1');
    noMDEntryTypes.setField(mdEntryType_bid);
    marketDataRequest.addGroup(noMDEntryTypes);
    noMDEntryTypes.setField(mdEntryType_offer);
    marketDataRequest.addGroup(noMDEntryTypes);

    FIX42::MarketDataRequest::NoRelatedSym noRelatedSym;
    noRelatedSym.setField(FIX::Symbol("EURUSD"));
    marketDataRequest.addGroup(noRelatedSym);

    // getting sender and target from settings
    if(mSettings.size() == 0)   return;
    auto& dic = mSettings.get(*mSettings.getSessions().begin());
    std::string sender = dic.getString("SenderCompID");
    std::string target = dic.getString("TargetCompID");
    auto& header = marketDataRequest.getHeader();
    header.setField( FIX::SenderCompID(sender) );
    header.setField( FIX::TargetCompID(target) );

//    std::this_thread::sleep_for(std::chrono::seconds(2));

    FIX::Session::sendToTarget( marketDataRequest );
    std::cout << "MarketDataRequest was sent" << std::endl;

    mApp.SetTask(std::move(task));
}

template <class T>
void SocketInitiatorDeleter::operator()(T *p)
{
//    delete a;
//    delete b;
//    delete c;
    delete p;
}
