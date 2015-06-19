#pragma once

#define _WINSOCKAPI_

#include <Windows.h>
#include <vector>

#include "LgEnvironment.h"

#include "DataNormalizer.h"
#include "KinectGenerator.h"
#include "MonoUserFilter.h"
#include "MouseController.h"
#include "OSCSender.h"
#include "PoseObserver.h"
#include "SpotlightProcessor.h"
#include "VRPNGenerator.h"

using namespace std;

struct pos_t{
	float x;
	float y;
};

Environment* myEnv;
KinectGenerator* gt;
VRPNGenerator* vrpn;
SpotlightProcessor* sp;
DataNormalizer* normalizer;
MonoUserFilter* mf;
PoseObserver* po;
MouseController* mouseController;
OSCSender* osc;
vector<pos_t> _pos;

int main(int argc, char* argv[])
{

	myEnv = new Environment();
	myEnv->enableDataCopy(false);
	myEnv->setHistoricLength(1);
	//myEnv->setVerboseLevel(LG_ENV_VERBOSE_NORMAL);

	// Historic length is the number of successive value to keep -> default is 10, reduce it to 2 or 3 can be time efficient
	//myEnv->setHistoricLength(3);

	// KINECT
	/*
	gt = new KinectGenerator("KinectTest"); 
	//gt->useCameraElevation();
	gt->setCameraPosition(0,100,0);
	if(myEnv->registerNode(gt))
		printf("Register Kinect OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());
	*/

	//VRPN 
	vrpn = new VRPNGenerator("Optitracker"); 
	if(myEnv->registerNode(vrpn))
		printf("Register Optitracker OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());

	normalizer = new DataNormalizer("DataTranslatorVRPN"); 
	if(myEnv->registerNode(normalizer))
		printf("Register Normalizer Processor OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());

	mf = new MonoUserFilter("MonoUserFilter"); 
	mf->setThres(0.9f);
	mf->enableDoing(true);
	po = new PoseObserver("PoseObserver",LG_ORIENTEDPOINT3D_NECK);
	po->setThreshold(200);
	//mf->setObserver(po);
	
	sp = new SpotlightProcessor("SpotlightProcessor"); 
	if(myEnv->registerNode(sp))
		printf("Register Spotlight Processor OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());

	/*if(myEnv->registerNode(mf))
		printf("Register MonoUser OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());
		
	mouseController = new MouseController("MouseController");
	if(myEnv->registerNode(mouseController))
		printf("Register mouseController OK.\n");
	else
		printf("%s.\n",myEnv->getLastError().c_str());
	*/

	osc = new OSCSender("OSCSender");
	osc->addClient("127.0.0.1","3335");
	// Ex pour ne regarder que la tête, les mains et un group POINTING
	osc->onlyObserveGroupType("TRANSLATE");
	//osc->onlyObserveGroupType("RAYCAST");
	//osc->onlyObservePointType("RAYCAST");
	//osc->onlyObservePointType("TRANSLATE_HEAD");
	//osc->onlyObservePointType(LG_ORIENTEDPOINT3D_LEFT_HAND);
	//osc->onlyObservePointType(LG_ORIENTEDPOINT3D_RIGHT_HAND);
	myEnv->registerNode(osc);

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
	}


	return 0;
}

