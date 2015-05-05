#include "PoseObserver.h"


PoseObserver::PoseObserver(string name,string joint) : Observer(name)
{
	_thres = 200;

	_middleJoint = joint;
}


PoseObserver::~PoseObserver(void)
{
}

Node* PoseObserver::clone(string name) const
{
	return new PoseObserver(name,_middleJoint);
}

bool PoseObserver::start()
{
	cout << "Start Observer" << endl;
	return true;
}

bool PoseObserver::stop()
{
	cout << "Stop Observer" << endl;
	return true;
}

bool PoseObserver::observe(map<string,Group3D*> g3D,map<string,Group2D*> groups2D, map<string,Group1D*> groups1D, map<string,GroupSwitch*> groupsSwitch)
{
	//cout << "Update Observer" << endl;

	for(map<string,Group3D*>::iterator mit = g3D.begin();mit != g3D.end();mit++){
		//cout << " 3D Groups " << mit->first.c_str() << endl;
		set<HOrientedPoint3D*> lhands = mit->second->getElementsByType(LG_ORIENTEDPOINT3D_LEFT_HAND);
		set<HOrientedPoint3D*> lelbows = mit->second->getElementsByType(LG_ORIENTEDPOINT3D_LEFT_ELBOW);
		set<HOrientedPoint3D*> torsos = mit->second->getElementsByType(_middleJoint);
		set<HOrientedPoint3D*> relbows = mit->second->getElementsByType(LG_ORIENTEDPOINT3D_RIGHT_ELBOW);
		set<HOrientedPoint3D*> rhands = mit->second->getElementsByType(LG_ORIENTEDPOINT3D_RIGHT_HAND);

		if((lhands.size() == 1)&&(lelbows.size() == 1)&&(torsos.size() == 1)&&(relbows.size() == 1)&&(rhands.size() == 1)){
			HOrientedPoint3D* lh = *lhands.begin();
			HOrientedPoint3D* le = *lelbows.begin();
			HOrientedPoint3D* t = *torsos.begin();
			HOrientedPoint3D* re = *relbows.begin();
			HOrientedPoint3D* rh = *rhands.begin();

			OrientedPoint3D* lhp = lh->getLast();
			OrientedPoint3D* lep = le->getLast();
			OrientedPoint3D* tp = t->getLast();
			OrientedPoint3D* rep = re->getLast();
			OrientedPoint3D* rhp = rh->getLast();

			//cout << "Test is nul" << tp->getPosition().getX() << tp->getPosition().getX() << tp->getPosition().getX() << tp->getPosition().isNul() << endl;

			if((abs(lhp->getPosition().getY() - lep->getPosition().getY()) < _thres)
				&&(abs(lep->getPosition().getY() - tp->getPosition().getY()) < _thres)
				&&(abs(tp->getPosition().getY() - rep->getPosition().getY()) < _thres)
				&&(abs(rep->getPosition().getY() - rhp->getPosition().getY()) < _thres)
				&&(abs(lhp->getPosition().getY() - rhp->getPosition().getY()) < _thres)
				&&(!tp->getPosition().isNul())
				){
				updateProbability(mit->first,1.0);
				/*cout << "Left Elbow " << lep->getPosition().getY()<< endl;
				cout << "Left Hand " << lhp->getPosition().getY()<< endl;
				cout << "Torso " << tp->getPosition().getY()<< endl;
				cout << "Right Hand " << rhp->getPosition().getY()<< endl;
				cout << "Right Elbow " << rep->getPosition().getY()<< endl;*/
			}
			else
				updateProbability(mit->first,0.0);


		}
		else
			updateProbability(mit->first,0.0);


	}

	return true;
}

set<string> PoseObserver::need() const
{
	set<string> needed;
	needed.insert(LG_ORIENTEDPOINT3D_LEFT_HAND);
	needed.insert(LG_ORIENTEDPOINT3D_LEFT_ELBOW);
	needed.insert(_middleJoint);
	needed.insert(LG_ORIENTEDPOINT3D_RIGHT_ELBOW);
	needed.insert(LG_ORIENTEDPOINT3D_RIGHT_HAND);
	return needed;
}

void PoseObserver::setThreshold(int newThres){
	_thres = newThres;
}