#include "OSCSender.h"


OSCSender::OSCSender(string name) : Observer(name)
{
}


OSCSender::~OSCSender(void)
{
	
}


Node* OSCSender::clone(string name) const{
	return 0;
}

bool OSCSender::start(){
	return true;
}

bool OSCSender::stop(){
	return true;
}

bool OSCSender::observe(map<string,Group3D*> groups3D,map<string,Group2D*> groups2D, map<string,Group1D*> groups1D, map<string,GroupSwitch*> groupsSwitch){

	// Parcours groups 3D
	for(map<string,Group3D*>::iterator git = groups3D.begin();git != groups3D.end();git++){
		// Check if group must be observed
		if(isObservedGroup(git->first,git->second->getType())){
			// Parcours points
			map<string,HOrientedPoint3D*> hops3D = git->second->getAll();
			for(map<string,HOrientedPoint3D*>::iterator pit = hops3D.begin();pit != hops3D.end();pit++){
				// Check if they must be observed
				if(isObservedType(pit->second->getType())){
					// Check if they are up to date
					HOrientedPoint3D* ho3D = pit->second;
					if(ho3D){
						if(ho3D->getLastTimestamp() == _timestamp){
							OrientedPoint3D* o3D = ho3D->getLast();
							if(_environment->getVerboseLevel() != LG_ENV_VERBOSE_MUTE)
								printf("OSC Sender - Send %i - %s %s : %s %s [%f;%f;%f] [%f;%f;%f] %f %f\n",_environment->getTime(), git->first.c_str(),git->second->getType().c_str(),pit->first.c_str(),pit->second->getType().c_str(),o3D->getPosition().getX(),o3D->getPosition().getY(),o3D->getPosition().getZ(),o3D->getOrientation().getX(),o3D->getOrientation().getY(),o3D->getOrientation().getZ(),o3D->getConfidence(),o3D->getIntensity() ); 
							// Send /3D time groupID groupType pointID pointType posX posY posZ oriX oriY oriZ confidence intensity
							for(set<lo_address>::iterator cit = _clients.begin();cit != _clients.end();cit++){
								if(_environment->getVerboseLevel() == LG_ENV_VERBOSE_HIGH)
									printf("\t Send To %s: %s\n", lo_address_get_hostname(*cit), lo_address_get_port(*cit)); 
								if (lo_send(*cit, "/3D", "issssffffffff" ,_environment->getTime(), git->first.c_str(),git->second->getType().c_str(),pit->first.c_str(),pit->second->getType().c_str(),o3D->getPosition().getX(),o3D->getPosition().getY(),o3D->getPosition().getZ(),o3D->getOrientation().getX(),o3D->getOrientation().getY(),o3D->getOrientation().getZ(),o3D->getConfidence(),o3D->getIntensity()) == -1) 
									printf("OSC error %d: %s\n", lo_address_errno(*cit), lo_address_errstr(*cit));
							}
						}
					}
				}
			}
		}
	}

		// Parcours groups 2D
	for(map<string,Group2D*>::iterator git = groups2D.begin();git != groups2D.end();git++){
		// Check if group must be observed
		if(isObservedGroup(git->first,git->second->getType())){
			// Parcours points
			map<string,HOrientedPoint2D*> hops2D = git->second->getAll();
			for(map<string,HOrientedPoint2D*>::iterator pit = hops2D.begin();pit != hops2D.end();pit++){
				// Check if they must be observed
				if(isObservedType(pit->second->getType())){
					// Check if they are up to date
					HOrientedPoint2D* ho2D = pit->second;
					if(ho2D){
						if(ho2D->getLastTimestamp() == _timestamp){
							OrientedPoint2D* o2D = ho2D->getLast();
							// Send /2D time groupID groupType pointID pointType posX posY ori confidence intensity
							for(set<lo_address>::iterator cit = _clients.begin();cit != _clients.end();cit++)
								if (lo_send(*cit, "/2D", "issssfffff" ,_environment->getTime(), git->first.c_str(),git->second->getType().c_str(),pit->first.c_str(),pit->second->getType().c_str(),o2D->getPosition().getX(),o2D->getPosition().getY(),o2D->getOrientation(),o2D->getConfidence(),o2D->getIntensity()) == -1) 
									printf("OSC error %d: %s\n", lo_address_errno(*cit), lo_address_errstr(*cit));
						}
					}
				}
			}
		}
	}

		// Parcours groups 1D
	for(map<string,Group1D*>::iterator git = groups1D.begin();git != groups1D.end();git++){
		// Check if group must be observed
		if(isObservedGroup(git->first,git->second->getType())){
			// Parcours points
			map<string,HOrientedPoint1D*> hops1D = git->second->getAll();
			for(map<string,HOrientedPoint1D*>::iterator pit = hops1D.begin();pit != hops1D.end();pit++){
				// Check if they must be observed
				if(isObservedType(pit->second->getType())){
					// Check if they are up to date
					HOrientedPoint1D* ho1D = pit->second;
					if(ho1D){
						if(ho1D->getLastTimestamp() == _timestamp){
							OrientedPoint1D* o1D = ho1D->getLast();
							// Send /1D time groupID groupType pointID pointType pos ori confidence intensity
							for(set<lo_address>::iterator cit = _clients.begin();cit != _clients.end();cit++)
								if (lo_send(*cit, "/1D", "issssfbff" ,_environment->getTime(), git->first.c_str(),git->second->getType().c_str(),pit->first.c_str(),pit->second->getType().c_str(),o1D->getPosition(),o1D->getOrientation(),o1D->getConfidence(),o1D->getIntensity()) == -1) 
									printf("OSC error %d: %s\n", lo_address_errno(*cit), lo_address_errstr(*cit));
						}
					}
				}
			}
		}
	}


		// Parcours groups Switch
	for(map<string,GroupSwitch*>::iterator git = groupsSwitch.begin();git != groupsSwitch.end();git++){
		// Check if group must be observed
		if(isObservedGroup(git->first,git->second->getType())){
			// Parcours points
			map<string,HSwitch*> hss = git->second->getAll();
			for(map<string,HSwitch*>::iterator pit = hss.begin();pit != hss.end();pit++){
				// Check if they must be observed
				if(isObservedType(pit->second->getType())){
					// Check if they are up to date
					HSwitch* hs = pit->second;
					if(hs){
						if(hs->getLastTimestamp() == _timestamp){
							Switch* s = hs->getLast();
							// Send /S time groupID groupType pointID pointType state
							for(set<lo_address>::iterator cit = _clients.begin();cit != _clients.end();cit++)
								if (lo_send(*cit, "/S", "issssb" ,_environment->getTime(), git->first.c_str(),git->second->getType().c_str(),pit->first.c_str(),pit->second->getType().c_str(),s->getState()) == -1) 
									printf("OSC error %d: %s\n", lo_address_errno(*cit), lo_address_errstr(*cit));
						}
					}
				}
			}
		}
	}

	



	

	return true;
}

void OSCSender::addClient(string address,string port){
	_clients.insert(lo_address_new(address.c_str(), port.c_str()));
}

