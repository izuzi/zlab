//
//  zutil.h
//  zlab
//
//  Created by izuzi on 16/10/9.
//  Copyright@2016 izuzi. All rights reserved.
//

#ifndef z_util_h
#define z_util_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdint.h>

using namespace std;


template<typename SRCTYPE>
string ToString(SRCTYPE src)
{
    ostringstream stream;
    stream << src;
    return stream.str();
}

#define _ZLAB_MAX_STR_BUF_SIZE 10240
bool StrSplit(const char* str, const char* pattern, std::vector<std::string>& result)
{
    char* beg = (char*)str;
    char* cur = beg;
    
    int len_sub = 0;
    int len_ptn = strlen(pattern);
    char tmp[_ZLAB_MAX_STR_BUF_SIZE] = {'\0'};
    
    while((*cur) != '\0')
    {
        if(strncasecmp(pattern, cur, len_ptn) == 0)
        {
            len_sub = cur - beg;
            
            if(len_sub > _ZLAB_MAX_STR_BUF_SIZE - 1)
            {
                return false;
            }
            else if(len_sub == 0)
            {
                result.push_back("");
            }
            else
            {
                strncpy(tmp, beg, len_sub);
                result.push_back(tmp);
            }
            
            cur = cur + len_ptn;
            beg = cur;
        }
        else
        {
            cur++;
        }
        
        memset(tmp, 0, _ZLAB_MAX_STR_BUF_SIZE);
    };
    
    len_sub = cur - beg;
    if(len_sub > _ZLAB_MAX_STR_BUF_SIZE - 1)
    {
        return false;
    }
    else if(len_sub == 0)
    {
        result.push_back("");
        
    }
    else
    {
        strncpy(tmp, beg, len_sub);
        result.push_back(tmp);
    }
    
    return true;
}

#endif /* z_util_h */
