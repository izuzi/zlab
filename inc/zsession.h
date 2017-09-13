//
//  zsession.h
//
//  Created by izuzi on 17/09/08.
//  Copyright@2017 izuzi@foxmail.com All rights reserved.
//

#ifndef Z_SESSION_H_
#define Z_SESSION_H_

#include<iostream>
#include<cstdlib>
#include<sys/time.h>
#include<unistd.h>

#include "zhashtable.h"

using namespace std;

template<typename TYPEVALUE>
class CZSession
{
    typedef void (*CALLBACK_FUNC_PTR)(uint64_t u64Key, TYPEVALUE & tValue, void * args);
    
private:
    struct stSessionNode{
        uint64_t u64Timestamp;
        uint64_t u64Timeout;
        TYPEVALUE tValue;
    };
    
    struct stSessionExt
    {
        CZSession<TYPEVALUE> * _this;
        CALLBACK_FUNC_PTR callback;
        void * args;
        bool is_del;
    };

    
    CZHashTable<stSessionNode>* m_pHashTable;
    uint64_t m_u64Timeout;
    
public:
    CZSession(uint32_t u32DataNum, uint64_t u64Timeout, key_t tShmKey = 0) :
            m_pHashTable(NULL), m_u64Timeout(0)
    {
        Init(u32DataNum, u64Timeout, tShmKey);
    }
    
    CZSession() : m_pHashTable(NULL), m_u64Timeout(0)
    {
        
    }
    
    ~CZSession()
    {
        if(m_pHashTable)
        {
            delete m_pHashTable;
            m_pHashTable = NULL;
        }
    }
    
    void Init(uint32_t u32DataNum, uint64_t u64Timeout, key_t tShmKey = 0)
    {
        m_u64Timeout = u64Timeout;
        m_pHashTable = new CZHashTable< stSessionNode >(u32DataNum, tShmKey);
        if(m_pHashTable == NULL)
        {
            cout << "#ERR! init CZSession failed." << endl;
            throw "#ERR! init CZSession failed.";
        }
    }
    
    bool Exists(uint64_t u64Key)
    {
        if(m_pHashTable == NULL) return -1;
        return m_pHashTable->Exists(u64Key);
    }

    int Get(uint64_t u64Key, TYPEVALUE & tValue)
    {
        if(m_pHashTable == NULL) return -1;
        
        stSessionNode oSessionNode;
        int iRet = m_pHashTable->Get(oSessionNode);
        if(0 != iRet)
        {
            return iRet;
        }
        
        tValue = oSessionNode.tValue;
        return 0;
    }
    
    int Add(uint64_t u64Key, const TYPEVALUE & tValue, uint64_t u64Timeout = 0)
    {
        if(m_pHashTable == NULL) return -1;
        
        stSessionNode oSessionNode;
        oSessionNode.u64Timestamp = time(NULL);
        oSessionNode.u64Timeout = u64Timeout > 0 ? u64Timeout : m_u64Timeout;
        oSessionNode.tValue = tValue;
        
        return m_pHashTable->Add(u64Key, oSessionNode);
        
    }
    
    int Update(uint64_t u64Key, const TYPEVALUE & tValue, uint64_t u64Timeout = 0)
    {
        if(m_pHashTable == NULL) return -1;
        
        stSessionNode oSessionNode;
        oSessionNode.u64Timestamp = time(NULL);
        oSessionNode.u64Timeout = u64Timeout > 0 ? u64Timeout : m_u64Timeout;
        oSessionNode.tValue = tValue;
        
        return m_pHashTable->Update(u64Key, oSessionNode);
        
    }
    
    int Delete(uint64_t u64Key)
    {
        if(m_pHashTable == NULL) return -1;
        
        return m_pHashTable->Delete(u64Key);
    }

    
    uint32_t LastSize()
    {
        if(m_pHashTable == NULL) return -1;
        
        return m_pHashTable->LastSize();
    }
    
    void Clear()
    {
        if(m_pHashTable == NULL) return;
        
        return m_pHashTable->Clear();
    }
    
    int Timeout(CALLBACK_FUNC_PTR callback, void * args = NULL, bool bIsDelExpired=true)
    {
        if(m_pHashTable == NULL) return -1;
        
        stSessionExt oSessionExt;
        oSessionExt._this = this;
        oSessionExt.callback = callback;
        oSessionExt.args = args;
        oSessionExt.is_del = bIsDelExpired;
        
        m_pHashTable->Travel(_Timeout, &oSessionExt);
        
        return 0;
    }

private:
    static void _Timeout(uint64_t u64Key, stSessionNode & oDataNode, void * pExt)
    {
        if(pExt == NULL) return;

        stSessionExt * _pExt = (stSessionExt*) pExt;

        
        if(oDataNode.u64Timeout > 0 && (time(NULL) - oDataNode.u64Timestamp) >= oDataNode.u64Timeout )
        {
            _pExt->callback(u64Key, oDataNode.tValue, _pExt->args);
            
            if(_pExt->is_del) _pExt->_this->Delete(u64Key);
        }
    }
};

#endif /* zsession_h */
