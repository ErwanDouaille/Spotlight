#include "DataTranslatorKinect.h"

using namespace lg;

DataTranslatorKinect::DataTranslatorKinect(string name) : Processor(name)
{
}

DataTranslatorKinect::~DataTranslatorKinect(void)
{
}

Node* DataTranslatorKinect::clone(string name) const
{
	return new DataTranslatorKinect(name);
}

bool DataTranslatorKinect::start()
{
	cout << "Start Kinect Data Translator Processor" << endl;
	return true;
}

bool DataTranslatorKinect::stop()
{
	cout << "Stop Kinect Data Translator Processor" << endl;
	return true;
}

set<string> DataTranslatorKinect::need() const
{
	set<string> needed;
	return needed;
}

set<string> DataTranslatorKinect::consume() const
{
	set<string> needed;
	return needed;
}

set<string> DataTranslatorKinect::produce() const
{
	set<string> needed;
	return needed;
}

bool DataTranslatorKinect::update(map<string,Group3D*>& g3D, map<string,Group2D*>& g2D, map<string,Group1D*>& g1D, map<string,GroupSwitch*>& groupsSwitch)
{

	for(map<string,Group3D*>::iterator mit = g3D.begin();mit != g3D.end();mit++){
		set<HOrientedPoint3D*> rhand = mit->second->getElementsByType(LG_ORIENTEDPOINT3D_RIGHT_HAND);
		set<HOrientedPoint3D*> head = mit->second->getElementsByType(LG_ORIENTEDPOINT3D_HEAD);

		if(rhand.size() > 0  && head.size() > 0 ) {
			HOrientedPoint3D* rh = *rhand.begin();
			HOrientedPoint3D* h = *head.begin();
			if(h && rh){
				OrientedPoint3D* rhp = rh->getLast();
				OrientedPoint3D* hp = h->getLast();	
				cout << "x:" << rhp->getPosition().getX() << "y:" << rhp->getPosition().getY() << "z:" << rhp->getPosition().getZ() << endl;

				updateData(_environment, g3D,"translateGroup", "TRANSLATE", "head", "TRANSLATE_HEAD", _timestamp,this->translateKinectData(hp));
				updateData(_environment, g3D,"translateGroup", "TRANSLATE", "right_hand", "TRANSLATE_RIGHT_HAND", _timestamp,this->translateKinectData(rhp));
			}
		}
	}
	return true;
}

OrientedPoint3D DataTranslatorKinect::translateKinectData(OrientedPoint3D* point) 
{
	float transformX = abs(((point->getPosition().getX()+1100)/2200)-1.);
	float transformY = 0.0;
	float transformZ = abs((point->getPosition().getZ()-800)/3200);
	Eigen::Vector3f a = Eigen::Vector3f(point->getPosition().getX(),
					point->getPosition().getY()* cos(-22./180.*3.14)-point->getPosition().getZ()*sin(-22/180.*3.14),
					point->getPosition().getY()* sin(-22./180.*3.14) + point->getPosition().getZ()* cos(-22./180.*3.14));
	transformY = (a.y()+1100)/1200;
	return OrientedPoint3D(Point3D(transformX, transformY, transformZ), 
		point->getOrientation(),
		0.0,
		0.0);
}
