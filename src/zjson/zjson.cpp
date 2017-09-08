//
//  z_json.h
//  Created by izuzi@foxmail.com on 17/8/30.
//  And you can visit http://o-k.la to get more infomation.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdint.h>

#include "zjson.h"

using namespace std;
using namespace NS_CZJSON;


template<typename KTYPE>
CZValue CZValue::Get(KTYPE key)
{
    return (*this)[key];
}

template<typename VTYPE>
CZValue& CZValue::Set(string key, VTYPE value)
{
    (*this)[key] = value;
    return *this;
}

int CZValue::Size()
{
    return size();
}

string CZValue::ToString()
{
    if(isString())
        return asString();
    else
        return _writer.write(*this);
}

int CZValue::ToInt()
{
    return asInt();
}

unsigned int CZValue::ToUInt()
{
    return asUInt();
}

long long CZValue::ToInt64()
{
    return asInt64();
}

unsigned long long CZValue::ToUInt64()
{
    return asUInt64();
}

float CZValue::ToFloat()
{
    return asFloat();
}

double CZValue::ToDouble()
{
    return asDouble();
}

bool CZValue::ToBool()
{
    return asBool();
}

bool CZValue::IsObject()
{
    return isObject();
}

bool CZValue::IsArray()
{
    return isArray();
}

bool CZValue::IsString()
{
    return isString();
}

bool CZValue::IsNumeric()
{
    return isNumeric();
}

bool CZValue::IsDouble()
{
    return isDouble();
}

bool CZValue::IsIntegral()
{
    return isIntegral();
}

bool CZValue::IsUInt64()
{
    return isUInt64();
}

bool CZValue::IsUInt()
{
    return isUInt();
}

bool CZValue::IsInt64()
{
    return isInt64();
}

bool CZValue::IsInt()
{
    return isInt();
}

bool CZValue::IsBool()
{
    return isBool();
}

bool CZValue::IsNull()
{
    return isNull();
}


CZJson::CZJson() : _pDataNode(NULL), _iDataIndex(0)
{
    _ClearData();
}

CZJson::CZJson(const char* str) : _pDataNode(NULL), _iDataIndex(0)
{
    _ClearData();
    Parse(str);
}

CZJson::CZJson(CZValue& root) : _pDataNode(NULL), _iDataIndex(0)
{
    _ClearData();
    _root = root;
}

CZJson::CZJson(CZJson& obj) : _pDataNode(NULL), _iDataIndex(0)
{
    _ClearData();
    _root = obj.Root();
}

CZJson::~CZJson()
{
    _ClearData();
}

void CZJson::Clear()
{
    _ClearRoot();
    _ClearData();
}

bool CZJson::Parse(const char* str)
{
    bool bRetFalg = false;
    Json::Reader reader;
    _ClearRoot();
    
    if (reader.parse(str, _root))
    {
        bRetFalg = true;
    }
    
    return bRetFalg;
}

CZJson& CZJson::End(string key, WRITE_TYPE wt)
{
    if(_iDataIndex > 0)
    {
        if(key != "")
        {
            if(wt == DICT)
            {
                _root[key] = (*_pDataNode);
            }
            else
            {
                _root[key].append((*_pDataNode));
            }
        }
        else
        {
            if((*_pDataNode).IsObject() && wt == DICT)
            {
                Json::Value::Members members( (*_pDataNode).getMemberNames() );
                for ( Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it )
                {
                    std::string key = *it;
                    _root[key] = (*_pDataNode)[key];
                }
            }
            else if((*_pDataNode).IsArray())
            {
                _root = (*_pDataNode);
            }
            else
            {
                _root.append((*_pDataNode));
            }
        }
    }
    
    _ClearData();
    return *this;
} 

CZValue CZJson::Get(string key)
{
    Json::Value v = _root[key];
    
    CZValue _v(v);
    return _v;
}

string CZJson::ToString()
{
    return _root.ToString();
}

int CZJson::ToInt()
{
    return _root.ToInt();
}

unsigned int CZJson::ToUInt()
{
    return _root.ToUInt();
}

long long CZJson::ToInt64()
{
    return _root.ToInt64();
}

unsigned long long CZJson::ToUInt64()
{
    return _root.ToUInt64();
}

float CZJson::ToFloat()
{
    return _root.ToFloat();
}

double CZJson::ToDouble()
{
    return _root.ToDouble();
}

bool CZJson::ToBool()
{
    return _root.ToBool();
}

bool CZJson::IsObject()
{
    return _root.IsObject();
}

bool CZJson::IsArray()
{
    return _root.IsArray();
}

bool CZJson::IsString()
{
    return _root.IsString();
}

bool CZJson::IsNumeric()
{
    return _root.IsNumeric();
}

bool CZJson::IsDouble()
{
    return _root.IsDouble();
}

bool CZJson::IsIntegral()
{
    return _root.IsIntegral();
}

bool CZJson::IsUInt64()
{
    return _root.IsUInt64();
}

bool CZJson::IsUInt()
{
    return _root.IsUInt();
}

bool CZJson::IsInt64()
{
    return _root.IsInt64();
}

bool CZJson::IsInt()
{
    return _root.IsInt();
}

bool CZJson::IsBool()
{
    return _root.IsBool();
}

bool CZJson::IsNull()
{
    return _root.IsNull();
}

CZValue CZJson::Root()
{
    return _root;
}

int CZJson::Size()
{
    return _root.Size();
}


void CZJson::_ClearRoot()
{
    _root.clear();
}

void CZJson::_ClearData()
{
    if(_pDataNode!=NULL)
    {
        delete _pDataNode;
    }
    
    _pDataNode = NULL;
    _iDataIndex=0;
}

void CZJson::_ResetData()
{
    if(0 == _iDataIndex)
    {
        if(_pDataNode != NULL)
        {
            delete _pDataNode;
            _pDataNode = NULL;
        }
        
        if(_pDataNode == NULL)
        {
            _pDataNode = new CZValue();
        }
        
        _iDataIndex++;
    }
}

bool CZJson::_ParseFromFile(const char* filename)
{
    bool bRetFalg = false;
    Json::Reader reader;
    _root.clear();   
    
    std::ifstream is;  
    is.open (filename, std::ios::binary );    
    if(reader.parse(is, _root, false))  
    {  
        bRetFalg = true; 
    }  
    is.close();
    
    return bRetFalg;  
}
