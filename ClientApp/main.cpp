#define BOOST_TEST_MODULE Client Application Test
#include <boost/test/included/unit_test.hpp>

#include "ClientApp.h"

//BOOST_AUTO_TEST_CASE(first_test)
//{
//  int i = 1;
//  BOOST_TEST(i);
//  BOOST_TEST(i == 2);
//}

BOOST_AUTO_TEST_CASE(send_test)
{
    Client client("res/settings");
}

