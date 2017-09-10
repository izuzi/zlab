#include <iostream>
#include "zhashtable.h"

using namespace std;

void show(uint64_t u64Key, int & iValue)
{
    cout << "key: " << u64Key << ", value: " << iValue << endl;
}

int main()
{
    CZHashTable<int> htable(10000, 1234);

    htable.Add(100, 123);
    htable.Add(100, 456);
    htable.Add(200, 789);
    
    int x = 0;
    htable.Get(100, x);
    cout << "key: " << 100 << ", value: " << x << endl;
    
    cout << "---------" << endl;
    
    htable.Travel(show);
    
    cout << "---------" << endl;
    
    htable.Update(100, 456);
    htable.Travel(show);
    
    cout << "---------" << endl;
    
    htable.Delete(100);
    htable.Travel(show);

    return 0;
}
