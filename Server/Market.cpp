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

#ifdef _MSC_VER
#pragma warning( disable : 4786 )
#endif

#include "Market.h"

#include "Trader.h"

#include <iostream>
#include <algorithm>

bool Market::insert( const Order& order )
{
  if ( order.getSide() == Order::buy )
      m_bidOrders.insert( BidOrders::value_type(order, order) );
  else
      m_askOrders.insert( AskOrders::value_type(order, order) );
  return true;
}

void Market::erase( const Order& order )
{
  std::string id = order.getClientID();
  if ( order.getSide() == Order::buy )
  {
    BidOrders::iterator i;
    for ( i = m_bidOrders.begin(); i != m_bidOrders.end(); ++i )
      if ( i->second.getClientID() == id )
      {
        m_bidOrders.erase( i );
        return ;
      }
  }
  else if ( order.getSide() == Order::sell )
  {
    AskOrders::iterator i;
    for ( i = m_askOrders.begin(); i != m_askOrders.end(); ++i )
      if ( i->second.getClientID() == id )
      {
        m_askOrders.erase( i );
        return ;
      }
  }
}

void Market::GetTops(BidOrders::iterator& iBid, AskOrders::iterator& iAsk)
{
    iBid = m_bidOrders.begin();
    iAsk = m_askOrders.begin();
    if(iBid->second.getType() == Order::market && iAsk->second.getType() == Order::market)
        iAsk = std::find_if(iAsk, m_askOrders.end(),
            [](AskOrders::value_type& el)
            {
                return el.second.getType() != Order::market;
            }
                );
}

void Market::GetTops(unsigned long& bid, unsigned long& ask)
{
    BidOrders::iterator iBid;    AskOrders::iterator iAsk;
    GetTops(iBid, iAsk);
    double b = iBid->second.getPrice(), a = iAsk->second.getPrice();
    bid = static_cast<int>(b * PRICE_ACCURACY * PRICE_COMMA);
    ask = static_cast<int>(a * PRICE_ACCURACY * PRICE_COMMA);
}

bool Market::match( std::queue < Order > & orders )
{
  while ( true )
  {
    if ( !m_bidOrders.size() || !m_askOrders.size() )
      return orders.size() != 0;

    BidOrders::iterator iBid;    AskOrders::iterator iAsk;
    GetTops(iBid, iAsk);
    Order& bid = iBid->second;
    Order& ask = iAsk->second;

    if ( bid.getType() == Order::market || ask.getType() == Order::market
         || bid.getPrice() >= ask.getPrice() )
    {

      match(bid, ask);
      orders.push( bid );
      orders.push( ask );

      if ( bid.isClosed() ) m_bidOrders.erase( iBid );
      if ( ask.isClosed() ) m_askOrders.erase( iAsk );
    }
    else
      return orders.size() != 0;
  }
}

Order& Market::find( Order::Side side, std::string id/* = ""*/ )
{
  if ( side == Order::buy )
  {
      if(id.empty())
          return  m_bidOrders.begin()->second;

    BidOrders::iterator i;
    for ( i = m_bidOrders.begin(); i != m_bidOrders.end(); ++i )
      if ( i->second.getClientID() == id ) return i->second;
  }
  else if ( side == Order::sell )
  {
      if(id.empty())
          return  m_askOrders.begin()->second;

    AskOrders::iterator i;
    for ( i = m_askOrders.begin(); i != m_askOrders.end(); ++i )
      if ( i->second.getClientID() == id ) return i->second;
  }
  throw std::exception();
}

void Market::match( Order& bid, Order& ask )
{
  double price = ask.getType() != Order::market ? ask.getPrice() : bid.getPrice();
  long quantity = 0;

  if ( bid.getOpenQuantity() > ask.getOpenQuantity() )
    quantity = ask.getOpenQuantity();
  else
    quantity = bid.getOpenQuantity();

  bid.execute( price, quantity );
  ask.execute( price, quantity );
}

void Market::display() const
{
  BidOrders::const_iterator iBid;
  AskOrders::const_iterator iAsk;

  std::cout << "BIDS:" << std::endl;
  std::cout << "-----" << std::endl << std::endl;
  for ( iBid = m_bidOrders.begin(); iBid != m_bidOrders.end(); ++iBid )
    std::cout << iBid->second << std::endl;

  std::cout << std::endl << std::endl;

  std::cout << "ASKS:" << std::endl;
  std::cout << "-----" << std::endl << std::endl;
  for ( iAsk = m_askOrders.begin(); iAsk != m_askOrders.end(); ++iAsk )
    std::cout << iAsk->second << std::endl;
}
