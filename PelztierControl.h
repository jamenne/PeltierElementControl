/*

PelztierControl.h


Created by Janine Müller on 12.10.2016


*/

#ifndef ____Pelztier__
#define ____Pelztier__

#include <string>

#include "../SourceMeter/SourceMeter.h"
#include "../MultiMeter/MultiMeter.h"




class Pelztier
{

public:
	Pelztier(SourceMeter &SourceM, int smuX, MultiMeter &MultiM);
	~Pelztier();
	MultiMeter& GetMultiMeter();
	SourceMeter& GetSourceMeter();
	int GetSourceMeterChannel();
	void Initialize(int masterUD, int SourceMeterPad, int MultiMeterPad, const std::string& voltagelimit);
	void Close();

	void SetSourceCurrent(string current);
	double GetSourceCurrent();
	double GetTemperature();
	double MeasureCurrent();
	double MeasureVoltage();
	vector<double> MeasureIV();

	double Constrain(double x, double a, double b);
	void TemperatureController(double temp_target);

	void WriteMeasurementToFile(vector<double> measurement, double temp, const std::string& path);
	void ITCurve(double minCurr, double maxCurr, double step);


private:

	SourceMeter& _SourceM; // SourceMeter as current source for peltier
	int _smuX; // which Output of SourceMeter
	MultiMeter& _MultiM; // MultiMeter for 4 wire measurement of resistance
	double _value; // resistance in Ohm
	double _temperature; // temperature in °C
	double _current; // source current

};


#endif /* defined(____Pelztier__) */