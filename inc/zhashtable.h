//
//  zhashtable.h
//
//  Created by izuzi on 17/09/08.
//  Copyright@2017 izuzi@foxmail.com All rights reserved.
//

#ifndef Z_HASH_TABLE_H_
#define Z_HASH_TABLE_H_

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<sys/shm.h>
using namespace std;

#define __ZHASH_STEP_NUM_ 50
#define __ZHASH_EXPAND_TIMES_ 2

template<typename TYPEVALUE>
class CZHashTable
{
private:
    void * m_pDataBuf;
    uint32_t m_u32BufSize;
    uint32_t m_u32DataNum;
    bool m_bIsShm;
    uint32_t m_u32StepTable[__ZHASH_STEP_NUM_];
    uint32_t m_u32PosTable[__ZHASH_STEP_NUM_];
    uint32_t m_u32TotleSize;
    uint32_t m_u32UsedSize;
    
    uint32_t m_u32MaxCol;
    
    struct stHashNode{
        uint64_t u64Key;
        TYPEVALUE tValue;
    };
    
public:
    CZHashTable(uint32_t u32DataNum, key_t tShmKey = 0)
    {
        Init(u32DataNum, tShmKey);
    }
    
    CZHashTable()
    {
        
    }
    
    ~CZHashTable()
    {
        Clear();
        
        if(m_pDataBuf && !m_bIsShm)
        {
            delete [] (stHashNode*)m_pDataBuf;
            m_pDataBuf = NULL;
        }
    }
    
    void Init(uint32_t u32DataNum, key_t tShmKey = 0)
    {
        m_pDataBuf = NULL;
        m_u32BufSize = 0;
        m_u32DataNum = 0;
        m_bIsShm = false;
        memset(m_u32StepTable, 0, __ZHASH_STEP_NUM_ * sizeof(uint32_t));
        memset(m_u32PosTable, 0, __ZHASH_STEP_NUM_ * sizeof(uint32_t));
        m_u32TotleSize = 0;
        m_u32UsedSize = 0;
        m_u32MaxCol = 0;
        
        int iErrNoInitStepTable = InitStepTable(u32DataNum);
        if(iErrNoInitStepTable != 0)
        {
            cout << "#ERR! init CZHashTable failed in InitStepTable(), iErrNo: " << iErrNoInitStepTable << endl;
            throw "#ERR! init CZHashTable failed in InitStepTable().";
        }
        
        uint32_t u32BufSize = CalBufSize();
        
        int iErrNoInitDataBuf = InitDataBuf(u32BufSize, tShmKey);
        if(iErrNoInitDataBuf != 0)
        {
            cout << "#ERR! init CZHashTable failed in InitDataBuf(), iErrNo: " << iErrNoInitDataBuf << endl;
            throw "#ERR! init CZHashTable failed in InitDataBuf.";
        }

        Clear();
    }
    
    bool Exists(uint64_t u64Key)
    {
        stHashNode* pHashNode = NULL;
        if(_Access(u64Key, &pHashNode) == -1) return false;
        
        return true;
    }
    
    int Get(uint64_t u64Key, TYPEVALUE & tValue)
    {
        stHashNode* pHashNode = NULL;
        if(_Access(u64Key, &pHashNode) == -1) return -1;
        
        tValue = pHashNode->tValue;
        
        return 0;
    }
    
    int Add(uint64_t u64Key, const TYPEVALUE & tValue)
    {
        if(Exists(u64Key)) return -1;
        
        stHashNode* pHashNode = NULL;
        if(_Access(u64Key, &pHashNode, false) == -1) return -1;
        
        if(pHashNode->u64Key != 0) return -1;
        
        pHashNode->u64Key = u64Key;
        pHashNode->tValue = tValue;
        
        m_u32UsedSize++;
        
        return 0;
    }
    
    int Update(uint64_t u64Key, const TYPEVALUE & tValue)
    {
        stHashNode* pHashNode = NULL;
        if(_Access(u64Key, &pHashNode) == -1) return -1;
        
        pHashNode->tValue = tValue;
        
        return 0;
    }
    
    int Delete(uint64_t u64Key)
    {
        stHashNode* pHashNode = NULL;
        if(_Access(u64Key, &pHashNode) == -1) return -1;
        
        pHashNode->u64Key = 0;
        
        m_u32UsedSize--;
        
        return 0;
    }

    uint32_t LastSize()
    {
        return m_u32TotleSize - m_u32UsedSize;
    }
    
    void Clear()
    {
        if(m_pDataBuf)
        {
            memset(m_pDataBuf, 0, m_u32BufSize);
        }
        
        m_u32UsedSize = 0;
    }
    
    void Travel(void (*fn)(uint64_t u64Key, TYPEVALUE & tValue))
    {
        for(int i=0; i< m_u32DataNum; i++)
        {
            stHashNode* _pHashNode = (stHashNode*)m_pDataBuf + i;
            if(_pHashNode && _pHashNode->u64Key != 0)
            {
                fn(_pHashNode->u64Key, _pHashNode->tValue);
            }
        }
    }
    
    void Travel(void (*fn)(uint64_t u64Key, TYPEVALUE & tValue, void * pExt), void * pExt=NULL)
    {
        for(int i=0; i< m_u32DataNum; i++)
        {
            stHashNode* _pHashNode = (stHashNode*)m_pDataBuf + i;
            if(_pHashNode && _pHashNode->u64Key != 0)
            {
                fn(_pHashNode->u64Key, _pHashNode->tValue, pExt);
            }
        }
    }
    
private:
    uint32_t CalMaxCol(uint32_t u32DataNum)
    {
        return u32DataNum * __ZHASH_EXPAND_TIMES_ / __ZHASH_STEP_NUM_;
    }
    
