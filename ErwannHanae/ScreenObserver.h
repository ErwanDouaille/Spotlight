#pragma once

#include "LgObserver.h"

class ScreenObserver
{

protected:
	int _height, _width;

public:
	ScreenObserver(string);
	~ScreenObserver(void);
	
	lg::Node* clone(string name) const;

	bool start();
	bool stop();

	bool observe(map<string, lg::Group3D*> groups3D,map<string, lg::Group2D*> groups2D, map<string,Group1D*> groups1D, map<string, lg::GroupSwitch*> groupsSwitch);
	
	const int getHeight() {return _height;}
	void setHeight(int newHeight) {_height = newHeight;}
	const int getWidth() {return _width;}
	void setWidth(int newWidth) {_width = newWidth;}
};

