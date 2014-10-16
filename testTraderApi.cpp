// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include "TraderSpi.h"

// UserApi����
CThostFtdcTraderApi* pUserApi;

// ���ò���

char  FRONT_ADDR[] = "";		// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "";				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "";			// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "";			// �û�����
TThostFtdcInstrumentIDType INSTRUMENT_ID = "";	// ��Լ����
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// ��������
TThostFtdcPriceType	LIMIT_PRICE =2350 ;// 38850;				// �۸�
// ������
int iRequestID = 0;

void main(void)
{
	// ��ʼ��UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// ����UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);				// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// ע��˽����
	pUserApi->RegisterFront(FRONT_ADDR);							// connect
	pUserApi->Init();

	pUserApi->Join();
//	pUserApi->Release();
}

/*
tcp://222.66.166.147
tcp://222.66.166.147��41205ǰ�õ�ַ
broker id2071
invester id 10072 ����123456
*/