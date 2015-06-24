/*
 * Receiver.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: gamberini
 */

#include "Receiver.hpp"

Receiver::Receiver(RxSocket * recSock, Queue<udp_t> * PckQue)
: mRS(recSock),
  mPQ(PckQue),
  mUDP()
{
	// TODO Auto-generated constructor stub

}

Receiver::~Receiver() {
	// TODO Auto-generated destructor stub
}

void Receiver::thread() {
	int i=0;
	while(1) {
//		std::cout<<"Receiver "<<i<<std::endl;
		mUDP.len = mRS->ReceiveFrom(mUDP.buf);
		mPQ->Enque(mUDP);
		i++;
	}
}
