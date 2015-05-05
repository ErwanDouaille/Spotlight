#include "SpotlightProcessor.h"

using namespace lg;
using namespace std;
using namespace Eigen;

SpotlightProcessor::SpotlightProcessor(string name) : Processor(name)
{
	_height = 2000;
	_width = 4000;

	_originScreenX = 0;
	_originScreenY = 0;

	_ray = new Vector3f();
	_posUser = new Vector3f();
	_orientationUser = new Vector3f();

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
		set<HOrientedPoint3D*> rhands = mit->second->getElementsByType(LG_ORIENTEDPOINT3D_RIGHT_HAND);
		set<HOrientedPoint3D*> head = mit->second->getElementsByType(LG_ORIENTEDPOINT3D_HEAD);
		
			HOrientedPoint3D* rh = *rhands.begin();
			OrientedPoint3D* rhp = rh->getLast();

			HOrientedPoint3D* h = *head.begin();
			OrientedPoint3D* hp = h->getLast();

			*_ray = Vector3f(rhp->getPosition().getX(), rhp->getPosition().getY(), rhp->getPosition().getZ()) 
				- Vector3f(hp->getPosition().getX(), hp->getPosition().getY(), hp->getPosition().getZ());
			_ray->normalize();
			*_posUser = Vector3f(hp->getPosition().getX(), hp->getPosition().getY(), hp->getPosition().getZ());
			*_orientationUser = Vector3f(hp->getOrientation().getX(), hp->getOrientation().getY(), hp->getOrientation().getZ());
			_orientationUser->normalize();
				
			cout << "x:" << rhp->getPosition().getX() << "y:" << rhp->getPosition().getY() << "z:" << rhp->getPosition().getZ() << endl;
	}

	return true;
}

Eigen::Vector3f* SpotlightProcessor::raycastVector()
{
	float step = _posUser->norm() / 2.;
	Vector3f intersection = _posUser;

	while (intersection.z() > 0.001)
	{
		if( (intersection+step * (*_ray)).z() < 0)
		{
			step /= 2.;
		}

		intersection += step * (*_ray);
	}
}

