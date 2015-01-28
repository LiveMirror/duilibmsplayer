#include "stdafx.h"
#include "LogQueryUI.h"
#include "../../DataBase/QDDvrDB.h"
#include "../../WellCommon/DataConvert.h"
#include "../../ShareData.h"
#include "ControlEx.h"

CLogQueryUI::CLogQueryUI()
{
	CDialogBuilder builder;
	m_nLogType = 0;
	CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("QueryLog.xml"), (UINT)0));
	if( pComputerExamine ) {
		this->Add(pComputerExamine);
	}
	else {
		this->RemoveAll();
		return;
	}
}

CLogQueryUI::~CLogQueryUI()
{

}

bool CLogQueryUI::Init(CPaintManagerUI& m_pm)
{
	//显示消息
	m_pLblMsg = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblmsg")));	

	m_btnQueryLog = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSearchLog")));
	m_pListDvrLog = static_cast<CListUI*>(m_pm.FindControl(_T("lstDvrLog")));
	m_pListPlatformLog = static_cast<CListUI*>(m_pm.FindControl(_T("lstPlatformLog")));
	m_plstWSStatusLog = static_cast<CListUI*>(m_pm.FindControl(_T("lstWSStatusLog")));

	m_cbTypeLog = static_cast<CComboUI*>(m_pm.FindControl(_T("cbLogType")));
	m_cbPoliceLog = static_cast<CComboUI*>(m_pm.FindControl(_T("cbPoliceLog")));
	m_lblPoliceLog = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblPoliceLog")));

	m_cbYearSLog = static_cast<CComboUI*>(m_pm.FindControl(_T("cbYearSLog")));
	m_cbMonSLog = static_cast<CComboUI*>(m_pm.FindControl(_T("cbMonthSLog")));
	m_cbDaySLog = static_cast<CComboUI*>(m_pm.FindControl(_T("cbDaySLog")));
	m_cbYearELog = static_cast<CComboUI*>(m_pm.FindControl(_T("cbYearELog")));
	m_cbMonELog = static_cast<CComboUI*>(m_pm.FindControl(_T("cbMonthELog")));
	m_cbDayELog = static_cast<CComboUI*>(m_pm.FindControl(_T("cbDayELog")));

	CScrollBarUI* pScroll;
	pScroll =  m_pListDvrLog->GetVerticalScrollBar();
	SetVScrollBar(pScroll);

	//pScroll->SetButton1NormalImage(_T("process.png"));
	pScroll = m_pListPlatformLog->GetVerticalScrollBar();
	pScroll->SetFixedWidth(40);
	SetVScrollBar(pScroll);

	pScroll=m_plstWSStatusLog->GetVerticalScrollBar();
	pScroll->SetFixedWidth(40);
	SetVScrollBar(pScroll);

	InitLogQueryPageData();

	m_cbTypeLog->SelectItem(0);
	return true;
}

void CLogQueryUI::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		OnItemSelect(msg);
	}
	else if( msg.sType == DUI_MSGTYPE_CLICK ) 
	{
		if( msg.pSender == m_btnQueryLog)
		{
			OnBtnLogQuery();
		}
	}
	else if( msg.sType == DUI_MSGTYPE_DROPDOWN)
	{
		if(msg.pSender == m_cbPoliceLog)
		{
			//RefreshPolices();
			return;
		}
	}
}

void CLogQueryUI::OnItemSelect(TNotifyUI &msg)
{
	if(msg.pSender == m_cbTypeLog)
	{
		OnItemSelectTypeLog();
	}
}

