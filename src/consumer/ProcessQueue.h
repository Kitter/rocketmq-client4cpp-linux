/**
* Copyright (C) 2013 kangliqiang ,kangliq@163.com
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef __PROCESSQUEUE_H__
#define __PROCESSQUEUE_H__

#include <list>
#include <map>
#include "Mutex.h"
#include "AtomicValue.h"

namespace rmq
{
    class MessageExt;

    /**
    * ���ڱ����ѵĶ��У�����Ϣ
    *
    */
    class ProcessQueue
    {

    public:
        ProcessQueue();

        bool isLockExpired();

        /**
        * @return �Ƿ���Ҫ�ַ���ǰ���е������̳߳�
        */
        bool putMessage(const std::list<MessageExt*>& msgs);

        /**
        * ��ȡ��ǰ���е������
        */
        long long getMaxSpan();

        /**
        * ɾ���Ѿ����ѹ�����Ϣ��������СOffset�����Offset��Ӧ����Ϣδ����
        *
        * @param msgs
        * @return
        */
        long long removeMessage(std::list<MessageExt*>& msgs);

		/**
        * �����Ϣ
        */
		void clear();

        std::map<long long, MessageExt*> getMsgTreeMap();
        kpr::AtomicInteger getMsgCount();
        bool isDropped();
        void setDropped(bool dropped);
		bool isPullExpired();

		unsigned long long getLastPullTimestamp();
		void setLastPullTimestamp(unsigned long long lastPullTimestamp);

		unsigned long long getLastConsumeTimestamp();
		void setLastConsumeTimestamp(unsigned long long lastConsumeTimestamp);

        /**
        * ========================================================================
        * ���²���Ϊ˳����Ϣר�в���
        */
		kpr::Mutex& getLockConsume();
        void setLocked(bool locked);
        bool isLocked();
		long long getTryUnlockTimes();
		void incTryUnlockTimes();

        void rollback();
        long long commit();
        void makeMessageToCosumeAgain(const std::list<MessageExt*>& msgs);

        /**
        * ���ȡ������Ϣ������������״̬��Ϊfalse
        *
        * @param batchSize
        * @return
        */
        std::list<MessageExt*> takeMessages(int batchSize);

        long long getLastLockTimestamp();
        void setLastLockTimestamp(long long lastLockTimestamp);

    public:
        static unsigned int s_RebalanceLockMaxLiveTime;// �ͻ��˱���Lock������ʱ�䣬�������Զ����ڣ���λms
        static unsigned int s_RebalanceLockInterval;// ��ʱLock���ʱ�䣬��λms
        static unsigned int s_PullMaxIdleTime;		// ��ȡ���idleʱ�䣬��λms

    private:
        kpr::RWMutex m_lockTreeMap;
        std::map<long long, MessageExt*> m_msgTreeMap;
        volatile long long m_queueOffsetMax ;
        kpr::AtomicInteger m_msgCount;
        volatile bool m_dropped;// ��ǰQ�Ƿ�rebalance����
        volatile unsigned long long m_lastPullTimestamp;
		volatile unsigned long long m_lastConsumeTimestamp;

        /**
        * ˳����Ϣר��
        */
        kpr::Mutex m_lockConsume;
        volatile bool m_locked;// �Ƿ��Broker����
        volatile unsigned long long m_lastLockTimestamp;// ���һ�������ɹ�ʱ���
        volatile bool m_consuming;// �Ƿ����ڱ�����
        std::map<long long, MessageExt*> m_msgTreeMapTemp;// ����ʽ���ѣ�δ�ύ����Ϣ
        kpr::AtomicInteger m_tryUnlockTimes;
    };
}

#endif
