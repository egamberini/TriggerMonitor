/*
 * DimInterface.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: gamberini
 */

#include <iostream>
#include "DimInterface.hpp"

DimInterface::DimInterface() {
	fInfoEOB = new DimInfo("NA62/Timing/EOB", -1, this);
	fInfoActivePC = new DimInfo("RunControl/EnabledPCNodes", "", this);
}

DimInterface::~DimInterface() {
	// TODO Auto-generated destructor stub
}

void DimInterface::infoHandler() {
	DimInfo *curr = getInfo();
	std::string activePCs;

	if(curr==fInfoEOB) {
		std::cout<<"EOB "<<curr->getInt()<<std::endl;
	}

	if(curr==fInfoActivePC) {
	activePCs = curr->getString();
//		cout<<activePCs<<endl;
	std::string search(",");
	std::size_t beginsAt = 0;
	std::size_t foundAt = 0;

	do {
		foundAt = activePCs.find(search, beginsAt);
		if(foundAt!=std::string::npos) {
				enabledPCNodes.push_back(activePCs.substr(beginsAt, foundAt-beginsAt));
			} else {
				foundAt = activePCs.size();
				enabledPCNodes.push_back(activePCs.substr(beginsAt, foundAt-beginsAt));
			}
			beginsAt = foundAt + 1;
		} while(foundAt != activePCs.size());

		for(unsigned int i=0; i<enabledPCNodes.size(); i++) {
			std::cout<<enabledPCNodes[i]<<std::endl;
		}
	}
}
