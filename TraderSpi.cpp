#include <windows.h>
#include <iostream>
using namespace std;

#include "./ThostTraderApi/ThostFtdcTraderApi.h"
#include "TraderSpi.h"

#pragma warning(disable : 4996)
//static bool thing=false;
//static bool qryorder=false;
//static bool position=false;
//static int tempout=0;
static bool offer=false;
static bool cancel = false;
static bool dele=false;
static int tempinner=0;
static int i=0;
static bool ex=false;
static int sysid[20];
static int reqid[20];
char orderref[20][13];
char idref[20][13];

// USER_API����
extern CThostFtdcTraderApi* pUserApi;


static int judge=0;
// ���ò���
extern char FRONT_ADDR[];		// ǰ�õ�ַ
extern char BROKER_ID[];		// ���͹�˾����
extern char INVESTOR_ID[];		// Ͷ���ߴ���
extern char PASSWORD[];			// �û�����
extern char INSTRUMENT_ID[];	// ��Լ����
extern TThostFtdcPriceType	LIMIT_PRICE;	// �۸�
extern TThostFtdcDirectionType	DIRECTION;	// ��������

CThostFtdcOrderField  *p[20];
CThostFtdcOrderField  *t;

struct CThostFtdcOrderField  q;
char message[20][13];
 
// ������
extern int iRequestID;

// �Ự����
TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��
TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
TThostFtdcOrderRefType	ORDER_REF;	//��������

// �����ж�
bool IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTraderSpi::OnFrontConnected()
{
	cerr << "--->>> " << "OnFrontConnected" << endl;
	///�û���¼����
	ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> �����û���¼����: " << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
}
static int iNextOrderRef=0;
void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		cout<<pRspUserLogin->MaxOrderRef<<endl;
		iNextOrderRef++;	
		
		//sprintf( ORDER_REF,"%d", iNextOrderRef);
			
		///��ȡ��ǰ������
		cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		///Ͷ���߽�����ȷ��
		ReqSettlementInfoConfirm();
	}
}
void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	cerr << "--->>> Ͷ���߽�����ȷ��: " << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{   
	cout<<pSettlementInfoConfirm->BrokerID<<endl;
	cerr << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		cmd();	
	}
}
void CTraderSpi::cmd()
{
	cout<<"������������룺\n1��ѯ�ֲ�\n2����\n3����\n4��ѯί��"<<endl;
	
	int temp=0;
	static int j=0;
	
	scanf("%d",&temp);
	
	if(temp==1)
	{
		ReqQryInvestorPosition();
	}
	else if(temp==2)
	{
		ReqOrderInsert();
	}
	else if (temp==3)
	{
		strcpy(p[1]->OrderSysID,idref[2]);
		strcpy(p[1]->OrderRef,orderref[2]);
		cout<<p[2]->OrderSysID<<endl;
		cout<<p[1]->OrderRef<<endl;
		ReqOrderAction(p[1]);	
	}
	else if(temp==4)
	{
		ReqQryOrder();	
	}
	else if(temp==5)
	{
		int temp1;
		for(temp1=1;temp<i+1;temp1++)
		cout<<p[temp1]->OrderRef<<p[temp1]->OrderLocalID<<p[temp1]->OrderSysID<<endl;
	}
	else
	{
		cout<<"��֪����������ʲô"<<endl;
		cmd();
	}
}
///*****************************��ĳһ����λ��ĳ����Լ���б����Ĳ���******************************************************
void CTraderSpi::ReqOrderInsert()
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, BROKER_ID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, INVESTOR_ID);
	///��Լ����
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	///��������
	strcpy(req.OrderRef, ORDER_REF);
	///�û�����
//	TThostFtdcUserIDType	UserID;
	///�����۸�����: �޼�
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///��������: 
	req.Direction = DIRECTION;
	///��Ͽ�ƽ��־: ����
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	req.LimitPrice = LIMIT_PRICE;
	///����: 1
	req.VolumeTotalOriginal = 1;
	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 1;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ֹ���
//	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	req.IsAutoSuspend = 0;
	///ҵ��Ԫ
//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
	req.UserForceClose = 0;
	judge=2;
	int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
	cerr << "--->>> ����¼������: " << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
	
}
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderInsert" << endl;
	IsErrorRspInfo(pRspInfo);
	cmd();	
}
///*****************************��ĳһ����λ��ĳ����Լ���г����Ĳ���******************************************************
void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{	if (pOrder->OrderStatus != THOST_FTDC_OST_Canceled)
	{		
		
		CThostFtdcInputOrderActionField req;
		memset(&req, 0, sizeof(req));
		///���͹�˾����
		cout<<pOrder->BrokerID<<pOrder->InvestorID<<pOrder->ExchangeID<<pOrder->BrokerID<<pOrder->OrderSysID<<endl;
		strcpy(req.BrokerID, pOrder->BrokerID);
		///Ͷ���ߴ���
		strcpy(req.InvestorID, pOrder->InvestorID);
		///������������
		TThostFtdcOrderActionRefType	OrderActionRef;
		///��������
		strcpy(req.OrderRef, pOrder->OrderRef);
		///������

			TThostFtdcRequestIDType	RequestID;
		///ǰ�ñ��

		printf("%d",SESSION_ID);
		req.FrontID = FRONT_ID;
		///�Ự���
		req.SessionID = SESSION_ID;
		///����������
		strcpy(req.ExchangeID,pOrder->ExchangeID);
		///�������
		 strcpy(req.OrderSysID, pOrder->OrderSysID);
		///������־
		req.ActionFlag = THOST_FTDC_AF_Delete;
		///�۸�
		//req.LimitPrice=LIMIT_PRICE;
		///�����仯
	//	TThostFtdcVolumeType	VolumeChange;
		///�û�����
		//TThostFtdcUserIDType	UserID;
		///��Լ����
		//strcpy(req.InstrumentID, pOrder->InstrumentID);
		judge=3;
	
		int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);cerr << "--->>> ������������: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
	}
	else
	{
		cout<<"�����Ѿ���������"<<endl;
		cmd();
	}
}

	

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
	cmd();
}
///����֪ͨ

