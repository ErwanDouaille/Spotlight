#include "VRPNGenerator.h"
#include <vector>

int a = 0;
int b = 1;
int c = 2;

OrientedPoint3D* headOrientedPoint = new OrientedPoint3D(Point3D(.0,.0,.0), Point3D(.0,.0,.0), .0, .0);
OrientedPoint3D* rightHandOrientedPoint = new OrientedPoint3D(Point3D(.0,.0,.0), Point3D(.0,.0,.0), .0, .0);
OrientedPoint3D* leftHandOrientedPoint = new OrientedPoint3D(Point3D(.0,.0,.0), Point3D(.0,.0,.0), .0, .0);

Orientation3D convertOrientationsToEuler(float* q1){
	float M_PI = 3.1415926f;
	float heading,attitude,bank;
	float test = q1[0]*q1[1] + q1[2]*q1[3];
	if (test > 0.499f) { // singularity at north pole
		heading = 2.0f * atan2(q1[0],q1[3]);
		attitude = M_PI/2.0f;
		bank = 0;
	}
	else{
		if (test < -0.499) { // singularity at south pole
			heading = -2 * atan2(q1[0],q1[3]);
			attitude = - M_PI/2;
			bank = 0;
		}
		else{
			double sqx = q1[0]*q1[0];
			double sqy = q1[1]*q1[1];
			double sqz = q1[2]*q1[2];
			heading = atan2((float)(2.0f*q1[1]*q1[3]-2.0f*q1[0]*q1[2]) ,(float) (1.0f - 2.0f*sqy - 2.0f*sqz));
			attitude = asin(2.0f*test);
			bank = atan2((float)(2.0f*q1[0]*q1[3]-2.0f*q1[1]*q1[2]) ,(float)( 1.0f - 2.0f*sqx - 2.0f*sqz));
		}
	}

	return Orientation3D(attitude,heading,bank);
}

void VRPN_CALLBACK handle_head_tracker(void* userData, const vrpn_TRACKERCB t )
{
	float arrayF[4] = { (float)t.quat[0], (float)t.quat[1], (float)t.quat[2], (float)t.quat[3]}; 
	if(*((int*)userData) == 0) {
		delete(headOrientedPoint);
		headOrientedPoint = new OrientedPoint3D(Point3D((float)t.pos[0]*1000.0f, (float)t.pos[1]*1000.0f, (float)t.pos[2]*1000.0f), convertOrientationsToEuler(arrayF), 1.0, 0.0);
	}
	if(*((int*)userData) == 1){
		delete(rightHandOrientedPoint);
		rightHandOrientedPoint = new OrientedPoint3D(Point3D((float)t.pos[0]*1000.0f, (float)t.pos[1]*1000.0f, (float)t.pos[2]*1000.0f), convertOrientationsToEuler(arrayF), 1.0, 0.0);
	}
	if(*((int*)userData) == 2) {
		delete(leftHandOrientedPoint);
		leftHandOrientedPoint = new OrientedPoint3D(Point3D((float)t.pos[0]*1000.0f, (float)t.pos[1]*1000.0f, (float)t.pos[2]*1000.0f), convertOrientationsToEuler(arrayF), 1.0, 0.0);
	}
}

VRPNGenerator::VRPNGenerator(string name) : Generator(name)
{
	this->headTracker = new vrpn_Tracker_Remote("Head@localhost");
	this->rightHandTracker = new vrpn_Tracker_Remote("HandR@localhost");
	this->leftHandTracker = new vrpn_Tracker_Remote("HandL@localhost");

	this->headTracker->register_change_handler( &a, handle_head_tracker);
	this->rightHandTracker->register_change_handler( &b, handle_head_tracker );
	
}

VRPNGenerator::~VRPNGenerator(void)
{
}

Node* VRPNGenerator::clone(string cloneName) const
{
	// A VRPNGenerator cannot be cloned, it is better to return the current instance 
	// Another VRPNGenerator can be created for openning another device or another file
	return (Node*)this;
}

bool VRPNGenerator::start()
{
	if(!_environment){
		cout << "VRPNGenerator Error : Parent environment has not be set." << endl;
		return false;
	}
	return true;
}

bool VRPNGenerator::stop()
{
	cout << "Stop Generator" << endl;
	return true;
}

set<string> VRPNGenerator::produce() const
{
	set<string> produce;
	produce.insert(LG_ORIENTEDPOINT3D_HEAD);
	produce.insert(LG_ORIENTEDPOINT3D_LEFT_HAND);
	produce.insert(LG_ORIENTEDPOINT3D_RIGHT_HAND);

	produce.insert("HEAD");
	produce.insert("LEFT_HAND");
	produce.insert("RIGHT_HAND");
	return produce;
}

bool VRPNGenerator::generate(map<string,Group3D*>& groups3D,map<string,Group2D*>& g2D,map<string,Group1D*>&,map<string,GroupSwitch*>&)
{
	//call all the mainloop
	this->headTracker->mainloop();
	//update data from new ones
	updateData(_environment, groups3D, "user0", "OPTITRACK_DATA", "head", LG_ORIENTEDPOINT3D_HEAD, _timestamp, *headOrientedPoint);
	updateData(_environment, groups3D, "user0", "OPTITRACK_DATA", "left_hand", LG_ORIENTEDPOINT3D_LEFT_HAND,_timestamp, *leftHandOrientedPoint);
	updateData(_environment, groups3D, "user0", "OPTITRACK_DATA", "right_hand", LG_ORIENTEDPOINT3D_RIGHT_HAND,_timestamp, *rightHandOrientedPoint);
	Sleep(10);
	return true;
}
