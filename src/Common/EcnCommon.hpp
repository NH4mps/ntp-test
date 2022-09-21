#pragma once

#include <boost/asio.hpp>

const static short g_serverPort = 5555;
const static boost::asio::ip::address g_serverAddress = boost::asio::ip::address::from_string("127.0.0.1");