bool CLogQueryUI::InitLogQueryPageData()
{
	CListLabelElementUI* pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetFixedHeight(50);
	pListLabelelement->SetText(_T("执法仪插拔日志"));
	m_cbTypeLog->AddAt(pListLabelelement,0);

	pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetFixedHeight(50);
	pListLabelelement->SetText(_T("采集站操作日志"));
	m_cbTypeLog->AddAt(pListLabelelement, 1);

	pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetFixedHeight(50);
	pListLabelelement->SetText(_T("采集站状态日志"));
	m_cbTypeLog->AddAt(pListLabelelement, 2);
	m_cbTypeLog->SelectItem(0);

	//////////////////////////////////////////////////////////////////////////
	//初始化日期
	CTime tm = CTime::GetCurrentTime();
	int year = tm.GetYear();
	int mon = tm.GetMonth();
	int day = tm.GetDay();
	for(int i = 0; i < 20; ++i)
	{
		CListLabelElementUI* pListLabelelement = new CListLabelElementUI; 
		pListLabelelement->SetText(TO_WSTRING(year - i).c_str());
		pListLabelelement->SetFixedHeight(40);
		m_cbYearSLog->AddAt(pListLabelelement, i);
		CListLabelElementUI* pLLE = new CListLabelElementUI(*pListLabelelement);
		m_cbYearELog->AddAt(pLLE, i);
	}
	for(int i = 0; i < 12; ++i)
	{
		CListLabelElementUI* pListLabelelement = new CListLabelElementUI; 
		pListLabelelement->SetText(TO_WSTRING(i + 1).c_str());
		pListLabelelement->SetFixedHeight(40);
		m_cbMonELog->AddAt(pListLabelelement, i );
		CListLabelElementUI* pLLE = new CListLabelElementUI(*pListLabelelement);
		m_cbMonSLog->AddAt(pLLE, i);
	}
	for(int i = 0; i < 31; ++i)
	{
		CListLabelElementUI* pListLabelelement = new CListLabelElementUI; 
		pListLabelelement->SetText(TO_WSTRING(i + 1).c_str());
		pListLabelelement->SetFixedHeight(40);
		m_cbDaySLog->AddAt(pListLabelelement, i);
		CListLabelElementUI* pLLE = new CListLabelElementUI(*pListLabelelement);
		m_cbDayELog->AddAt(pLLE, i);
	}

	m_cbYearELog->SelectItem(0);
	m_cbMonELog->SelectItem(mon - 1);
	m_cbDayELog->SelectItem(day - 1);			
	// 	CTimeSpan ts(1,0,0,0);
	// 	CTime tS = tm - ts;
	// 	m_cbYearSLog->SelectItem(tm.GetYear() - tS.GetYear());
	// 	m_cbMonSLog->SelectItem(tS.GetMonth() - 1);
	// 	m_cbDaySLog->SelectItem(tS.GetDay() - 1);
	m_cbYearSLog->SelectItem(0);
	m_cbMonSLog->SelectItem(mon - 1);
	m_cbDaySLog->SelectItem(day - 1);
	//////////////////////////////////////////////////////////////////////////
	
	RefreshPolices();

	return true;
}


void CLogQueryUI::OnItemSelectTypeLog()
{

	m_nLogType = m_cbTypeLog->GetCurSel();
	if(m_nLogType == 0)
	{
		m_lblPoliceLog->SetVisible(true);
		m_cbPoliceLog->SetVisible(true);
		m_pListDvrLog->SetVisible(true);
		m_pListPlatformLog->SetVisible(false);
		m_plstWSStatusLog->SetVisible(false);
	}
	else if(m_nLogType == 1)
	{
		m_lblPoliceLog->SetVisible(false);
		m_cbPoliceLog->SetVisible(false);
		m_pListDvrLog->SetVisible(false);
		m_pListPlatformLog->SetVisible(true);
		m_plstWSStatusLog->SetVisible(false);
	}
	else if(m_nLogType == 2)
	{
		m_lblPoliceLog->SetVisible(false);
		m_cbPoliceLog->SetVisible(false);
		m_pListDvrLog->SetVisible(false);
		m_pListPlatformLog->SetVisible(false);
		m_plstWSStatusLog->SetVisible(true);
	}
}

bool CLogQueryUI::ShowDvrLogData(const list<CTDvrLog>& dvrLog)
{
	if(dvrLog.size() > 0)
	{
		list<CTDvrLog>::const_iterator it = dvrLog.begin();
		for(int index = 0;it != dvrLog.end();index ++,it ++)
		{
			CListTextElementUI* pListElement = new CListTextElementUI;
			pListElement->SetTag(index);
			m_pListDvrLog->Add(pListElement);

			pListElement->SetText(0, (*it).getDvrId().c_str());
			pListElement->SetText(1, (*it).getPId().c_str());
			pListElement->SetText(2, (*it).getType() == 1 ? _T("设备拔出") : _T("设备接入"));
			pListElement->SetText(3, (*it).getOperateTime().c_str());
			pListElement->SetText(4,  (*it).getContent().c_str());
		}
	}
	return true;
}

bool CLogQueryUI::ShowPlatformLogData(const list<CTPlatformLog>& pfLog)
{
	if(pfLog.size() > 0)
	{
		list<CTPlatformLog>::const_iterator it = pfLog.begin();
		for(int index = 0;it != pfLog.end();index ++,it ++)
		{
			CListTextElementUI* pListElement = new CListTextElementUI;
			pListElement->SetTag(index);
			m_pListPlatformLog->Add(pListElement);

			pListElement->SetText(0, (*it).getWorkstationId().c_str());
			pListElement->SetText(1, (*it).getUserIp().c_str());
			pListElement->SetText(2, (*it).getOperateTime().c_str());
			pListElement->SetText(3,  (*it).getContent().c_str());
		}
	}
	return true;
}

