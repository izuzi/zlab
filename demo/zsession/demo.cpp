#include <iostream>
#include "zsession.h"

using namespace std;

void show(uint64_t u64Key, int & iValue, void * args = NULL)
{
    char* str = (char*)args;
    cout << "key: " << u64Key << ", value: " << iValue << ", args: " << str << endl;
}

int main()
{
    CZSession<int> sess(10000, 3);
    
    sess.Add(1, 123);
    sess.Add(2, 456);
    sess.Add(3, 789);
    
    sleep(5);
    
    char str[] = "hello";
    
    sess.Timeout(show, str, false);

    return 0;
}
