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

#ifndef __RMQ_DEFAULTMQPRODUCER_H__
#define __RMQ_DEFAULTMQPRODUCER_H__

#include <string>
#include <list>
#include <vector>

#include "RocketMQClient.h"
#include "MQClientException.h"
#include "Message.h"
#include "SendResult.h"
#include "ClientConfig.h"
#include "MQProducer.h"

namespace rmq
{
	class MessageQueue;
	class MessageQueueSelector;
	class MQClientException;
	class Message;
	class DefaultMQProducerImpl;

	/**
	* ��Ϣ�����ߣ��ʺ�ʹ��spring��ʼ��
	*
	*/
	class DefaultMQProducer : public ClientConfig ,public MQProducer
	{
	public:
		DefaultMQProducer();
		DefaultMQProducer(const std::string& producerGroup);
		~DefaultMQProducer();

		//begin MQProducer
		void start();
		void shutdown();

		std::vector<MessageQueue>* fetchPublishMessageQueues(const std::string& topic);

		SendResult send(Message& msg);
		SendResult send(Message& msg, MessageQueue& mq);
		SendResult send(Message& msg, MessageQueueSelector* selector, void* arg);
		void send(Message& msg, SendCallback* pSendCallback);
		void send(Message& msg, MessageQueue& mq, SendCallback* pSendCallback);
		void send(Message& msg, MessageQueueSelector* selector, void* arg, SendCallback* pSendCallback);
		void sendOneway(Message& msg);
		void sendOneway(Message& msg, MessageQueue& mq);
		void sendOneway(Message& msg, MessageQueueSelector* selector, void* arg);

		TransactionSendResult sendMessageInTransaction(Message& msg,
														LocalTransactionExecuter* pTranExecuter,
														void* arg);
		//end MQProducer

		void createTopic(const std::string& key, const std::string& newTopic, int queueNum) ;
		long long searchOffset(const MessageQueue& mq, long long timestamp);
		long long maxOffset(const MessageQueue& mq);
		long long minOffset(const MessageQueue& mq);
		long long earliestMsgStoreTime(const MessageQueue& mq);
		MessageExt* viewMessage(const std::string& msgId) ;
		QueryResult queryMessage(const std::string& topic,
								 const std::string& key,
								 int maxNum,
								 long long begin,
								 long long end);

		std::string getProducerGroup();
		void setProducerGroup(const std::string& producerGroup);

		std::string getCreateTopicKey();
		void setCreateTopicKey(const std::string& createTopicKey);

		int getSendMsgTimeout();
		void setSendMsgTimeout(int sendMsgTimeout) ;

		int getCompressMsgBodyOverHowmuch();
		void setCompressMsgBodyOverHowmuch(int compressMsgBodyOverHowmuch);

		int getCompressLevel();
		void setCompressLevel(int compressLevel);

		DefaultMQProducerImpl* getDefaultMQProducerImpl();

		bool isRetryAnotherBrokerWhenNotStoreOK();
		void setRetryAnotherBrokerWhenNotStoreOK(bool retryAnotherBrokerWhenNotStoreOK);

		int getMaxMessageSize();
		void setMaxMessageSize(int maxMessageSize);

		int getDefaultTopicQueueNums();
		void setDefaultTopicQueueNums(int defaultTopicQueueNums);

		int getRetryTimesWhenSendFailed();
	    void setRetryTimesWhenSendFailed(int retryTimesWhenSendFailed);

	protected:
		DefaultMQProducerImpl* m_pDefaultMQProducerImpl;

	private:
		std::string m_producerGroup;///< һ�㷢��ͬ����Ϣ��Producer����Ϊͬһ��Group��Ӧ�ñ������ã�����֤����Ψһ
		std::string m_createTopicKey;///< ֧���ڷ�����Ϣʱ�����Topic�����ڣ��Զ�����Topic������Ҫָ��Key

		int m_defaultTopicQueueNums;///< ������Ϣ���Զ�����Topicʱ��Ĭ�϶�����4
		int m_sendMsgTimeout;///< ������Ϣ��ʱ��Ĭ��3000, �������޸�
		int m_compressMsgBodyOverHowmuch;///< Message Body��С������ֵ����ѹ��,Ĭ��4k
		int m_retryTimesWhenSendFailed;///< ����ʧ�������Դ�����Ĭ��2
		bool m_retryAnotherBrokerWhenNotStoreOK;// ��Ϣ�Ѿ��ɹ�д��Master������ˢ�̳�ʱ����ͬ����Slaveʧ�ܣ�����������һ��Broker���������޸�Ĭ��ֵ��˳����Ϣ��Ч
		int m_maxMessageSize;///< �����Ϣ��С��Ĭ��128K
		int m_compressLevel;
	};
}

#endif
