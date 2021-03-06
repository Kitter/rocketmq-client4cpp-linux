#include "Common.h"
#include "DefaultMQProducer.h"
using namespace rmq;

void Usage(const char* program)
{
	printf("Usage:%s ip:port [-g group] [-t topic] [-n count] [-s size] [-w logpath]\n", program);
	printf("\t -g group\n");
	printf("\t -t topic\n");
	printf("\t -n message count\n");
	printf("\t -s message size \n");
	printf("\t -w log path\n");
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		Usage(argv[0]);
		return 0;
	}

	std::string namesrv = argv[1];
	std::string group = "pg_test_group";
	std::string topic = "topic_test";
	int size = 32;
	int count = 1000;

	for (int i=2; i< argc; i++)
	{
		if (strcmp(argv[i],"-g")==0)
		{
			if (i+1 < argc)
			{
				group = argv[i+1];
				i++;
			}
			else
			{
				Usage(argv[0]);
				return 0;
			}
		}
		else if (strcmp(argv[i],"-t")==0)
		{
			if (i+1 < argc)
			{
				topic = argv[i+1];
				i++;
			}
			else
			{
				Usage(argv[0]);
				return 0;
			}
		}
		else if (strcmp(argv[i],"-n")==0)
		{
			if (i+1 < argc)
			{
				count = atoi(argv[i+1]);
				i++;
			}
			else
			{
				Usage(argv[0]);
				return 0;
			}
		}
		else if (strcmp(argv[i],"-s")==0)
		{
			if (i+1 < argc)
			{
				size = atoi(argv[i+1]);
				i++;
			}
			else
			{
				Usage(argv[0]);
				return 0;
			}
		}
		else if (strcmp(argv[i],"-w")==0)
        {
            if (i+1 < argc)
            {
                MyUtil::initLog(argv[i+1]);
                i++;
            }
            else
            {
                Usage(argv[0]);
                return 0;
            }
        }
		else
		{
			Usage(argv[0]);
			return 0;
		}
	}

	// 初始化client api日志，此处非必要，需要对api进行调试才需要进行初始化，可以考虑注释
	// 这里默认只打印警告、错误日志，日志会按天滚动，如果需要修改日志级别，请设置一下环境变量，export ROCKETMQ_LOGLEVEL=日志级别
    // 日志级别如下:
    //  0   - 关闭日志
    //  1   - 写错误 日志
    //  2   - 写错误,警告 日志
    //  3   - 写错误,警告,信息 日志
    //  4   - 写错误,警告,信息,调试 日志
	RocketMQUtil::initLog("/tmp/rocketmq_producer.log");

	// 初始化RocketMQ生产者，传入生产组名称
	RMQ_DEBUG("producer.new: %s", group.c_str());
	DefaultMQProducer producer(group);

	// 设置MQ的NameServer地址
	RMQ_DEBUG("producer.setNamesrvAddr: %s", namesrv.c_str());
	producer.setNamesrvAddr(namesrv);

	// 启动生产者
	RMQ_DEBUG("producer.start");
	producer.start();

	std::string tags[] = { "TagA", "TagB", "TagC", "TagD", "TagE" };

	int _cost = 0, _tps = 0, _avg = 0, _min = 0, _max = 0;
	int _failCnt = 0;
	TimeCount tc;
	TimeCount tcTotal;
	tcTotal.begin();

	int nNow = time(NULL);
	char key[64];
	char value[1024];

	std::string str;
	for (int i = 0; i < size; i += 8)
	{
        str.append("hello baby");
    }

	for (int i = 0; i < count; i++)
	{
		try
		{

			tc.begin();

			snprintf(key, sizeof(key), "KEY_%d_%d", nNow, i);
			snprintf(value, sizeof(value), "%011d_%s", i, str.c_str());
			Message msg(topic,// topic
				tags[i % 5],// tag
				key,// key
				value,// body
				strlen(value)+1
			);

			// 同步生产消息
			SendResult sendResult = producer.send(msg);

			tc.end();

			int cost = tc.countMsec();
			_min = (_min == 0) ? cost : (std::min(cost, _min));
			_max = (_max == 0) ? cost : (std::max(cost, _max));

			MYLOG("[%d]|succ|cost:%dms, result:%s\n", i, cost, sendResult.toString().c_str());
		}
		catch (MQClientException& e)
		{
			_failCnt++;
			MYLOG("[%d]|fail|%s\n", i, e.what());
		}
	}
	tcTotal.end();

	MYDEBUG("statsics: num=%d, fail=%d, total_cost=%ds, tps=%d, avg=%dms, min=%dms, max=%dms\n",
		count, _failCnt, tcTotal.countSec(), (int)((double)count/(tcTotal.countMsec()/1000)),
		tcTotal.countMsec()/count, _min, _max);

	// 停止生产者
	producer.shutdown();

	return 0;
}

