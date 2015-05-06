#include "SpotlightProcessor.h"

using namespace lg;
using namespace std;
using namespace Eigen;

SpotlightProcessor::SpotlightProcessor(string name) : Processor(name)
{
	_height = 2000;
	_width = 4000;

	_ray = new Vector3f();
	_posUser = new Vector3f();
	_orientationUser = new Vector3f();
	_screenNormal = new Vector3f(0,0,1);
	_screenOrigin = new Vector3f(0,0,0);
}

SpotlightProcessor::~SpotlightProcessor(void)
{
}

Node* SpotlightProcessor::clone(string name) const
{
	return new SpotlightProcessor(name);
}

bool SpotlightProcessor::start()
{
	cout << "Start Spotlight Processor" << endl;
	return true;
}

bool SpotlightProcessor::stop()
{
	cout << "Stop Spotlight Processor" << endl;
	return true;
}

set<string> SpotlightProcessor::need() const
{
	set<string> needed;
	return needed;
}

set<string> SpotlightProcessor::consume() const
{
	set<string> needed;
	return needed;
}

set<string> SpotlightProcessor::produce() const
{
	set<string> needed;
	return needed;
}

bool SpotlightProcessor::update(map<string,Group3D*>& g3D, map<string,Group2D*>& g2D, map<string,Group1D*>& g1D, map<string,GroupSwitch*>& groupsSwitch)
{

	for(map<string,Group3D*>::iterator mit = g3D.begin();mit != g3D.end();mit++){
		//cout << " 3D Groups " << mit->first.c_str() << endl;
		set<HOrientedPoint3D*> rhand = mit->second->getElementsByType("TRANSLATE_HEAD");
		set<HOrientedPoint3D*> head = mit->second->getElementsByType("TRANSLATE_RIGHT_HAND");

		if(rhand.size() > 0  && head.size() > 0 ) {
			HOrientedPoint3D* rh = *rhand.begin();
			HOrientedPoint3D* h = *head.begin();
			if(h && rh){
				OrientedPoint3D* rhp = rh->getLast();
				OrientedPoint3D* hp = h->getLast();
				*_ray = Vector3f(rhp->getPosition().getX(), rhp->getPosition().getY(), rhp->getPosition().getZ()) 
					- Vector3f(hp->getPosition().getX(), hp->getPosition().getY(), hp->getPosition().getZ());
				_ray->normalize();
				*_posUser = Vector3f(hp->getPosition().getX(), hp->getPosition().getY(), hp->getPosition().getZ());
				*_orientationUser = Vector3f(hp->getOrientation().getX(), hp->getOrientation().getY(), hp->getOrientation().getZ());
				_orientationUser->normalize();

				cout << "x:" << rhp->getPosition().getX() << "y:" << rhp->getPosition().getY() << "z:" << rhp->getPosition().getZ() << endl;
				_raycastIntersection = this->raycastVector();
				updateData(_environment,g2D, "RAYCAST","RAYCAST","_raycastIntersection","RAYCAST",_timestamp,OrientedPoint2D(Point2D(_raycastIntersection->x(),_raycastIntersection->y()),0.0,1.0,0.0));
			}
		}
	}
	return true;
}

Eigen::Vector3f* SpotlightProcessor::raycastVector()
{
	float T =0.0, d = 0.0;
	d = -((*_screenNormal).dot(*_screenOrigin));
	T = -((*_screenNormal).dot((*_posUser))+d)/((*_screenNormal).dot(*_ray));
	return new Vector3f(_posUser->x() + _ray->x()*T,
		_posUser->y() + _ray->y()*T,
		_posUser->z() + _ray->z()*T);
}

