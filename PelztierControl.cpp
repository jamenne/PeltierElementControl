/*

PelztierControl.cpp


Created by Janine Müller on 07.10.2016

*/

#include <gpib/ib.h>
#include <gpib/gpib_user.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include "time.h"
#include <sstream>
#include <cmath>

#include "PelztierControl.h"

using namespace std;

//---------------------Pelztier class---------------------//

// Constructor
Pelztier::Pelztier(SourceMeter &SourceM, int smuX, MultiMeter &MultiM) :
	_SourceM(SourceM), 
	_smuX(smuX), 
	_MultiM(MultiM),
	_value(314), //resistance in Ohm
	_temperature(314), // temperature in °C
	_current(314), // current in A
	_LogFile()
{
	
};

//Destructor
Pelztier::~Pelztier(){

};

MultiMeter& Pelztier::GetMultiMeter(){

	return this->_MultiM;
}

SourceMeter& Pelztier::GetSourceMeter(){

	return this->_SourceM;
}

LogDev& Pelztier::GetLogFile(){

	return this->_LogFile;
}

int Pelztier::GetSourceMeterChannel(){

	return this->_smuX;
}

void Pelztier::Initialize(const std::string& voltagelimit){

	//this->_SourceM.Initialize(masterUD, SourceMeterPad);
	this->_SourceM.SelectCurrentFunction(this->_smuX);
	this->_SourceM.SetVoltageLimit(this->_smuX,voltagelimit);
	this->_SourceM.SetOutputOnOff(this->_smuX,true);

	this->_MultiM.Set4WireFunction();
	this->_MultiM.SetAutorange4Wire();
	this->_MultiM.SetTriggerContinously();

	this->_LogFile.Initialize("PelztierControl");
	this->_LogFile.WriteString("date\ttemp\tcurrent\tvoltage");

}

void Pelztier::Close(){
	this->SetSourceCurrent("0");
	_SourceM.SetOutputOnOff(this->_smuX,false);

}

void Pelztier::SetSourceCurrent(const string current){

	_SourceM.SetSourceCurrent(this->_smuX, current);
	stringstream ss;
	ss << current;

	ss >> _current;

}

double Pelztier::GetSourceCurrent(){

	_current = _SourceM.GetSourceCurrent(_smuX);

	return _current;
}

// Steinhart-Hart equiation: 1/T = 1/T_0 + 1/B*ln(R/R_0)
// Temperatures in K
// R_0: resistance at T_0
// B-paramater can also be written as: ln(R)= B/T + ln(R_inf). 
// This can be used to convert the function of resistance vs. temperature of a thermistor into a linear function of ln(R) vs 1/T.
// The average slope of this function will then yield an estimate of the value of the B parameter.
//
// B-Parameter for Hamamatsu cooled MPPC S13362-1350DG: B(25/50)=3410K
// Thermistor resistance at 25°C: 9kOhm
// 25°C = 298.15K
// T = B/(ln(R/R_0)+B/T_0)
//
double Pelztier::GetTemperature(){

	this->_value = _MultiM.Fetch();

	this->_temperature = 3410/(log(this->_value/9000)+3410/298.15)-273.15; // returns temperature in °C

	cout << "Measured temperature to: " << this->_temperature << "°C" << endl;

	return this->_temperature;
}

double Pelztier::MeasureCurrent(){

	return this->_SourceM.MeasureI(this->_smuX);
}

double Pelztier::MeasureVoltage(){

	return this->_SourceM.MeasureV(this->_smuX);
}

vector<double> Pelztier::MeasureIV(){

	vector<double> iv(2,0);

	iv = _SourceM.MeasureIV(_smuX);

	return iv;

}

// Saftey function, that target temperature is not set to temp out of bounds of MPPC (-25°C to 25°C) or current over 1A
double Pelztier::Constrain(double x, double a, double b){

	if (x<a) return a;

	else if(x>b) return b;

	else return x;
}

