//
//  z_json.h
//  Created by izuzi@foxmail.com on 17/8/30.
//  And you can visit http://o-k.la to get more infomation.
//

#ifndef z_json_h
#define z_json_h


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdint.h>
#include "json/json.h"

using namespace std;

namespace NS_CZJSON
{

class CZValue : public Json::Value
{
private:
    Json::FastWriter _writer;
	
public:
	CZValue() : Json::Value(){}
	
	CZValue(Json::Value v) : Json::Value(v){}
	
	template<typename KTYPE>
    CZValue Get(KTYPE key);
    
	template<typename VTYPE>
    CZValue& Set(string key, VTYPE value);
    
    int Size();
	
    string ToString();
    int ToInt();
    unsigned int ToUInt();
    long long ToInt64();
    unsigned long long ToUInt64();
    float ToFloat();
    double ToDouble();
    bool ToBool();
    
    bool IsObject();
    bool IsArray();
    bool IsString();
    bool IsNumeric();
    bool IsDouble();
    bool IsIntegral();
    bool IsUInt64();
    bool IsUInt();
    bool IsInt64();
    bool IsInt();
    bool IsBool();
    bool IsNull();
};	
		
enum WRITE_TYPE
{
	DICT = 1,
	LIST = 2,
};

class CZJson
{
private:
	CZValue _root; //int, string, object, array      
    CZValue* _pDataNode;
    int _iDataIndex;
	
public:
    CZJson();
    CZJson(const char* str);
    CZJson(CZValue& root);
    CZJson(CZJson& obj);
    ~CZJson();
    
    void Clear();

    bool Parse(const char* str);
    
    template<typename VTYPE>
    CZJson& Set(string key, VTYPE value)
    {
        return _Set(value, key);
    }

    CZJson& Set(string key, CZJson value)
    {
        return Set(key, value.Root());
    }
    
    template<typename VTYPE>
    CZJson& Set(VTYPE value)
    {
        return _Set(value);
    }
    
    CZJson& Set(CZJson value)
    {
        return _Set(value.Root());
    }
    
    CZJson& End(string key = "", WRITE_TYPE wt = DICT);
	
    CZValue Get(string key);
    
    template<typename KTYPE>
    CZJson operator[](KTYPE key)
    {
        if(_root.IsNull())
        {
            return *this;
        }
        
        CZValue value = _root[key];
        
        if(value.IsNull())
        {
            return *this;
        }
        
        CZJson obj(value);
        
        return obj;
    }
	
    string ToString();
    int ToInt();
    unsigned int ToUInt();
    long long ToInt64();
    unsigned long long ToUInt64();
    float ToFloat();
    double ToDouble();
    bool ToBool();
    
    bool IsObject();
    bool IsArray();
    bool IsString();
    bool IsNumeric();
    bool IsDouble();
    bool IsIntegral();
    bool IsUInt64();
    bool IsUInt();
    bool IsInt64();
    bool IsInt();
    bool IsBool();
    bool IsNull();
    
    CZValue Root();
    
    int Size();

private:
    void _ClearRoot();
    void _ClearData();
    void _ResetData();
    
    template<typename VTYPE>
    CZJson& _Set(VTYPE value, string key="")
    {
        _ResetData();
        
        if(key != "")
        {
            (*_pDataNode)[key] = value;
        }
        else
        {
            (*_pDataNode).append(value);	
        }
        
        return *this;
    }
	
    CZJson& _Set(CZValue value, string key = "")
    {
        _ResetData();
        
        if(key != "")
        {
            (*_pDataNode)[key] = value;
        }
        else
        {
            if(value.IsArray() || value.IsObject())
            {
                (*_pDataNode) = value;
            }
            else
            {
                (*_pDataNode).append(value);
            }
        }
        
        return *this;
    }

    bool _ParseFromFile(const char* filename);
	
};

}

#endif /* z_json_h */



