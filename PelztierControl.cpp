/*

MultiMeter.cpp


Created by Janine Müller on 07.10.2016

*/

#include <gpib/ib.h>
#include <gpib/gpib_user.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include "time.h"
#include <sstream>

#include "PelztierControl.h"
//#include "../MultiMeter/MultiMeter.h"
//#include "../SourceMeter/SourceMeter.h"

using namespace std;

//---------------------Pelztier class---------------------//

// Constructor
Pelztier::Pelztier()
{

	_value=314;
	_temperature=314;
	_current=314;
	_voltage=314;

}

//Destructor
Pelztier::~Pelztier(){}


Pelztier::ReturnTemperature(){


}

