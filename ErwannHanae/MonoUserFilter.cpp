#include "MonoUserFilter.h"


MonoUserFilter::MonoUserFilter(string name) : Processor(name)
{
	_chooser = 0;

	_multi = false;
	_thres= 0.7f;
	_doing = false;



}

Node* MonoUserFilter::clone(string name) const
{
	return new MonoUserFilter(name);
}

MonoUserFilter::~MonoUserFilter(void)
{
}

void MonoUserFilter::setObserver(Observer* obs)
{
	_chooser = obs;
	_chooser->setEnvironment(_environment);
	_chooser->setID(_id);
}

bool MonoUserFilter::start()
{
	cout << "Start Processor" << endl;

	_chooser->setEnvironment(_environment);
	_chooser->setID(_id);

	return (_chooser && _chooser->start());
}

bool MonoUserFilter::stop()
{
	cout << "Stop Processor" << endl;

	_chooser->stop();

	return true;
}

bool MonoUserFilter::update(map<string,Group3D*>& g3D, map<string,Group2D*>& g2D, map<string,Group1D*>& g1D, map<string,GroupSwitch*>& groupsSwitch)
{
	//cout << "Update Processor" << endl;

	//cout << "Test 1" << endl;

	if(!_chooser)
		return false;

	//cout << "Test 2" << endl;

	if(!_chooser->update(g3D,g2D,g1D,groupsSwitch))
		return false;

	//cout << "Test 2" << endl;

	map<string,float> probas = _chooser->getProbabilities();
	
	if(_doing){
		//cout << "clear" << endl;
		actives.clear();
	}
	
	float maxProba = 0;
	for(map<string,float>::iterator pit = probas.begin();pit != probas.end();pit++){
		if(pit->second > _thres){
			//cout << "doing " << atof(getProperty(LG_MONO_THRESHOLD).c_str()) << endl;
			//cout << "doing " << pit->first.c_str() << endl;
			if(!_multi){
				if(pit->second > maxProba)
					actives.clear();
			}
			actives.insert(pit-> first);
			maxProba = pit->second;
		}
	}

	//cout << "Test 3" << endl;

	// Solution 1 : copy active groups, then delete everything, and finally replace maps
	/*map<string,Group3D*> c3D;
	map<string,Group2D*> c2D;
	map<string,Group1D*> c1D;
	for(set<string>::iterator ait = actives.begin();ait != actives.end();ait++){
		if(g3D.count(*ait)){
			c3D[*ait] = new Group3D(*g3D[*ait]);
		}
		if(g2D.count(*ait)){
			c2D[*ait] = new Group2D(*g2D[*ait]);
		}
		if(g1D.count(*ait)){
			c1D[*ait] = new Group1D(*g1D[*ait]);
		}
	}

	//cout << "Test 4" << endl;

	for(map<string,Group3D*>::iterator git = g3D.begin();git != g3D.end();git++)
		delete git->second;
	g3D.clear();
	for(map<string,Group2D*>::iterator git = g2D.begin();git != g2D.end();git++)
		delete git->second;
	g2D.clear();
	for(map<string,Group1D*>::iterator git = g1D.begin();git != g1D.end();git++)
		delete git->second;
	g1D.clear();

	//cout << "Test 5" << endl;

	g3D = c3D;
	g2D = c2D;
	g1D = c1D;*/


	//Solution 2 : delete groups which are not active
	set<string> del3D;
	set<string> del2D;
	set<string> del1D;
	for(map<string,Group3D*>::iterator git = g3D.begin();git != g3D.end();git++)
		if(actives.count(git->first) == 0) del3D.insert(git->first);
	for(map<string,Group2D*>::iterator git = g2D.begin();git != g2D.end();git++)
		if(actives.count(git->first) == 0) del2D.insert(git->first);
	for(map<string,Group1D*>::iterator git = g1D.begin();git != g1D.end();git++)
		if(actives.count(git->first) == 0) del1D.insert(git->first);

	for(set<string>::iterator ait = del3D.begin();ait != del3D.end();ait++)
		deleteGroup(g3D,*ait);
	for(set<string>::iterator ait = del2D.begin();ait != del2D.end();ait++)
		deleteGroup(g2D,*ait);
	for(set<string>::iterator ait = del1D.begin();ait != del1D.end();ait++)
		deleteGroup(g1D,*ait);

	return true;
}

set<string> MonoUserFilter::need() const
{
	set<string> needed;
	return needed;
}

set<string> MonoUserFilter::consume() const
{
	set<string> needed;
	return needed;
}

set<string> MonoUserFilter::produce() const
{
	set<string> needed;
	return needed;
}

void MonoUserFilter::setThres(float newThres){
	_thres = newThres;
}

void MonoUserFilter::enableMulti(bool multi){
	_multi = multi;
}

void MonoUserFilter::enableDoing(bool doing){
	_doing = doing;
}