// Controller to control the temperature of the peltier element, regulates the current with an integral function
void Pelztier::TemperatureController(double temp_target){

	temp_target = Constrain(temp_target, -25, 25);
	double temp;

	vector<double> TempDiff(10,0); // vector for integral
	double temp_diff;

	uint index = 0;
	double integral = 0;

	double r_fac = 20e-6;

	double current = 0;

	stringstream ss;

	fstream file;

	time_t sec = time(NULL);

	tm *uhr = localtime(&sec);

	stringstream path;

	path << "data/TContollerMeasurement_" << uhr->tm_year-100 << uhr->tm_mon+1 << uhr->tm_mday << "-" << uhr->tm_hour << uhr->tm_min << uhr->tm_sec << ".txt";

	vector<double> measure(2,0);

	while(true){

		// Read MultiMeter and get temperature
		temp = this->GetTemperature();
		
		measure = this->MeasureIV();

		file.open(path.str().c_str(), fstream::in | fstream::out | fstream::app);

		file << temp << "\t" << measure[0] << "\t" << measure[1] << endl;

		file.close();

		// integration
		temp_diff = temp_target - temp;

		integral = integral - TempDiff[index];
		TempDiff[index] = temp_diff;
		integral = integral + TempDiff[index];
		cout << "integral:\t" << integral << endl;

		if (index < TempDiff.size()-1)
		{
			index++;
		}

		else index = 0;

		// peltier control

		// max integral 20*50 (tempdiff) = 7500 
		// max änderung des stroms 1mA gewünscht
		// -> x=10-6
		current = current - r_fac * integral;
		cout << "Current:\t" << current << endl;

		current = Constrain(current, 0, 0.5);
		cout << "Current nach Constrain:\t" << current << endl;

		ss << current;

		this->SetSourceCurrent(ss.str());

		ss.str("");

		// sleep for 1 second
		sleep(1);
	}

}

// Controller to control the temperature of the peltier element, regulates the current with an integral function
// no while(true) loop
void Pelztier::OneTempControl(vector<double> &TempDiff, double &integral, int &index, double &current, double temp_target){

	temp_target = Constrain(temp_target, -25, 25);

	double temp;

	double temp_diff;

	double r_fac = 20e-6;

	stringstream ss;

	vector<double> measure(2,0);

	//---------This is actually the loop--------//

	// Read MultiMeter and get temperature
	temp = this->GetTemperature();
	
	measure = this->MeasureIV();

	// save measurement in LogFile
	this->_LogFile.WriteDoubleAndVector(temp, measure);

	// integration
	temp_diff = temp_target - temp;

	integral = integral - TempDiff[index];
	TempDiff[index] = temp_diff;
	integral = integral + TempDiff[index];
	cout << "integral:\t" << integral << endl;

	if (index < TempDiff.size()-1)
	{
		index++;
	}

	else index = 0;

	// peltier control

	// max integral 20*50 (tempdiff) = 7500 
	// max änderung des stroms 1mA gewünscht
	// -> x=10-6
	current = current - r_fac * integral;
	cout << "Current:\t" << current << endl;

	current = Constrain(current, 0, 0.5);
	cout << "Current nach Constrain:\t" << current << endl;

	ss << current;

	this->SetSourceCurrent(ss.str().c_str());

	ss.str("");

}


void Pelztier::WriteMeasurementToFile(vector<double> measurement, double temp, const std::string& path){

	fstream file;
	file.open(path.c_str(), fstream::in | fstream::out | fstream::app);

	file << measurement[0] << "\t" << measurement[1] << "\t" << temp << endl;

	file.close();

}

void Pelztier::ITCurve(double minCurr, double maxCurr, double step){

	time_t sec = time(NULL);

	tm *uhr = localtime(&sec);

	stringstream path;

	path << "data/IVTMeasurement_" << uhr->tm_year-100 << uhr->tm_mon+1 << uhr->tm_mday << "-" << uhr->tm_hour << uhr->tm_min << uhr->tm_sec << ".txt";

	double Curr = minCurr;

	vector<double> measure(2,0);
	double i = 0, v = 0; 
	double temp=0;

	while(Curr <= maxCurr){
		stringstream ss;
		ss << Curr;

		this->SetSourceCurrent(ss.str());

		sleep(5*60);

		for (int j = 0; j < 5; j++)
		{
			measure = this->MeasureIV();
			i += measure[0];
			v += measure[1];
			temp += this->GetTemperature();
			sleep(5);

		}

		measure[0] = i/5;
		measure[1] = v/5;
		temp = temp/5;
		
		this->WriteMeasurementToFile(measure, temp, path.str());

		Curr = Curr + step;
		i = 0;
		v = 0;
		temp = 0;
	}

}












