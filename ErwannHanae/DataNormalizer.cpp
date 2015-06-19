#include "DataNormalizer.h"

using namespace lg;

DataNormalizer::DataNormalizer(string name) : Processor(name)
{
}

DataNormalizer::~DataNormalizer(void)
{
}

Node* DataNormalizer::clone(string name) const
{
	return new DataNormalizer(name);
}

bool DataNormalizer::start()
{
	cout << "Start Kinect Data Translator Processor" << endl;
	return true;
}

bool DataNormalizer::stop()
{
	cout << "Stop Kinect Data Translator Processor" << endl;
	return true;
}

set<string> DataNormalizer::need() const
{
	set<string> needed;
	return needed;
}

set<string> DataNormalizer::consume() const
{
	set<string> needed;
	return needed;
}

set<string> DataNormalizer::produce() const
{
	set<string> needed;
	return needed;
}

bool DataNormalizer::update(map<string,Group3D*>& g3D, map<string,Group2D*>& g2D, map<string,Group1D*>& g1D, map<string,GroupSwitch*>& groupsSwitch)
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

				updateData(_environment, g3D,"translateGroup", "TRANSLATE", "head", "TRANSLATE_HEAD", _timestamp,this->normalizeData(hp));
				updateData(_environment, g3D,"translateGroup", "TRANSLATE", "right_hand", "TRANSLATE_RIGHT_HAND", _timestamp,this->normalizeData(rhp));
				//cout << "x: " << hp->getOrientation().getX() << " \ty: " << hp->getOrientation().getY() << "\tz:" << hp->getOrientation().getZ() << endl;
			}
		}
	}
	return true;
}

OrientedPoint3D DataNormalizer::normalizeData(OrientedPoint3D* point) 
{
	float transformX = 1 - abs(((point->getPosition().getX()+1100)/2200)-1.);
	float transformY = point->getPosition().getY()/2000.;
	float transformZ = abs((point->getPosition().getZ()-800)/3200);
	//cout << "\t ici " <<		point->getOrientation().getX() << endl;
	return OrientedPoint3D(Point3D(transformX, transformY, transformZ), 
		point->getOrientation(),
		0.0,
		0.0);
}
