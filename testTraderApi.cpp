// testTraderApi.cpp : 定义控制台应用程序的入口点。
//

/*
CTP仿真账号：99000533
密码：013035
经纪公司代码 4200

电信	交易 180.169.40.126 21205
		行情 180.169.40.126 21213
		
联通	交易 58.247.2.86 21205
		行情 58.247.2.86 21213
*/
#include <string>
#include "./ThostTraderApi/ThostFtdcTraderApi.h"
#include "./TraderSpi_rewrite.h"
#include "./cfg.h"

using namespace std;
// UserApi对象
CThostFtdcTraderApi* pUserApi;

cfg * simucfg;
/*
将这些都用cfg实现
// 配置参数
string FRONT_ADDR;			// 前置地址
TThostFtdcBrokerIDType	BROKER_ID;				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID;		// 投资者代码
TThostFtdcPasswordType  PASSWORD;			// 用户密码
TThostFtdcInstrumentIDType INSTRUMENT_ID;	// 合约代码
*/

/*
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// 买卖方向
TThostFtdcPriceType	LIMIT_PRICE =2350 ;					// 价格
*/
// 请求编号
int iRequestID = 0;

void loadconfig();

void main(void)
{
	loadconfig();
	// 初始化UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// 创建UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);				// 注册公有流
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// 注册私有流
	pUserApi->RegisterFront(const_cast<char*>(simucfg->getparam("FRONT_ADDR").c_str()));// connect
	pUserApi->Init();
	pUserApi->Join();
//	pUserApi->Release();
}

void loadconfig()
{
	simucfg=new cfg();
	simucfg->setcfgfile("c:/cfg/simu_trade.cfg");
}
/*
tcp://222.66.166.147
tcp://222.66.166.147：41205前置地址
broker id2071
invester id 10072 密码123456
*/