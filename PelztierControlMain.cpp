/*

MultiMeterMain.cpp

Created by Janine Müller on 07.10.2016

*/

#include <iostream>
#include <fcntl.h>

#include "../MultiMeter/MultiMeter.h"
#include "../SourceMeter/SourceMeter.h"
#include "../gpib/gpib.h"
#include "PelztierControl.h"

int main(int argc, char const *argv[])
{

	// ----------Flags for while(true) interrupt------------- //
	int fd = STDIN_FILENO;
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);


	int masterUD = InitializeMaster();

	// -------------------------SourceMeter----------------------------//
	SourceMeter SourceM;

	int SourceMeterPad = 26;

	SourceM.Initialize(masterUD, SourceMeterPad);

	

	// -------------------------MultiMeter----------------------------//	

	MultiMeter MultiM;

	int MultiMeterPad = 18;

	MultiM.Initialize(masterUD, MultiMeterPad);



	// -------------------------Pelztier----------------------------//

	// SourceMeter, Output, MultiMeter
	Pelztier Peltier(SourceM, 1, MultiM);

	const string voltagelimit = "0.6";

	Peltier.Initialize(voltagelimit);

	double temp_target = 5;

	int index = 0;

	double integral = 0;

	vector<double> TempDiff(10,0);

	double current = 0;

	do{
		Peltier.OneTempControl(TempDiff, integral, index, current, temp_target);
		sleep(1);
	}while(getchar() != 'q');

//	Peltier.TemperatureController(-20);

//	Peltier.ITCurve(0, 0.4, 0.02);

//	Peltier.SetCurrent("0.425");

//	sleep(5*60);

//	vector<double> measure(2,0);

//	measure = Peltier.MeasureIV();

//	cout << measure[0] << "\t" << measure[1] << endl;

//	double temp = Peltier.GetTemperature();
	
//	cout << temp << endl;

	Peltier.Close();

	
	return 0;
}
