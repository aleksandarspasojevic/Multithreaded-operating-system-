/*
 * thrMain.cpp
 *
 *  Created on: Sep 22, 2021
 *      Author: OS1
 */


#include <iostream.h>
#include <stdlib.h>
#include "thrMain.h"

#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "PCB.h"
#include "semaphor.h"
#include "IVTEntry.h"
#include "event.h"


int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	lock;
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlock;
		return res;
}


/*
	Test: Niti maxStack velicine
*/

const int n = 2;

void tick(){}

class TestThread : public Thread
{
public:

	TestThread(): Thread(32768,2) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{

	int buffer=2;

	for(int i=0;i<32000;i++)
	{
		buffer = 4096/2048;
		for (int j = 0; j < 1024; j++)
		{
			buffer = buffer*2;
			if(buffer%2)
				buffer = 2;
		}
	}

}


int userMain(int argc, char** argv)
{
	syncPrintf("Test starts: %d threads.\n",n);
	int i;
	TestThread threads[n];
	for(i=0;i<n;i++)
	{
		threads[i].start();
	}
	for(i=0;i<n;i++)
	{
		threads[i].waitToComplete();
		syncPrintf("%d. Done!\n",Thread::getThreadById(i+10)->getId());
	}
	syncPrintf("Test ends.\n");
	return 0;
}


