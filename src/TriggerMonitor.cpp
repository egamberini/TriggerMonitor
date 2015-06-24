//============================================================================
// Name        : TriggerMonitor.cpp
// Author      : enrico.gamberini@fe.infn.it
// Version     :
// Description : Software for monitoring the L0TP triggers
//============================================================================

#include <iostream>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "Receiver.hpp"
#include "Decoder.hpp"
#include "DecoderFpgaEvent.hpp"
#include "Merger.hpp"
#include "MepAssembler.hpp"
#include "DimInterface.hpp"
#include "EobDataHandler.hpp"

using namespace std;

int main() {
	DimInterface * dimIface = new DimInterface();
	sleep(1);

	/* cpu receiver initialization */
	Queue<udp_t> * cpuRxQueue = new Queue<udp_t>;
	RxSocket * cpuRxSock = new RxSocket(localIP1, remoteIP1, localPort1, remotePort1);

	boost::thread * cpuRcvrThread;
	Receiver cpuRcvr(cpuRxSock, cpuRxQueue);
	cpuRcvrThread = new boost::thread(boost::bind(&Receiver::thread, cpuRcvr));

	/* cpu mep decoder initialization */
	Queue<CPU_EVENT> * cpuEvntQue = new Queue<CPU_EVENT>;
	EobDataHandler * eobDataH = new EobDataHandler();

	boost::thread * cpuDcdrThread;
	Decoder cpuDcdr(cpuRxQueue, cpuEvntQue, eobDataH);
	cpuDcdrThread = new boost::thread(boost::bind(&Decoder::thread, cpuDcdr));

	/* fpga receiver initialization */
	Queue<udp_t> * fpgaRxQueue = new Queue<udp_t>;
	RxSocket * fpgaRxSock = new RxSocket(localIP2, remoteIP2, localPort2, remotePort2);

	boost::thread * fpgaRcvrThread;
	Receiver fpgaRcvr(fpgaRxSock, fpgaRxQueue);
	fpgaRcvrThread = new boost::thread(boost::bind(&Receiver::thread, fpgaRcvr));

	/* fpga event decoder initialization */
	Queue<FPGA_EVENT> * fpgaEvntQue = new Queue<FPGA_EVENT>;

	boost::thread * fpgaDcdrThread;
	DecoderFpgaEvent fpgaDcdr(fpgaRxQueue, fpgaEvntQue);
	fpgaDcdrThread = new boost::thread(boost::bind(&DecoderFpgaEvent::thread, fpgaDcdr));

	/* merger initialization */
	Queue<event_t> * eventQue = new Queue<event_t>;

	boost::thread * mergerThread;
	Merger mrgr(fpgaEvntQue, cpuEvntQue, eventQue, eobDataH);
	mergerThread = new boost::thread(boost::bind(&Merger::thread, mrgr));

	/* assembler initialization */
	TxSocket * TxSock = new TxSocket(localPcFarmIP, dimIface->enabledPCNodes, localPcFarmPort, remotePcFarmPort);
	boost::thread * assemblerThread;
	MepAssembler asmblr(eventQue, TxSock, eobDataH);
	assemblerThread = new boost::thread(boost::bind(&MepAssembler::thread, asmblr));


	assemblerThread->join();
//	dcdrThread->join();

//	delete cpuRcvrThread;
//	delete cpuDcdrThread;
//	delete fpgaRcvrThread;
//	delete fpgaDcdrThread;
//	delete mergerThread;
//	delete assemblerThread;


	return 0;
}
