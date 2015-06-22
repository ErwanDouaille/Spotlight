#include "UDPSender.h"

UDPSender::UDPSender(string name, char* ip, int port) : Observer(name)
{
	this->slen = sizeof(si_other);
	this->ip = ip;
	this->port = port;
}

UDPSender::~UDPSender(void)
{	
}

Node* UDPSender::clone(string name) const{
	return 0;
}

bool UDPSender::start(){
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
		printf("Failed. Error Code : %d",WSAGetLastError());
		return false;
	}

	if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
		printf("socket() failed with error code : %d" , WSAGetLastError());
		return false;
	}
	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	si_other.sin_addr.S_un.S_addr = inet_addr(ip);
	cout << port << endl;
	return true;
}

bool UDPSender::stop(){
	return true;
}

bool UDPSender::observe(map<string,Group3D*> groups3D,map<string,Group2D*> groups2D, map<string,Group1D*> groups1D, map<string,GroupSwitch*> groupsSwitch) {
	std::stringstream ss;
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
								printf("UDP Sender - Send %i - %s %s : %s %s [%f;%f;%f] [%f;%f;%f] %f %f\n",_environment->getTime(), git->first.c_str(),git->second->getType().c_str(),pit->first.c_str(),pit->second->getType().c_str(),o3D->getPosition().getX(),o3D->getPosition().getY(),o3D->getPosition().getZ(),o3D->getOrientation().getX(),o3D->getOrientation().getY(),o3D->getOrientation().getZ(),o3D->getConfidence(),o3D->getIntensity() ); 
							ss << "id" << pit->first.c_str() << ":" << o3D->getPosition().getX() << ":" << o3D->getPosition().getY() << ":" << o3D->getPosition().getZ();
						}
					}
				}
			}
		}
	}
	/*
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
	*/
	strcpy(message, ss.str().c_str());
	if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR) {
		printf("sendto() failed with error code : %d\n" , WSAGetLastError());
		return false;
	}
	return true;
}