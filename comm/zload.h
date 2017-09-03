//
//  zload.h
//  zlab
//
//  Created by izuzi on 16/10/9.
//  Copyright@2016 izuzi. All rights reserved.
//

#ifndef z_load_h
#define z_load_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdint.h>
#include "zutil.h"

using namespace std;

class CZLoad
{
private:
    ifstream m_file_handle;

public:
    CZLoad(string strPath)
    {
        if(false == Open(strPath))
        {
            throw "#ERR, CZLoad.Open is failed!";
        }
    }
    
    ~CZLoad()
    {
        Close();
    }
    
    
    bool Read(vector< vector<string> >& vecData, string strPatten = "\t")
    {
        vecData.clear();
        
        string strLine;
        
        while(getline(m_file_handle, strLine))
        {
            std::vector<std::string> vecTmp;
            if(!StrSplit(strLine.c_str(), strPatten.c_str(), vecTmp))
            {
                return false;
            }

            vecData.push_back(vecTmp);
        }
        
        return true;
    }
    
private:
    bool Open(string strPath)
    {
        m_file_handle.open(strPath.c_str(), std::ios::in);
        
        if(!m_file_handle.is_open())
        {
            return false;
        }
        
        return true;
    }
    
    void Close()
    {
        m_file_handle.close();
    }
    
};

#endif /* z_load_h */




