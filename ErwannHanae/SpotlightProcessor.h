#pragma once

#include <math.h>

#include "LgProcessor.h"
#include "Eigen\Geometry"

class SpotlightProcessor : public lg::Processor
{
protected:
	float _height, _width;

	Eigen::Vector3f* _ray;
	Eigen::Vector3f* _posUser;
	Eigen::Vector3f* _orientationUser;
	Eigen::Vector3f* _screenNormal;
	Eigen::Vector3f* _screenOrigin;
	Eigen::Vector3f* _raycastIntersection;

	Eigen::Vector3f* raycastVector();

public:
	SpotlightProcessor(string);
	~SpotlightProcessor(void);

	Node* clone(string name) const;

	bool start();
	bool stop();
	bool update(map<string,lg::Group3D*>& groups3D, map<string,lg::Group2D*>& groups2D, map<string,lg::Group1D*>& groups1D, map<string,lg::GroupSwitch*>& groupsSwitch);

	set<string> need() const; 
	set<string> consume() const; 
	set<string> produce() const;
	const Eigen::Vector3f getRaycastIntersection() { return *_raycastIntersection;}

};

