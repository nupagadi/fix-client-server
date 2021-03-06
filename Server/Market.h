/* -*- C++ -*- */

/****************************************************************************
** Copyright (c) 2001-2014
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifndef ORDERMATCH_MARKET_H
#define ORDERMATCH_MARKET_H

#include "Order.h"
#include <map>
#include <queue>
#include <string>
#include <functional>

class Market
{
    typedef std::multimap < OrderKey, Order, decltype(IsGreater)*> BidOrders;
    typedef std::multimap < OrderKey, Order, decltype(IsLess)*> AskOrders;

public:
    Market() : m_bidOrders(IsGreater), m_askOrders(IsLess) {}

    bool insert( const Order& order );
    void erase( const Order& order );
    Order& find( Order::Side side, std::string id = "" );
    bool match( std::queue < Order > & );
    void display() const;

    // Get top bid and price; only one of <iBid> and <iAsk> is Order::market
    void GetTops(BidOrders::iterator& iBid, AskOrders::iterator& iAsk);

private:
    // assume only one of <bid> and <ask> is Order::market
    void match( Order& bid, Order& ask );

    std::queue < Order > m_orderUpdates;
    BidOrders m_bidOrders;
    AskOrders m_askOrders;
};

#endif
