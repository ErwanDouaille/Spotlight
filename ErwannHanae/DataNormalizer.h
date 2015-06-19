#pragma once

#include "LgProcessor.h"
#include "Eigen\Geometry"

class DataNormalizer : public lg::Processor
{
protected:
	lg::OrientedPoint3D normalizeData(lg::OrientedPoint3D*);

public:
	DataNormalizer(string);
	~DataNormalizer(void);
	
	Node* clone(string name) const;

	bool start();
	bool stop();
	bool update(map<string,lg::Group3D*>& groups3D, map<string,lg::Group2D*>& groups2D, map<string,lg::Group1D*>& groups1D, map<string,lg::GroupSwitch*>& groupsSwitch);

	set<string> need() const; 
	set<string> consume() const; 
	set<string> produce() const;
};