bool CLogQueryUI::ShowWorkstationStatusData(const list<CTWorkstationStatus>& pfLog)
{
	if(pfLog.size() > 0)
	{
		list<CTWorkstationStatus>::const_iterator it = pfLog.begin();
		for(int index = 0;it != pfLog.end();index ++,it ++)
		{
			CListTextElementUI* pListElement = new CListTextElementUI;
			pListElement->SetTag(index);
			m_plstWSStatusLog->Add(pListElement);

			pListElement->SetText(0, (*it).getWorkstationCode().c_str());
			wstring strT = (*it).getCPU();
			if(!strT.empty())
			{
				strT = strT.substr(4, strT.length() - 4) + _T("%");
			}
			pListElement->SetText(1, strT.c_str());

			strT = (*it).getMemory();
			if(!strT.empty())
			{
				strT = strT.substr(7, strT.length() - 7) + _T("%");
			}
			pListElement->SetText(2, strT.c_str());
			pListElement->SetText(3,  (*it).getNET().c_str());
			pListElement->SetText(4,  (*it).getTotalSize().c_str());
			pListElement->SetText(5,  (*it).getFreeSize().c_str());
			pListElement->SetText(6,  (*it).getOperateTime().c_str());
			pListElement->SetText(7,  (*it).getStatus().c_str());
		}
	}
	return true;
}


void CLogQueryUI::OnBtnLogQuery()
{
	CDuiString strDateS = m_cbYearSLog->GetText() + _T("-") + m_cbMonSLog->GetText() + _T("-") + m_cbDaySLog->GetText() + _T(" 00:00:01");
	CDuiString strDateE = m_cbYearELog->GetText()+ _T("-") + m_cbMonELog->GetText() + _T("-") + m_cbDayELog->GetText() + _T(" 59:59:59");
	int size = 0;

	QueryParam tQueryParam;
	tQueryParam.startTime = strDateS.GetData();
	tQueryParam.endTime = strDateE.GetData();
	if(m_nLogType > 3 || m_nLogType < 0)
		m_nLogType = 0;
	if(m_nLogType == 0)
	{
		if(m_cbPoliceLog->GetCurSel() >= 0)
			tQueryParam.policeID = m_cbPoliceLog->GetItemAt(m_cbPoliceLog->GetCurSel())->GetUserData();
		else
			tQueryParam.policeID.clear();
		if(tQueryParam.policeID == _T("--全部--"))
			tQueryParam.policeID.clear();

		list<CTDvrLog> tList = QD_DB.QueryDvrLog(tQueryParam);
		size = tList.size();
		m_pListDvrLog->RemoveAll();

		m_lblPoliceLog->SetVisible(true);
		m_cbPoliceLog->SetVisible(true);
		m_pListDvrLog->SetVisible(true);
		m_pListPlatformLog->SetVisible(false);
		m_plstWSStatusLog->SetVisible(false);

		ShowDvrLogData(tList);
	}
	else if(m_nLogType == 1)
	{
		list<CTPlatformLog> tList = QD_DB.QueryPlatformLog(tQueryParam);
		size = tList.size();
		m_pListPlatformLog->RemoveAll();
		ShowPlatformLogData(tList);
	}
	else if(m_nLogType == 2)
	{
		list<CTWorkstationStatus> tList = QD_DB.QueryWorkstationStatus(tQueryParam);
		size = tList.size();
		m_plstWSStatusLog->RemoveAll();
		ShowWorkstationStatusData(tList);
	}

	if( m_pLblMsg )
	{
		wstring tShowMessage(_T("查询结果数："));
		tShowMessage += TO_WSTRING(size);
		m_pLblMsg->SetText(tShowMessage.c_str());
	}
}

void CLogQueryUI::RefreshPolices()
{
	list<IDNAME> _userInfo;
	_userInfo = QD_DB.QueryUserInfoWithDebID(SHAREDATA.g_strDeptId);
	if(_userInfo.size() < 0)
		return;

	m_cbPoliceLog->RemoveAll();

	CListLabelElementUI* pListElement = new CListLabelElementUI;
	pListElement->SetFixedHeight(40);
	pListElement->SetText(_T("--全部--"));
	m_cbPoliceLog->AddAt(pListElement, 0);

	list<IDNAME>::iterator it = _userInfo.begin();
	for(int i = 0;it != _userInfo.end();i++,it ++)
	{
		pListElement = new CListLabelElementUI;
		pListElement->SetTag(i);;

		m_cbPoliceLog->Add(pListElement);
		CDuiString strP;
		strP.Format(_T("%s(%s)"), (*it).policename.c_str(), (*it).policeID.c_str());
		pListElement->SetText(strP);
		pListElement->SetUserData(it->policeID.c_str());
	}
}

//////////////////////////////////////////////////////////////////////////