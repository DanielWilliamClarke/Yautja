#pragma once

class Food
{
private:
	int timeInPlay_;
public:
	int getTIP();
	void setTIP(int newTimeInPlay);
	Food() {timeInPlay_ = 0;}
};