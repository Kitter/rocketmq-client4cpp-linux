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
#ifndef __RMQ_OFFSETSTORE_H__
#define __RMQ_OFFSETSTORE_H__

#include <set>
#include <map>

#include "RocketMQClient.h"

namespace rmq
{
	class MessageQueue;

	enum ReadOffsetType
	{
		// ֻ��Memory��ȡ
		READ_FROM_MEMORY,
		// ֻ�Ӵ洢���ȡ�����ػ���Զ�ˣ�
		READ_FROM_STORE,
		// �ȴ��ڴ�����ڴ治�����ٴӴ洢���
		MEMORY_FIRST_THEN_STORE,
	};

	/**
	* Consumer Offset�洢�ӿ�
	*
	*/
	class OffsetStore
	{
	public:
		virtual ~OffsetStore() {}

		/**
		* ����Offset
		*/
		virtual void load()=0;

		/**
		* �������ѽ��ȣ��洢���ڴ�
		*/
		virtual void updateOffset(const MessageQueue& mq, long long offset, bool increaseOnly)=0;

		/**
		* �ӱ��ػ����ȡ���ѽ���
		*/
		virtual long long readOffset(const MessageQueue& mq, ReadOffsetType type)=0;

		/**
		* �־û�ȫ�����ѽ��ȣ����ܳ־û����ػ���Զ��Broker
		*/
		virtual void persistAll(std::set<MessageQueue>& mqs)=0;
		virtual void persist(const MessageQueue& mq)=0;

		/**
		* ɾ������Ҫ��MessageQueue offset
		*/
		virtual void removeOffset(const MessageQueue& mq)=0;

		/**
		* ��¡ƫ������
		*/
		virtual std::map<MessageQueue, long long> cloneOffsetTable(const std::string& topic) = 0;
	};
}

#endif
