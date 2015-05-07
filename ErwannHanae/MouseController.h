#pragma once
#include "Windows.h"
#include "Winuser.h"
#include "LgObserver.h"

#include "math.h"

class MouseController : public lg::Observer
{
public:
	MouseController(string);
	~MouseController(void);

	Node* clone(string name) const;

	bool start();
	bool stop();

	bool observe(map<string,lg::Group3D*> groups3D,map<string,lg::Group2D*> groups2D, map<string,lg::Group1D*> groups1D, map<string,lg::GroupSwitch*> groupsSwitch);

	set<string> need() const; 

	void setThreshold(int);
};

