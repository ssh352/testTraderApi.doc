// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

/*
CTP�����˺ţ�99000533
���룺013035
���͹�˾���� 4200

����	���� 180.169.40.126 21205
		���� 180.169.40.126 21213
		
��ͨ	���� 58.247.2.86 21205
		���� 58.247.2.86 21213
*/
#include <string>
#include "./ThostTraderApi/ThostFtdcTraderApi.h"
#include "./TraderSpi_rewrite.h"
#include "./cfg.h"

using namespace std;
// UserApi����
CThostFtdcTraderApi* pUserApi;

cfg * simucfg;
/*
����Щ����cfgʵ��
// ���ò���
string FRONT_ADDR;			// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID;				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID;		// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD;			// �û�����
TThostFtdcInstrumentIDType INSTRUMENT_ID;	// ��Լ����
*/

/*
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// ��������
TThostFtdcPriceType	LIMIT_PRICE =2350 ;					// �۸�
*/
// ������
int iRequestID = 0;

void loadconfig();

void main(void)
{
	loadconfig();
	// ��ʼ��UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// ����UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);				// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// ע��˽����
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
tcp://222.66.166.147��41205ǰ�õ�ַ
broker id2071
invester id 10072 ����123456
*/