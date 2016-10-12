/*

SourecMeter.h


Created by Janine Müller on 12.10.2016


*/

#ifndef ____Pelztier__
#define ____Pelztier__

using namespace std;


class Pelztier
{

public:
	Pelztier();
	~Pelztier();
	static Pelztier* Instance();

private:

	double _value;
	double _temperature;
	double _current;
	double _voltage;


};


#endif /* defined(____Pelztier__) */