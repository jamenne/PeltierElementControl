/*

MultiMeterMain.cpp

Created by Janine Müller on 07.10.2016

*/

#include <iostream>

#include "../MultiMeter/MultiMeter.h"
#include "../SourceMeter/SourceMeter.h"
#include "../gpib/gpib.h"
#include "PelztierControl.h"

int main(int argc, char const *argv[])
{

	int masterUD = InitializeMaster();

	SourceMeter SourceM;

	MultiMeter MultiM;

	// SourceMeter, Output, MultiMeter
	Pelztier Peltier(SourceM, 1, MultiM);

	// masterUD, SourceMeterPad, MultiMeterPad, voltagelimit
	Peltier.Initialize(masterUD, 26, 18, "0.6");

	Peltier.TemperatureController(-20);

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
