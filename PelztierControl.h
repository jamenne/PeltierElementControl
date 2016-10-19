/*

PelztierControl.h


Created by Janine Müller on 12.10.2016


*/

#ifndef ____Pelztier__
#define ____Pelztier__

#include <string>
#include <vector>

#include "../SourceMeter/SourceMeter.h"
#include "../MultiMeter/MultiMeter.h"
#include "../LogFileDev/LogDev.h"




class Pelztier
{

public:
	Pelztier(SourceMeter &SourceM, int smuX, MultiMeter &MultiM);
	~Pelztier();
	MultiMeter& GetMultiMeter();
	SourceMeter& GetSourceMeter();
	LogDev& GetLogFile();
	int GetSourceMeterChannel();
	void Initialize(const std::string& voltagelimit);
	void Close();

	void SetSourceCurrent(const std::string current);
	double GetSourceCurrent();
	double GetTemperature();
	double MeasureCurrent();
	double MeasureVoltage();
	vector<double> MeasureIV();

	double Constrain(double x, double a, double b);
	void TemperatureController(double temp_target);
	void OneTempControl(std::vector<double> &TempDiff, double &integral, int &index, double &current, double temp_target);

	void WriteMeasurementToFile(std::vector<double> measurement, double temp, const std::string& path);
	void ITCurve(double minCurr, double maxCurr, double step);


private:

	SourceMeter& _SourceM; // SourceMeter as current source for peltier
	int _smuX; // which Output of SourceMeter
	MultiMeter& _MultiM; // MultiMeter for 4 wire measurement of resistance
	double _value; // resistance in Ohm
	double _temperature; // temperature in °C
	double _current; // source current
	LogDev _LogFile;

};


#endif /* defined(____Pelztier__) */