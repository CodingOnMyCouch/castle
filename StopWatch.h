// the game stopwatch
#ifndef STOPWATCH_H
#define STOPWATCH_H

#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#endif


#include <chrono>
#include <ctype.h>    
#include <iostream>
#include <iomanip>
//#include <unistd.h>   //for usleep which pauses the systems for milliseconds

#include "Game.h"

//usleep(1000);

// int currentTime()
inline auto currentTime()
{
	return std::chrono::high_resolution_clock::now();
	//long int time = static_cast<long int>(std::chrono::duration_cast<std::chrono::nanoseconds>(
	//		std::chrono::high_resolution_clock::now());
	//return time;
	//long int time = static_cast<long int>(std::chrono::high_resolution_clock::now());
	//return time;
}

// this is a singleton class
class  Timer {
public:
	// destructor
	~Timer() {}

	static Timer *instance()
	{
		if (!s_instance)
			s_instance = new Timer;
		return s_instance;
	}

	void reset()
	{ // reset the stopwatch to 0
		startTime = currentTime();  //currentTime() is a function member of the chrono class
	}

	unsigned int getElapsed()
	{ // get the elapsed time (in seconds)
		return std::chrono::duration_cast<std::chrono::seconds>(
			currentTime() - startTime
			).count(); // 1000000000.0;	
		//long int time = static_cast<long int>(std::chrono::duration_cast<std::chrono::nanoseconds>(
		//	std::chrono::high_resolution_clock::now().time_since_epoch()).count());
		//return time;
	}

	// setter functions
	void setHr(unsigned int h)
	{
		hr = h;
	}

	void setMin(unsigned int m)
	{
		min = m;
	}

    void setSec(unsigned int s)
	{
		sec = s;
	}

    void display()
	{
		std::cout << hr << ":" << min << ":" << sec << std::endl;
		//if (sec == 30)
			//std::cin.get();
	}

	void ResetDisplay()
	{
		this->setSec(0);
		this->setMin(0);
		this->setHr(0);
		std::cout << hr << ":" << min << ":" << sec << std::endl;
	}

	void getTimefromSeconds(unsigned int seconds)
	{
		unsigned hour, minute;
		hour = seconds / 3600;
		seconds = seconds % 3600;
		minute = seconds / 60;
		seconds = seconds % 60;
		this->setSec(seconds);
		this->setMin(minute);
		this->setHr(hour);
	}

	// getter functions
	unsigned getHr()
	{
		return hr;
	}

	unsigned getMin()
	{
		return min;
	}

	unsigned getSec()
	{
		return sec;
	}

	std::chrono::time_point<std::chrono::steady_clock> startTime;

private:

	unsigned int hr;
	unsigned int min;
	unsigned int sec;

	static Timer *s_instance;

	// constructor
	Timer(unsigned int h = 0, unsigned int m = 0, unsigned int s = 0) : hr(h), min(m), sec(s)
	{
		std::cout << "Creating the stopwatch object" << std::endl;
		//std::cin.get();
		//reset();
	}
	
};



#endif //STOPWATCH_H
