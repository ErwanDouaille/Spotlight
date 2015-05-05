#pragma once


#define _WINSOCKAPI_
//#include <windows.h>
//#include <winsock2.h>


#include "LgObserver.h"


#include "lo/lo.h"

using namespace lg;

class OSCSender:
	public Observer
{
private:
	set<lo_address> _clients;
public:
	OSCSender(string name);
	~OSCSender(void);

	Node* clone(string name) const;

	bool start();
	bool stop();

	bool observe(map<string,Group3D*> groups3D,map<string,Group2D*> groups2D, map<string,Group1D*> groups1D, map<string,GroupSwitch*> groupsSwitch);

	set<string> need() const {return set<string>();}


	void addClient(string address,string port);
};

