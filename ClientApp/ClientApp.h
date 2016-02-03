#ifndef BUYORDERSENDER_H
#define BUYORDERSENDER_H

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>

#include <functional>

class  ClientApplication :
        public FIX::Application,
        public FIX::MessageCracker
{
public:
    ClientApplication() {}

    virtual ~ClientApplication() {}
    /// Notification of a session begin created
    virtual void onCreate( const FIX::SessionID& ) {}
    /// Notification of a session successfully logging on
    virtual void onLogon( const FIX::SessionID& );
    /// Notification of a session logging off or disconnecting
    virtual void onLogout( const FIX::SessionID& );
    /// Notification of admin message being sent to target
    virtual void toAdmin( FIX::Message&, const FIX::SessionID& ) {}
    /// Notification of app message being sent to target
    virtual void toApp( FIX::Message&, const FIX::SessionID& )
    throw( FIX::DoNotSend );
    /// Notification of admin message being received from target
    virtual void fromAdmin( const FIX::Message&, const FIX::SessionID& )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}
    /// Notification of app message being received from target
    virtual void fromApp( const FIX::Message&, const FIX::SessionID& )
    throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

    void onMessage( const FIX42::ExecutionReport&, const FIX::SessionID& ) {}
    void onMessage( const FIX42::OrderCancelReject&, const FIX::SessionID& ) {}
//    void onMessage( const FIX42::MarketDataRequest&, const FIX::SessionID& ) {}
    void onMessage( const FIX42::MarketDataIncrementalRefresh&, const FIX::SessionID& );

    void SetTask(std::function<void(double, double)>&& task) { mTask = std::move(task); }
private:
    // market data future
    std::function<void(double, double)> mTask;
};

namespace FIX {
class SocketInitiator;
//class SessionSettings;
class FileStoreFactory;
class ScreenLogFactory;
}

struct SocketInitiatorDeleter {
//    FIX::SessionSettings* a;
    FIX::FileStoreFactory* b;
    FIX::ScreenLogFactory* c;
  template <class T>
  void operator()(T* p);
};

class Client
{
public:
    Client(const char* settings);
    ~Client();

    void Send(char ord_type, char side, const std::string& symbol, unsigned lot, double price = 0);
    void AskForMarketData(std::function<void(double, double)>&& task);

private:
    ClientApplication mApp;
    FIX::SessionSettings mSettings;
    std::unique_ptr<FIX::SocketInitiator, SocketInitiatorDeleter> mInitiator;
};

inline std::string GenOrderId()
{
    static uint_least32_t i = 0;
    return std::to_string(++i);
}

#endif // BUYORDERSENDER_H
