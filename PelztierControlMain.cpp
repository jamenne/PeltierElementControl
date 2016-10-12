/*

MultiMeterMain.cpp

Created by Janine Müller on 07.10.2016

*/

#include <iostream>

#include "../MultiMeter/MultiMeter.h"
#include "../SourceMeter/SourceMeter.h"
#include "../gpib/gpib.h"


int main(int argc, char const *argv[])
{

	int masterUD = InitializeMaster();

	MultiMeter* MultiM = new MultiMeter();

	MultiM->Initialize4Wire(masterUD, 18);
	
	return 0;
}
