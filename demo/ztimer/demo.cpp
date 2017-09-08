#include <iostream>
#include "ztimer.h"

using namespace std;

class CMyTimer : public CZTimer
{
    void OnTimer()
    {
	cout << "hello izuzi!" << endl;
    }
};

int main()
{
    CMyTimer timer;
    timer.Start();
    timer.Wait();	
    return 0;
}
