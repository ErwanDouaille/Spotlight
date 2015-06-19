#pragma once

#define _WINSOCKAPI_

#include "LgObserver.h"

using namespace lg;

class UDPSender : public Observer
{
public:
	UDPSender(string name);
	~UDPSender(void);

	Node* clone(string name) const;

	bool start();
	bool stop();

	bool observe(map<string,Group3D*> groups3D,map<string,Group2D*> groups2D, map<string,Group1D*> groups1D, map<string,GroupSwitch*> groupsSwitch);

	set<string> need() const {return set<string>();}

	void addClient(string address,string port);
};

