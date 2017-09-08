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

using namespace std;

class CZTimer
{
private:
    pthread_t m_thread_timer;
    long m_second, m_microsecond;

    void * _Action(void * _this)
    {
        while (true)
        {
            OnTimer();

            pthread_testcancel();

            struct timeval stSleepTime;
            stSleepTime.tv_sec = m_second;
            stSleepTime.tv_usec = m_microsecond; 
            
            select(0, NULL, NULL, NULL, &stSleepTime);
        }
    }

public:
    CZTimer(long second=1, long microsecond=0) 
    {
        SetTimer(second, microsecond);
    }

    CZTimer::~CZTimer()
    {
    }

    void Set(long second, long microsecond)
    {
        m_second = second;
        m_microsecond = microsecond;
    }

    void Start()
    {
        pthread_create(&m_thread_timer, NULL, _Action, this);
    }

    void Stop()
    {
        pthread_cancel(m_thread_timer);
        pthread_join(m_thread_timer, NULL); //wait the thread stopped
    }

    virtual void CZTimer::OnTimer()
    {
        cout << "hi ztimer..." << endl;
    }
};
#endif
