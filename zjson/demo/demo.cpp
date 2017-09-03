#include <iostream>
#include "zjson.h"

using namespace std;
using namespace NS_CZJSON;

int main()
{
    //case 1
    CZJson zjson_obj_1;
    const char* str = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
    if (zjson_obj_1.Parse(str))
    {  
        std::string upload_id = zjson_obj_1["uploadid"].ToString();
        int code = zjson_obj_1["code"].ToInt();
        cout << upload_id << ", " << code << endl;
    }
    
    //case 2
    CZJson zjson_obj_2;
    CZJson zjson_item;
    zjson_obj_2.Set("a", 1).Set("b", 2).Set("c", 3).End("key-1", LIST);
    zjson_item.Set(10).Set(20).Set(30).End();
    zjson_obj_2.Set(zjson_item).End("key-2");
    cout << zjson_obj_2.ToString() << endl;

    return 0;
}
