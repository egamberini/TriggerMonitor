/*
 * DecoderFpgaEvent.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: gamberini
 *
 * Decoder for FPGA events
 *
 */

#include <netinet/in.h>

#include "DecoderFpgaEvent.hpp"


DecoderFpgaEvent::DecoderFpgaEvent(Queue<udp_t> * PckQue, Queue<FPGA_EVENT> * fpgaEvntQue)
: mPQ(PckQue),
  mFEQ(fpgaEvntQue),
  type()
{
	mUDP = new udp_t;
	mFE = new FPGA_EVENT;
}

DecoderFpgaEvent::~DecoderFpgaEvent() {
	// TODO Auto-generated destructor stub
}

void DecoderFpgaEvent::thread() {
	int i=0;
	while(1) {
//		usleep(10000);
//		std::cout<<"Deque FPGA PACKET: "<<i<<std::endl;

		memcpy(mUDP, mPQ->Deque(), sizeof(udp_t));
		mPQ->Pop();

//		std::cout<<"FpgaEvent:	0x"<<i<<std::endl;

		mFE = reinterpret_cast<FPGA_EVENT *>(mUDP->buf+16);

//		printf("%02x %02x %02x %02x\n", mUDP->buf[16], mUDP->buf[16+1], mUDP->buf[16+2], mUDP->buf[16+3]);
//		printf("%02x %02x %02x %02x\n", mUDP->buf[20], mUDP->buf[20+1], mUDP->buf[20+2], mUDP->buf[20+3]);

		// skip Sob and Eob "pre-packets"
		if(mFE->triggerWord == 0 && mFE->triggerNumber == 0 && mFE->timeStamp == 0) {
//			std::cout<<"[DBG] pre-packet"<<std::endl;
			//capture the EoB signal packet!!!
			if(mUDP->buf[0] == 50) {
//				std::cout<<"[DBG] EoB signal packet"<<std::endl;
				//search for EoB/TTC packet
				do {
					memcpy(mUDP, mPQ->Deque(), sizeof(udp_t));
					mPQ->Pop();
					mFE = reinterpret_cast<FPGA_EVENT *>(mUDP->buf+16);
//		            std::cout<<"triggerNumber:    0x"<<std::hex<<(int)mFE->triggerNumber<<std::endl;
//		            std::cout<<"triggerWord:      0x"<<std::hex<<(int)mFE->triggerWord<<std::endl;
//		            std::cout<<"timeStamp:        0x"<<std::hex<<(int)mFE->timeStamp<<std::endl;
				} while(mFE->triggerWord != 35);
//				std::cout<<"[DBG] Found EoB/TTC packet"<<std::endl;
			}
			else {
//				std::cout<<"[DBG] SoB signal packet"<<std::endl;
				continue;
			}
		}

		// manage Sob packet
		if(mFE->triggerWord == 34) {
			i=mFE->triggerNumber;
			std::cout<<"\n==================================="<<std::endl;
			std::cout<<"DecoderFPGA->SOB"<<std::endl;
            std::cout<<"triggerNumber:    0x"<<std::hex<<(int)mFE->triggerNumber<<std::endl;
            std::cout<<"triggerWord:      0x"<<std::hex<<(int)mFE->triggerWord<<std::endl;
            std::cout<<"timeStamp:        0x"<<std::hex<<(int)mFE->timeStamp<<std::endl;
            i++;
//            std::cout<<"Enque SoB FPGA event"<<std::endl;
            mFEQ->Enque(*mFE);
			continue;
		}

		// manage Eob packet
		if(mFE->triggerWord == 35) {
			std::cout<<"\nDecoderFPGA->EOB"<<std::endl;
//			printf("sw counter =	%08x\n", i);
//			printf("Fpga counter =	%08x\n", mFE->triggerNumber);
			mFE->triggerNumber = i & 0x00FFFFFF; // overwrite trigger number taken from sw
            std::cout<<"triggerNumber:    0x"<<std::hex<<(int)mFE->triggerNumber<<std::endl;
            std::cout<<"triggerWord:      0x"<<std::hex<<(int)mFE->triggerWord<<std::endl;
            std::cout<<"timeStamp:        0x"<<std::hex<<(int)mFE->timeStamp<<std::endl;
            this->Print();
            i=0;
//            std::cout<<"Enque EoB FPGA event"<<std::endl;
            mFEQ->Enque(*mFE);
			continue;
		}

		type[mFE->triggerWord]++;

		if(i != mFE->triggerNumber) {
			std::cout<<"ERROR -> FPGA: Wrong trigger number"<<std::endl;
			printf("sw counter =	%08x\n", i);
			printf("Fpga counter =	%08x\n", mFE->triggerNumber);
//			sleep(1);
		}

		i++;

//		std::cout<<"Enque Normal FPGA event"<<std::endl;
		mFEQ->Enque(*mFE);
	}
}

void DecoderFpgaEvent::Print() {
	std::cout<<"DecoderFPGA:"<<std::endl;
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
