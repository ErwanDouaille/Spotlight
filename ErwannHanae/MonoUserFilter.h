#pragma once

#include "LgProcessor.h"
#include "LgObserver.h"

#include <vector>

using namespace lg;
using namespace std;

class MonoUserFilter :
	public Processor
{
private:
	set<string> actives;
	Observer* _chooser;

	float _thres;
	bool _multi;
	bool _doing;
public:
	MonoUserFilter(string);
	~MonoUserFilter(void);

	Node* clone(string name) const;

	void setObserver(Observer* obs);

	bool start();
	bool stop();

	bool update(map<string,Group3D*>& groups3D, map<string,Group2D*>& groups2D, map<string,Group1D*>& groups1D, map<string,GroupSwitch*>& groupsSwitch);

	set<string> need() const; 
	set<string> consume() const; 
	set<string> produce() const; 

	void setThres(float);
	void enableMulti(bool);
	void enableDoing(bool);
};