    uint32_t CalKeyHash(uint64_t u64Key, uint32_t u32Step)
    {
        return (u64Key + m_u32MaxCol) % m_u32StepTable[u32Step];
    }
    
    int _Access(uint64_t u64Key, stHashNode** pHashNode, bool bIsNeedFound=true)
    {
        uint32_t u32Hash;
        stHashNode* _pHashNode = NULL;
 
        for(uint32_t u32Step=0; u32Step<__ZHASH_STEP_NUM_; u32Step++)
        {
            u32Hash = CalKeyHash(u64Key, u32Step);
            
            _pHashNode = (stHashNode*)FindHashNode(u32Step, u32Hash);
            
            if(_pHashNode == NULL) return -1;
            
        
            if(bIsNeedFound == true && _pHashNode->u64Key == u64Key)
            {
                *pHashNode = _pHashNode;
                return 0;
            }
            
            if(bIsNeedFound == false && _pHashNode->u64Key == 0)
            {
                *pHashNode = _pHashNode;
                return 0;
            }
        }
        return -1;
    }
    
    int InitStepTable(uint32_t u32DataNum)
    {
        m_u32DataNum = u32DataNum;

        uint32_t u32CheckNum = 0;
        m_u32MaxCol = CalMaxCol(m_u32DataNum);
        
        //采用 6n+1 6n-1 素数集中原理
        if(m_u32MaxCol < 5) return -1;
        
        int t,m,n,p;
        int i,j,k;
        int a,b;
        
        uint32_t u32TableRowId=0;
        
        for(t=m_u32MaxCol/6; t>=0; t--)
        {
            if(u32TableRowId >= __ZHASH_STEP_NUM_)
            {
                break;
            }
            //i,j的值为是否进行验证的标志，也是对应的6t-1和6t+1的素性标志
            i=1; j=1; k=t%10;
            m=6*t;
            
            
            //此处是简单验证6*t-1,6*t+1 是不是素数，借以提高素数纯度
            if(((k-4)==0)||((k-9)==0)||((m+1)%3==0)) j=0;

            //先通过初步判断去除末尾是5，及被3整除的数
            if(((k-6)==0)||((m-1)%3==0)) i=0;
            
            for(p=1;p*6<=sqrt(m+1)+2;p++ )
            {
                //将6*p-1和6*p+1看作伪素数来试除
                n=p*6;
                k=p%10;
                
                //同样此处a,b的值也是用来判断除数是否为素数提高除数的素数纯度
                a=1; b=1;
                
                if(((k-4)==0)||((k-9)==0)) a=0;
                if(((k-6)==0)) b=0;
                
                //如果i非零就对m-1即所谓6*t-1进行验证，当然还要看除数n+1,n-1,素性纯度
                if(i)
                {
                    if(a) if((m-1)%(n+1)==0) i=0; //一旦被整除就说明不是素数故素性为零即将i 赋值为零
                    if(b) if((m-1)%(n-1)==0) i=0;
                }
                
                //如果j非零就对m+1即所谓6*t+1进行验证，当然还要看除数n+1,n-1,素性纯度
                if(j)
                {
                    if(a) if((m+1)%(n+1)==0) j=0; //一旦被整除就说明不是素数故素性为零即将j 赋值为零
                    if(b) if((m+1)%(n-1)==0)  j=0;
                }
                
                //如果已经知道6*t-1,6*t+1都不是素数了那就结束试除循环
                if((i+j)==0) break;
            }
            
            if(j)
            {
                u32CheckNum += m+1;
                m_u32StepTable[ u32TableRowId++ ] = m+1;
                if(u32TableRowId >= __ZHASH_STEP_NUM_) break;
            }
            
            if(i)
            {
                u32CheckNum += m-1;
                m_u32StepTable[ u32TableRowId++ ]=m-1;
                if(u32TableRowId >= __ZHASH_STEP_NUM_) break;
            }
        }

        if(u32CheckNum < u32DataNum) return -2;
        
        for(int i=0; i<__ZHASH_STEP_NUM_; i++)
        {
            if(i==0)
            {
                m_u32PosTable[i] = 0;
            }
            else
            {
                m_u32PosTable[i] = m_u32PosTable[i-1] + m_u32StepTable[i-1];
            }
        }
        
        return 0;
    }
    
    int CalBufSize()
    {
        m_u32TotleSize=0;
        for(int i=0; i<__ZHASH_STEP_NUM_; i++)
        {
            m_u32TotleSize += m_u32StepTable[i];
        }
        
        m_u32BufSize = sizeof(stHashNode) * m_u32TotleSize;
 
        return m_u32BufSize;
    }
    
    int InitDataBuf(uint32_t u32BufSize, key_t tShmKey = 0)
    {
        if(u32BufSize == 0) return 1;
        
        if(tShmKey == 0)
        {
            m_pDataBuf = new uint8_t[u32BufSize];
            if(m_pDataBuf == NULL)
            {
                return -1;
            }
            
            m_bIsShm = false;
        }
        else
        {
            int iShmId = shmget(tShmKey, u32BufSize, IPC_CREAT | 0666);
            if(iShmId == -1)
            {
                return -2;
            }
            
            m_pDataBuf = shmat(iShmId, NULL, 0);
            if(m_pDataBuf == NULL){
                return -3;
            }
            
            m_bIsShm = true;
        }
        
        return 0;
    }
    
    void * FindHashNode(uint32_t u32Row, uint32_t u32Col)
    {
        uint32_t u32Pos = m_u32PosTable[u32Row] + u32Col;
  
        if(u32Pos < m_u32DataNum)
        {
            return (void *)((stHashNode*)m_pDataBuf + u32Pos);
        }
        
        return NULL;    
    }
    
};


#endif
