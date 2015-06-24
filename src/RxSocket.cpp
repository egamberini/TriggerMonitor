/*
 * RxSocket.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: gamberini
 */

#include "RxSocket.hpp"

#include "Config.hpp"

RxSocket::RxSocket(const std::string localIP, const std::string remoteIP, const uint16_t localPort, const uint16_t remotePort)
: mLocIP(localIP),
  mRemIP(remoteIP),
  mLocPort(localPort),
  mRemPort(remotePort)
{
	local_endpoint = boost::asio::ip::udp::endpoint(
			boost::asio::ip::address::from_string(mLocIP), mLocPort);
	socket = new boost::asio::ip::udp::socket(io_service, local_endpoint);
	remote_endpoint = boost::asio::ip::udp::endpoint(
			boost::asio::ip::address::from_string(mRemIP), mRemPort);
}

RxSocket::~RxSocket() {
	// TODO Auto-generated destructor stub
}

int RxSocket::ReceiveFrom(unsigned char * receivedPck) {
	return socket->receive_from(boost::asio::buffer(receivedPck, udpPayload), remote_endpoint);
}
