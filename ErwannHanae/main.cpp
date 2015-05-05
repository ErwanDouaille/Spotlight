#pragma once

#define _WINSOCKAPI_

#include <Windows.h>
#include <vector>

#include "LgEnvironment.h"
#include "KinectGenerator.h"
#include "PoseObserver.h"
#include "MonoUserFilter.h"
#include "OSCSender.h"
#include "SpotlightProcessor.h"

using namespace std;

struct pos_t{
	float x;
	float y;
};

Environment* myEnv;
KinectGenerator* gt;
SpotlightProcessor* sp;
MonoUserFilter* mf;
PoseObserver* po;
OSCSender* osc;
vector<pos_t> _pos;

int main(int argc, char* argv[])
{

	myEnv = new Environment();
	myEnv->enableDataCopy(false);
	//myEnv->setVerboseLevel(LG_ENV_VERBOSE_NORMAL);

	// Historic length is the number of successive value to keep -> default is 10, reduce it to 2 or 3 can be time efficient
	//myEnv->setHistoricLength(3);

	
	gt = new KinectGenerator("KinectTest"); 
	if(myEnv->registerNode(gt))
		printf("Register Kinect OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());
	



	sp = new SpotlightProcessor("SpotlightProcessor"); 
	if(myEnv->registerNode(sp))
		printf("Register Spotlight Processor OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());
	




	mf = new MonoUserFilter("MonoUserFilter"); 
	mf->setThres(0.9f);

	mf->enableDoing(true);

	po = new PoseObserver("PoseObserver",LG_ORIENTEDPOINT3D_NECK);
	po->setThreshold(200);
	mf->setObserver(po);


	/*if(myEnv->registerNode(mf))
		printf("Register MonoUser OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());
		*/

	// TODO ajouter le Processor de pointage 


	osc = new OSCSender("OSCSender");
	osc->addClient("127.0.0.1","3335");
	// Ex pour ne regarder que la tête, les mains et un group POINTING
	//osc->onlyObserveGroupType("POINTING");
	//osc->onlyObservePointType(LG_ORIENTEDPOINT3D_HEAD);
	//osc->onlyObservePointType(LG_ORIENTEDPOINT3D_LEFT_HAND);
	//osc->onlyObservePointType(LG_ORIENTEDPOINT3D_RIGHT_HAND);
	myEnv->registerNode(mf);

	
	if(myEnv->checkCompatibility())
		printf("Compatibility OK.\n");
	else{
		printf("Compatibility not OK.\n");
		printf("%s.\n",myEnv->getLastError().c_str());
	}

	if(myEnv->start())
		printf("Start OK.\n");
	else{
		printf("Start not OK.\n");
		printf("%s.\n",myEnv->getLastError().c_str());
	}


	while(true){
	
		myEnv->update();

		map<string,Group3D*> g3 = myEnv->getGroups3D();

		//printf("%i groups present\n",g2.size());

		if(g3.size() > 0)
			printf("1 group present at %i\n",myEnv->getTime());

		//printf("Time %i\n",myEnv->getTime());

		/*for(map<string,Group2D*>::iterator git = g2.begin();git != g2.end();git++){
			map<string,HOrientedPoint2D*> elements = git->second->getAll();
			printf("\tgroup %s has %i elements\n",git->first.c_str(),elements.size());
			for(map<string,HOrientedPoint2D*>::iterator eit = elements.begin();eit != elements.end();eit++){
				pos_t onePos;
				onePos.x = eit->second->getLast()->getPosition().getX();
				onePos.y = eit->second->getLast()->getPosition().getY();
				_pos.push_back(onePos);
			}
		}*/
	
	}


	return 0;
}

