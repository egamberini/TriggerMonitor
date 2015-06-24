/*
 * RxSocket.hpp
 *
 *  Created on: Feb 19, 2015
 *      Author: gamberini
 */

#ifndef TRIGGERMONITOR_V0_SRC_RXSOCKET_HPP_
#define TRIGGERMONITOR_V0_SRC_RXSOCKET_HPP_

#include <boost/asio.hpp>

class RxSocket {
	std::string mLocIP;
	std::string mRemIP;
	uint16_t mLocPort, mRemPort;

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket* socket;
	boost::asio::ip::udp::endpoint local_endpoint;
	boost::asio::ip::udp::endpoint remote_endpoint;

public:
	RxSocket(const std::string localIP, const std::string remoteIP, const uint16_t localPort, const uint16_t remotePort);
	virtual ~RxSocket();

	int ReceiveFrom(unsigned char * receivedPck);
};

#endif /* TRIGGERMONITOR_V0_SRC_RXSOCKET_HPP_ */
