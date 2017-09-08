//
//  zload.h
//
//  Created by izuzi on 17/09/08.
//  Copyright@2017 izuzi@foxmail.com All rights reserved.
//

#ifndef Z_TIMER_H_
#define Z_TIMER_H_

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include <string>

using namespace std;

class CZTimer
{
private:
    pthread_t m_thread_timer;
    long m_second, m_microsecond;
    string m_strFlag;
    long m_Counter;

    void _Sleep()
    {
        pthread_testcancel();

        struct timeval stSleepTime;
        stSleepTime.tv_sec = m_second;
        stSleepTime.tv_usec = m_microsecond; 
        
        select(0, NULL, NULL, NULL, &stSleepTime);
    }

    static void * _Action(void * _this)
    {
        while (true)
        {
            (static_cast<CZTimer*>(_this))->OnTimer();

            (static_cast<CZTimer*>(_this))->_Sleep();
        }
    }

public:
    CZTimer(long second=1, long microsecond=0) 
    {
        Set(second, microsecond);
        m_Counter = 0;
    }

    ~CZTimer()
    {
    }

    void Set(long second, long microsecond)
    {
        m_second = second;
        m_microsecond = microsecond;
    }

    void Start(string strFlag="0")
    {
        m_strFlag = strFlag;
        int ret = pthread_create(&m_thread_timer, NULL, _Action, this);
    }

    void Stop()
    {
        pthread_cancel(m_thread_timer);
        pthread_join(m_thread_timer, NULL); //wait the thread stopped
    }

    void Wait()
    {
        pthread_join(m_thread_timer, NULL);
    }

    virtual void OnTimer()
    {
        cout << "hi ztimer(" << m_strFlag << "), " << m_Counter++ % 10000 << endl;

        //if(m_Counter==5)
        //{
        //    Stop();
        //}
    }
};
#endif
