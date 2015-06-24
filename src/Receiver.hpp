/*
 * Receiver.hpp
 *
 *  Created on: Feb 19, 2015
 *      Author: gamberini
 */

#ifndef TRIGGERMONITOR_V0_SRC_RECEIVER_HPP_
#define TRIGGERMONITOR_V0_SRC_RECEIVER_HPP_

#include "Queue.hpp"
#include "RxSocket.hpp"

class Receiver {
	RxSocket * mRS;
	Queue<udp_t> * mPQ;

	udp_t mUDP;

public:
	Receiver(RxSocket * recSock, Queue<udp_t> * PckQue);
	virtual ~Receiver();

	void thread();
};

#endif /* TRIGGERMONITOR_V0_SRC_RECEIVER_HPP_ */
