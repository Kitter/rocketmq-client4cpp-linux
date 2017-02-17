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
#ifndef __RMQ_ALLOCATEMESSAGEQUEUESTRATEGY_H__
#define __RMQ_ALLOCATEMESSAGEQUEUESTRATEGY_H__

#include <vector>
#include <string>
#include <list>

#include "RocketMQClient.h"
#include "MessageQueue.h"

namespace rmq
{
	/**
	* Consumer�����Զ��������
	*
	*/
	class AllocateMessageQueueStrategy
	{
		/**
		* ����ǰ��ConsumerId�������
		*
		* @param currentCID
		*            ��ǰConsumerId
		* @param mqAll
		*            ��ǰTopic�����ж��м��ϣ����ظ����ݣ�������
		* @param cidAll
		*            ��ǰ�����������Consumer���ϣ����ظ����ݣ�������
		* @return �����������ظ�����
		*/
	public:
		virtual ~AllocateMessageQueueStrategy() {}
		virtual std::vector<MessageQueue>* allocate(
				const std::string& consumerGroup,
				const std::string& currentCID,
				std::vector<MessageQueue>& mqAll,
				std::list<std::string>& cidAll)=0;
		virtual std::string getName()=0;
	};
}

#endif