///*****************************�����������߳���������ϵͳ�ķ���******************************************************
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
	{
	int temp=0;	
	cerr << "--->>> " << "OnRtnOrder"  << endl;
	//�ҵ����Զ��ص�cmd()	
	if(judge==2)
	{
		cerr << "--->>> " << "OnRtnOrder"  << endl;
		if(offer)
		{
			offer=false;
			cout<<ORDER_REF<<endl;
			temp=atoi(pOrder->OrderRef);
			printf("%d",temp);
			i++;
			strcpy(orderref[i],pOrder->OrderRef);
			strcpy(idref[i],pOrder->OrderSysID);
			reqid[temp]=pOrder->RequestID;
			cout<<pOrder->BrokerID<<pOrder->InvestorID<<pOrder->ExchangeID<<pOrder->BrokerID<<pOrder->OrderSysID<<idref<<orderref<<endl;
			p[temp]=pOrder;			
			cmd();
		}
		else 
		{
			offer=true;
		}
	}

	if(judge==3)
	{
		cout<<"�����ɹ�"<<endl;
		if(cancel)
		{
			cout<<"�����ɹ�22"<<endl;
			cancel=false;
			cmd();
		}
		else
		{
			cancel=true;
		}
	}
}
///*****************************��ѯ���еĹҵ�******************************************************
void CTraderSpi::ReqQryOrder()
{	 CThostFtdcQryOrderField req;

	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, BROKER_ID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, INVESTOR_ID);	
	strcpy(req.InstrumentID,INSTRUMENT_ID);
	///��Լ����
	///���͹�˾����
	/*{
	///���͹�˾����
	TThostFtdcBrokerIDType	BrokerID;
	///Ͷ���ߴ���
	TThostFtdcInvestorIDType	InvestorID;
	///��Լ����
	TThostFtdcInstrumentIDType	InstrumentID;
	///����������
	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	TThostFtdcOrderSysIDType	OrderSysID;
	///��ʼʱ��
	TThostFtdcTimeType	InsertTimeStart;
	///����ʱ��
	TThostFtdcTimeType	InsertTimeEnd;
};*/

	tempinner=0;

	int iResult = pUserApi->ReqQryOrder(&req, ++iRequestID);
		
	if (!IsFlowControl(iResult))
	{
		cerr << "--->>> ������ѯ����: " << iResult << ((iResult == 0) ? (", �ɹ�") : ", ʧ��") << endl;
	}
	else
	{
		cerr << "--->>> �����ѯͶ���ֲ߳�: "  << iResult << ", �ܵ�����" << endl;
		Sleep(1000);
	}
};
void  CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{

	if(pOrder->OrderStatus!= THOST_FTDC_OST_Canceled)	
	{	
		cout<<pOrder->OrderStatus<<pOrder->OrderRef<<pOrder->OrderSysID<<endl;}
	
					/*if(dele)
			{	if(!ex)	
				{printf("%d ",tempinner );
		cout<<pOrder->OrderStatus<<pOrder->OrderRef<<pOrder->OrderSysID<<endl;}
			else
				if(tempinner==i)
		{
		ReqOrderAction(pOrder);	
			ex=false;
				i=0;
				dele=false;}
				}	*/
		
		
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{ 
	/*if(dele)
	{cout<<"��������Ҫ���ĵ��ӵĺ��룺"<<endl;
	scanf("%d",&i);
	ex=true;
	ReqQryOrder();
	}
	else */
		cmd();
	}
}
///�ɽ�֪ͨ
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cerr << "--->>> " << "OnRtnTrade"  << endl;
}

void CTraderSpi:: OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << "OnFrontDisconnected" << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}
		
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
	{
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	}
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return 
		(
			(pOrder->FrontID == FRONT_ID) &&
			(pOrder->SessionID == SESSION_ID) &&
			(strcmp(pOrder->OrderRef, ORDER_REF) == 0)
		);
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return 
		(
			(pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded)
		);
}
void CTraderSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cerr << "--->>> �����ѯ��Լ: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
			break;
		}
		else
		{
			cerr << "--->>> �����ѯ��Լ: "  << iResult << ", �ܵ�����" << endl;
			Sleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspQryInstrument" << endl;
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		cmd();
	}
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
		if (!IsFlowControl(iResult))
		{
			cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
			break;
		}
		else
		{
			cerr << "--->>> �����ѯ�ʽ��˻�: "  << iResult << ", �ܵ�����" << endl;
			Sleep(1000);
		}
	} // while
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspQryTradingAccount" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
	cmd();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	while (true)
	{
		int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
		printf("%d",iResult);
		if (!IsFlowControl(iResult))
		{
			cerr << "--->>> �����ѯͶ���ֲ߳�: "  << iResult << ((iResult == 0) ? ", �ɹ�" : ", ʧ��") << endl;
			break;
		}
		else
		{
			cerr << "--->>> �����ѯͶ���ֲ߳�: "  << iResult << ", �ܵ�����" << endl;
			Sleep(1000);
			cmd();	
		}
	} // while
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspQryInvestorPosition" << endl;
//	qryorder=false;
	cout<<pInvestorPosition->Position<<endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		cmd();
	}			
}
