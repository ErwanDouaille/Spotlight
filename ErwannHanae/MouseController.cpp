#include "MouseController.h"

using namespace lg;

MouseController::MouseController( string name) : Observer(name)
{

}

MouseController::~MouseController(void)
{
}

Node* MouseController::clone( string name ) const
{
	return new MouseController(name);
}

bool MouseController::start()
{
	cout << "Start MouseController" << endl;
	return true;
}

bool MouseController::stop()
{
	cout << "Stop MouseController" << endl;
	return true;
}

bool MouseController::observe( map<string,Group3D*> groups3D,map<string,Group2D*> groups2D, map<string,Group1D*> groups1D, map<string,GroupSwitch*> groupsSwitch )
{
	// Check the raycast group
	for(map<string,Group2D*>::iterator mit = groups2D.begin();mit != groups2D.end();mit++){

		set<HOrientedPoint2D*> raycastPoints = mit->second->getElementsByType("RAYCAST");

		if(raycastPoints.size() > 0 ) {
			HOrientedPoint2D* raycastPoint = *raycastPoints.begin();
			if(raycastPoint){
				OrientedPoint2D* raycastPointP = raycastPoint->getLast();
				
				if (raycastPointP->getPosition().getX()>0 && raycastPointP->getPosition().getX()<=1 
					&& raycastPointP->getPosition().getY()>0 && raycastPointP->getPosition().getY()<=1)
				{
					// Create the INPUT structure to send
					INPUT ip = {0};

					ip.type = INPUT_MOUSE;
					ip.mi.dx = long(1-raycastPointP->getPosition().getX() * 65535 - 1);
					ip.mi.dy = long((1-raycastPointP->getPosition().getY()) * 65535 - 1);
					ip.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

					SendInput(1, &ip, sizeof(INPUT));

					// Draw circle
					HDC hdc;
					SIZE s;
					s.cx = ::GetSystemMetrics(SM_CXSCREEN);
					s.cy = ::GetSystemMetrics(SM_CYSCREEN);
					int x, y, z, r, g, b;
					int horizontal = 0;
					int vertical = 0;
					RECT desktop;
					// Get a handle to the desktop window
					const HWND hDesktop = GetDesktopWindow();
					// Get the size of screen to the variable desktop
					GetWindowRect(hDesktop, &desktop);
					// The top left corner will have coordinates (0,0)
					// and the bottom right corner will have coordinates
					// (horizontal, vertical)
					horizontal = desktop.right;
					vertical = desktop.bottom;
					HBRUSH hbr, hbrOld;
					x = float((1-raycastPointP->getPosition().getX()) * horizontal);
					y = float((1-raycastPointP->getPosition().getY()) * vertical);
					z = 30; // radius
					r = 255; // red color componennt
					g = 200;// green color component
					b = 0;// blue color component
					hbr = ::CreateSolidBrush(RGB(r,g,b));
					hdc = ::GetDCEx(NULL, 0, 0);
					hbrOld = (HBRUSH) ::SelectObject(hdc, hbr);
					::Ellipse(hdc, x - z, y - z, x + z, y + z);
					::SelectObject(hdc, hbrOld);
					::DeleteObject(hbr);
					::ReleaseDC(NULL, hdc);

					cout << "x : " << horizontal << " y: " << vertical << endl;
				}
			}
		}
	}

	return true;
}

set<string> MouseController::need() const
{
	set<string> needed;
	return needed;
}