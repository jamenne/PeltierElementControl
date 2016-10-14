/*

SourecMeter.h


Created by Janine Müller on 12.10.2016


*/

#ifndef ____Pelztier__
#define ____Pelztier__

#include "../SourceMeter/SourceMeter.h"
#include "../MultiMeter/MultiMeter.h"

using namespace std;


class Pelztier
{

public:
	Pelztier(SourceMeter &SourceM, int smuX, MultiMeter &MultiM);
	~Pelztier();
	void Initialize(int masterUD, int SourceMeterPad, int MultiMeterPad, string voltagelimit);

	void SetCurrent(string current);
	double GetTemperature();
	double GetSourceCurrent();
	vector<double> MeasureIV();

	double Constrain(double x, double a, double b);
	void TemperatureController(double temp_target);

	void WriteMeasurementToFile(vector<double> measurement, double temp, string path);
	void ITCurve(double minCurr, double maxCurr, double step);

	void Close();
private:

	double _value; // resistance in Ohm
	double _temperature; // temperature in °C
	double _current; // current
	double _voltage; // voltage
	SourceMeter _SourceM; // SourceMeter as current source for peltier
	int _smuX; // which Output of SourceMeter
	MultiMeter _MultiM; // MultiMeter for 4 wire measurement of resistance


};


#endif /* defined(____Pelztier__) */