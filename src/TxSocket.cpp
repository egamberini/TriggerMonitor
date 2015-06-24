/*
 * TxSocket.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: gamberini
 */

#include <iostream>

#include "TxSocket.hpp"

#include "Config.hpp"

TxSocket::TxSocket(const std::string localIP, std::vector<std::string>& remoteIPs, const uint16_t localPort, const uint16_t remotePort)
: mLocIP(localIP),
  mRemIPs(remoteIPs),
  mLocPort(localPort),
  mRemPort(remotePort),
  rr(0)
{
	local_endpoint = boost::asio::ip::udp::endpoint(
			boost::asio::ip::address::from_string(mLocIP), mLocPort);
	socket = new boost::asio::ip::udp::socket(io_service, local_endpoint);
	remote_endpoint = new boost::asio::ip::udp::endpoint[mRemIPs.size()];
	for(unsigned int i=0; i<mRemIPs.size(); i++) {
		remote_endpoint[i] = boost::asio::ip::udp::endpoint(
				boost::asio::ip::address::from_string(mRemIPs[i]), mRemPort);
	}
}

TxSocket::~TxSocket() {
	// TODO Auto-generated destructor stub
}

void TxSocket::SendTo(unsigned char * txPck, int txPckLen) {
//	std::cout<<"[DBG] TxSocket::SendTo -> remote endpoint: "<<mRemIPs[rr]<<std::endl;
	socket->send_to(boost::asio::buffer(txPck, txPckLen), remote_endpoint[rr]);
	rr++;
	if(rr == mRemIPs.size()) rr = 0;
}
