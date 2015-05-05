#pragma once
#include "LgProcessor.h"

#include "Eigen\Geometry"

class SpotlightProcessor : public lg::Processor
{
protected:
	float _height, _width, _originScreenX, _originScreenY;

	Eigen::Vector3f* _ray;
	Eigen::Vector3f* _posUser;
	Eigen::Vector3f* _orientationUser;

	Eigen::Affine3f* _matrixToScreen;
	Eigen::Affine3f* _matrixToWorld;

public:
	SpotlightProcessor(string);
	~SpotlightProcessor(void);

	Node* clone(string name) const;

	//void setObserver(Observer* obs);

	bool start();
	bool stop();

	bool update(map<string,lg::Group3D*>& groups3D, map<string,lg::Group2D*>& groups2D, map<string,lg::Group1D*>& groups1D, map<string,lg::GroupSwitch*>& groupsSwitch);

	set<string> need() const; 
	set<string> consume() const; 
	set<string> produce() const; 
};

