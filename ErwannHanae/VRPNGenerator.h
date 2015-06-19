#pragma once

#define _WINSOCKAPI_
#include "LgGenerator.h"

#include "vrpn_Tracker.h"
#include "vrpn_Button.h"
#include "vrpn_Analog.h"

using namespace lg;

class VRPNGenerator : public Generator
{
private:
	vrpn_Tracker_Remote* headTracker;
	vrpn_Tracker_Remote* rightHandTracker;
	vrpn_Tracker_Remote* leftHandTracker;

public:
	VRPNGenerator(string name);
	~VRPNGenerator(void);
	Node* clone(string cloneName) const;
	bool start();
	bool stop();
	bool generate(map<string,Group3D*>& groups3D,map<string,Group2D*>& groups2D,map<string,Group1D*>&,map<string,GroupSwitch*>&);
	set<string> produce() const; 
};

