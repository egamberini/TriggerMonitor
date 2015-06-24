/*
 * DimInterface.hpp
 *
 *  Created on: Apr 8, 2015
 *      Author: gamberini
 */

#ifndef DIMINTERFACE_HPP_
#define DIMINTERFACE_HPP_

#include <string>
#include <vector>

#include "dic.hxx"

class DimInterface: public DimClient {

private:
	DimInfo * fInfoEOB;
	DimInfo * fInfoActivePC;

public:
	std::vector<std::string> enabledPCNodes;

	DimInterface();
	virtual ~DimInterface();

	void infoHandler();
};

#endif /* DIMINTERFACE_HPP_ */
