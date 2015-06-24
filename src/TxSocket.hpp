/*
 * TxSocket.hpp
 *
 *  Created on: Feb 19, 2015
 *      Author: gamberini
 */

#ifndef TRIGGERMONITOR_V0_SRC_TxSOCKET_HPP_
#define TRIGGERMONITOR_V0_SRC_TxSOCKET_HPP_

#include <boost/asio.hpp>

class TxSocket {
	const std::string mLocIP;
	const std::vector<std::string> mRemIPs;
	const uint16_t mLocPort, mRemPort;

	unsigned int rr;

	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket * socket;
	boost::asio::ip::udp::endpoint local_endpoint;
	boost::asio::ip::udp::endpoint * remote_endpoint;

public:
	TxSocket(const std::string localIP, std::vector<std::string>& remoteIPs, const uint16_t localPort, const uint16_t remotePort);
	virtual ~TxSocket();

	void SendTo(unsigned char * txPck, int txPckLen);

	void ResetRoundRobin() {
		rr = 0;
	}
};

#endif /* TRIGGERMONITOR_V0_SRC_TxSOCKET_HPP_ */
