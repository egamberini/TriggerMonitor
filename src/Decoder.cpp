/*
 * Decoder.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: gamberini
 *
 * Decoder for CPU events
 *
 */

#include <netinet/in.h>

#include "Decoder.hpp"
#include "EobDataHandler.hpp"

Decoder::Decoder(Queue<udp_t> * PckQue, Queue<CPU_EVENT> * cpuEvntQue, EobDataHandler * eobDatHand)
: mPQ(PckQue),
  mCEQ(cpuEvntQue),
  mEDH(eobDatHand),
  type()
{
	mUDP = new udp_t;
	mLH = new LPE_HDR;
	mEH = new EVNT_HDR;
	mEFC = new EVNT_FROM_CPU;
	mCE = new CPU_EVENT;
}

Decoder::~Decoder() {
	// TODO Auto-generated destructor stub
}

void Decoder::thread() {
	int i=0;
	while(1) {
//		std::cout<<"Decoder "<<i<<std::endl;
//		usleep(1000);
//		std::cout<<"Deque CPU PACKET "<<i<<std::endl;
		memcpy(mUDP, mPQ->Deque(), sizeof(udp_t));
		mPQ->Pop();

		mLH = reinterpret_cast<LPE_HDR *>(mUDP->buf);

		for(int j=0; j<mLH->numEvents; j++) {
			mEH = reinterpret_cast<EVNT_HDR *>(mUDP->buf+8+j*24);
			mCE->timeStamp = mEH->eventTimestamp;
			mCE->type = mEH->eventFlags;


			if(mCE->type == 35) {
//				std::cout<<"[BDG] Decoder: CPU->EOB"<<std::endl;
				mEDH->setNumOfAnalyzedPrim(mUDP->buf+8+8);
				this->Print();
				mCEQ->Enque(*mCE);
				break;
			}

			mEFC = reinterpret_cast<EVNT_FROM_CPU *>(mUDP->buf+8+8+j*24);
			mCE->fineTime = mEFC->fineTime;
			mCE->pIdA = mEFC->pIdA;
			mCE->pIdB = mEFC->pIdB;
			mCE->pIdC = mEFC->pIdC;
			mCE->pIdD = mEFC->pIdD;
			mCE->pIdE = mEFC->pIdE;
			mCE->pIdF = mEFC->pIdF;
			mCE->pIdG = mEFC->pIdG;


			type[mEH->eventFlags]++;

			// DEBUG
//			std::cout<<"TimeStamp: 0x"<<std::hex<<(int)mCE->timeStamp<<std::endl;
//			std::cout<<"Type: 0x"<<std::hex<<(int)mCE->type<<std::endl;
//			std::cout<<"fineTime: 0x"<<std::hex<<(int)mCE->fineTime<<std::endl;
//			std::cout<<"PIdA: 0x"<<std::hex<<(int)mCE->pIdA<<std::endl;

//			std::cout<<"Enque CPU EVENT"<<std::endl;
			mCEQ->Enque(*mCE);
		}
		i++;
	}
}

void Decoder::Print() {
	std::cout<<"DecoderCPU:"<<std::endl;
	std::cout<<"Trigger mask 1:	"<<std::dec<<type[1]<<std::endl;
	std::cout<<"Trigger mask 2:	"<<std::dec<<type[2]<<std::endl;
	std::cout<<"Trigger mask 3:	"<<std::dec<<type[3]<<std::endl;
	std::cout<<"Trigger mask 4:	"<<std::dec<<type[4]<<std::endl;
	std::cout<<"Trigger mask 5:	"<<std::dec<<type[5]<<std::endl;
	std::cout<<"Trigger mask 6:	"<<std::dec<<type[6]<<std::endl;
	std::cout<<"Trigger mask 7:	"<<std::dec<<type[7]<<std::endl;
	std::cout<<"Trigger mask 8:	"<<std::dec<<type[8]<<std::endl;
	memset(type, 0, sizeof(type));
}
