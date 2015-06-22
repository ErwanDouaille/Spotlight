#pragma once

#define _WINSOCKAPI_
#include <iostream>
#include <map>
#include <math.h>
#include <winsock2.h>
#include <sstream>

#define BUFLEN 1024

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include "LgObserver.h"

using namespace lg;
using namespace std;

class UDPSender : public Observer
{
protected:
	struct sockaddr_in si_other;
	int s, slen, port;
	char* ip;
	char message[BUFLEN];
	WSADATA wsa;

public:
	UDPSender(string name, char* ip, int port);
	~UDPSender(void);

	Node* clone(string name) const;

	bool start();
	bool stop();
	bool observe(map<string,Group3D*> groups3D,map<string,Group2D*> groups2D, map<string,Group1D*> groups1D, map<string,GroupSwitch*> groupsSwitch);

	set<string> need() const {return set<string>();}
};

