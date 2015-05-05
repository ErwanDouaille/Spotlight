#pragma once

#include "LgObserver.h"

#define LG_POSE_THRESHOLD "LG_POSE_THRESHOLD"

using namespace lg;

class PoseObserver :
	public Observer
{
private:
	string _middleJoint;
	int _thres;
public:
	PoseObserver(string,string);
	~PoseObserver(void);

	Node* clone(string name) const;

	bool start();
	bool stop();

	bool observe(map<string,Group3D*> groups3D,map<string,Group2D*> groups2D, map<string,Group1D*> groups1D, map<string,GroupSwitch*> groupsSwitch);

	set<string> need() const; 

	void setThreshold(int);
};

