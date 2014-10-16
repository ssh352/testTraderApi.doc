// testTraderApi.cpp : 定义控制台应用程序的入口点。
//
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include "TraderSpi.h"

// UserApi对象
CThostFtdcTraderApi* pUserApi;

// 配置参数

char  FRONT_ADDR[] = "";		// 前置地址
TThostFtdcBrokerIDType	BROKER_ID = "";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "";			// 用户密码
TThostFtdcInstrumentIDType INSTRUMENT_ID = "";	// 合约代码
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// 买卖方向
TThostFtdcPriceType	LIMIT_PRICE =2350 ;// 38850;				// 价格
// 请求编号
int iRequestID = 0;

void main(void)
{
	// 初始化UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// 创建UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// 注册事件类
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);				// 注册公有流
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// 注册私有流
	pUserApi->RegisterFront(FRONT_ADDR);							// connect
	pUserApi->Init();

	pUserApi->Join();
//	pUserApi->Release();
}

/*
tcp://222.66.166.147
tcp://222.66.166.147：41205前置地址
broker id2071
invester id 10072 密码123456
*/