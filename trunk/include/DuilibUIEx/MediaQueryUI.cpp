#include "stdafx.h"
#include "MediaQueryUI.h"
#include "../../DataBase/QDDvrDB.h"
#include "../../WellCommon/DataConvert.h"
#include "../../ShareData.h"
#include "ListEx.h"
#include "../../WellCommon/CommUtils.h"
#include "../UIHelper.h"

CMediaQueryUI::CMediaQueryUI()
{
	CDialogBuilder builder;
	CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("InformationQuery.xml"), (UINT)0));
	if( pComputerExamine ) {
		this->Add(pComputerExamine);
	}
	else {
		this->RemoveAll();
		return;
	}
}

CMediaQueryUI::~CMediaQueryUI()
{

}

bool CMediaQueryUI::Init(HWND hWnd, CPaintManagerUI& m_pm)
{
	m_pHwnd = hWnd;

	//显示消息
	m_pLblMsg = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblmsg")));	

	m_btnQuery = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnSearch")));
	m_pList = static_cast<CListUIEx*>(m_pm.FindControl(_T("lstinfo")));
	m_Cclass = static_cast<CComboUI*>(m_pm.FindControl(_T("comclass")));
	m_CUser= static_cast<CComboUI*>(m_pm.FindControl(_T("comuser")));
	m_CType = static_cast<CComboUI*>(m_pm.FindControl(_T("comtype")));

	m_cbYearS = static_cast<CComboUI*>(m_pm.FindControl(_T("cbYearS")));
	m_cbMonS = static_cast<CComboUI*>(m_pm.FindControl(_T("cbMonthS")));
	m_cbDayS = static_cast<CComboUI*>(m_pm.FindControl(_T("cbDayS")));
	m_cbYearE = static_cast<CComboUI*>(m_pm.FindControl(_T("cbYearE")));
	m_cbMonE = static_cast<CComboUI*>(m_pm.FindControl(_T("cbMonthE")));
	m_cbDayE = static_cast<CComboUI*>(m_pm.FindControl(_T("cbDayE")));

	CScrollBarUI* pScroll;
	pScroll =  m_pList->GetVerticalScrollBar();
	SetVScrollBar(pScroll);

	InitMediaQueryPageData();

	return true;
}

void CMediaQueryUI::Notify(TNotifyUI& msg)
{
	if( msg.sType == DUI_MSGTYPE_CLICK ) 
	{
		if( msg.pSender == m_btnQuery)
		{
			OnBtnMediaQuery();
		}
		else if(msg.pSender->GetName() == _T("play"))
		{
			OnPlay(msg.pSender);
		}
	}
	else if( msg.sType == DUI_MSGTYPE_DROPDOWN)
	{
		if(msg.pSender == m_CUser)
		{
			//RefreshPolices();
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//媒体查询
//////////////////////////////////////////////////////////////////////////
list<CTMediaInfo> CMediaQueryUI::GetMediaInfo(const wstring& szValue)
{
	return QD_DB.QueryMediaInfo(szValue.c_str());
}

bool CMediaQueryUI::ShowMediaQueryData(list<CTMediaInfo>& listData)
{
	if(listData.size() > 0)
	{
		wstring strSuff;
		list<CTMediaInfo>::iterator it = listData.begin();
		for(int index = 0;it != listData.end();index ++,it ++)
		{
			CListContainerElementUI* pListElement = AddLine();

			strSuff = (*it).getSuffixName();
			pListElement->GetItemAt(0)->SetText((*it).getPId().c_str());
			pListElement->GetItemAt(1)->SetText((*it).getUploadTime().c_str());
			pListElement->GetItemAt(2)->SetText((*it).getFileName().c_str());
			pListElement->GetItemAt(2)->SetUserData((*it).getLocalPath().c_str());

			pListElement->GetItemAt(3)->SetText(strSuff.c_str());
			pListElement->GetItemAt(4)->SetText((*it).getCreateTime().c_str());

			pListElement->GetItemAt(7)->SetText(TO_WSTRING((*it).getFrameRate()).c_str());
			pListElement->GetItemAt(8)->SetText((TO_WSTRING((*it).getWidth()) + _T("*") + TO_WSTRING((*it).getHeight())).c_str());
			CHorizontalLayoutUI* pOpt = static_cast<CHorizontalLayoutUI*>(pListElement->GetItemAt(9));
			CButtonUI* pBtn = NULL;
			if (pOpt)
			{
				pBtn = static_cast<CButtonUI*>(pOpt->GetItemAt(0));
				pBtn->SetVisible(true);
			}

			if(strSuff.compare(_T("AVI")) == 0 || strSuff.compare(_T("avi")) == 0
				|| strSuff.compare(_T("WAV")) == 0 || strSuff.compare(_T("wav")) == 0)
			{
				pBtn->SetText(_T("播放"));
				int len = (*it).getLength();
				CDuiString strVLen;
				strVLen.Format(_T("%d时%d分%d秒"), len / 3600, len % 3600 / 60, len % 3600 % 60);

				pListElement->GetItemAt(5)->SetText(TO_WSTRING((*it).getFileSize()).c_str());
				pListElement->GetItemAt(6)->SetText(strVLen);
			}
			else if(strSuff.compare(_T("JPG")) == 0 || strSuff.compare(_T("jpg")) == 0)
			{
				pBtn->SetText(_T("查看"));
				pListElement->GetItemAt(5)->SetText(TO_WSTRING((*it).getFileSize()).c_str());
				pListElement->GetItemAt(6)->SetText(_T(""));
			}
			else if(strSuff.compare(_T("TXT")) == 0 || strSuff.compare(_T("txt")) == 0)
			{
				pBtn->SetVisible(false);
				pListElement->GetItemAt(5)->SetText(TO_WSTRING((*it).getFileSize()).c_str());
				pListElement->GetItemAt(6)->SetText(_T(""));
			}
			else
			{
				pBtn->SetVisible(false);
				pListElement->GetItemAt(5)->SetText(TO_WSTRING((*it).getFileSize()).c_str());
				pListElement->GetItemAt(6)->SetText(_T(""));
			}
		}
	}
	return true;
}

void CMediaQueryUI::OnBtnMediaQuery()
{
	wstring tFileType =  m_CType->GetText();
	if(tFileType == _T("--全部--")) tFileType = _T("");
	else if(tFileType == _T("视频"))	tFileType = _T("video");
	else if(tFileType == _T("图片"))	tFileType = _T("picture");
	else if(tFileType == _T("音频"))	tFileType = _T("audio");
	else if(tFileType == _T("日志"))	tFileType = _T("log");

	CDuiString strDateS = m_cbYearS->GetText() + _T("-") + m_cbMonS->GetText() + _T("-") + m_cbDayS->GetText() + _T(" 00:00:01");
	CDuiString strDateE = m_cbYearE->GetText()+ _T("-") + m_cbMonE->GetText() + _T("-") + m_cbDayE->GetText() + _T(" 59:59:59");

	QueryParam tQueryParam;
	if(m_CUser->GetCurSel() >= 0)
	{
		tQueryParam.policeID = m_CUser->GetItemAt(m_CUser->GetCurSel())->GetUserData();
		if(tQueryParam.policeID.length() == 6)
		{
			tQueryParam.policeID = _T("00") + tQueryParam.policeID;
		}
	}
	else
	{
		tQueryParam.policeID.clear();
	}
	
	if(tQueryParam.policeID == _T("--全部--"))
		tQueryParam.policeID.clear();

	tQueryParam.mediaType = tFileType;
	tQueryParam.startTime = strDateS.GetData();
	tQueryParam.endTime = strDateE.GetData();
	list<CTMediaInfo> tList = QD_DB.QueryMediaInfoWhole(tQueryParam);

	m_pList->RemoveAll();
	ShowMediaQueryData(tList);

	if( m_pLblMsg )
	{
		wstring tShowMessage(_T("查询结果数："));
		tShowMessage += TO_WSTRING(tList.size());
		m_pLblMsg->SetText(tShowMessage.c_str());
	}

	if(tList.size() == 0)
	{
		WELLUI.Alert(_T("没有您想要的数据！\r\n\r\n变更查询条件再试试！"), _T("提示"), _T(""), m_pHwnd);
	}
}

bool CMediaQueryUI::InitMediaQueryPageData()
{
	CListLabelElementUI* pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetText(_T("--全部--"));
	pListLabelelement->SetFixedHeight(50);
	m_CType->AddAt(pListLabelelement,0);

	pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetText(_T("视频"));
	pListLabelelement->SetFixedHeight(50);
	m_CType->AddAt(pListLabelelement,1);

	pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetText(_T("图片"));
	pListLabelelement->SetFixedHeight(50);
	m_CType->AddAt(pListLabelelement,2);

	pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetText(_T("音频"));
	pListLabelelement->SetFixedHeight(50);
	m_CType->AddAt(pListLabelelement,3);

	pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetText(_T("日志"));
	pListLabelelement->SetFixedHeight(50);
	m_CType->AddAt(pListLabelelement,4);
	m_CType->SelectItem(1);

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
		m_cbYearS->AddAt(pListLabelelement, i);
		CListLabelElementUI* pLLE = new CListLabelElementUI(*pListLabelelement);
		m_cbYearE->AddAt(pLLE, i);
	}
	for(int i = 0; i < 12; ++i)
	{
		CListLabelElementUI* pListLabelelement = new CListLabelElementUI; 
		pListLabelelement->SetText(TO_WSTRING(i + 1).c_str());
		pListLabelelement->SetFixedHeight(40);
		m_cbMonE->AddAt(pListLabelelement, i );
		CListLabelElementUI* pLLE = new CListLabelElementUI(*pListLabelelement);
		m_cbMonS->AddAt(pLLE, i);
	}
	for(int i = 0; i < 31; ++i)
	{
		CListLabelElementUI* pListLabelelement = new CListLabelElementUI; 
		pListLabelelement->SetText(TO_WSTRING(i + 1).c_str());
		pListLabelelement->SetFixedHeight(40);
		m_cbDayS->AddAt(pListLabelelement, i);
		CListLabelElementUI* pLLE = new CListLabelElementUI(*pListLabelelement);
		m_cbDayE->AddAt(pLLE, i);
	}

	m_cbYearE->SelectItem(0);
	m_cbMonE->SelectItem(mon - 1);
	m_cbDayE->SelectItem(day - 1);
	CTimeSpan ts(7,0,0,0);
	CTime tS = tm - ts;
	m_cbYearS->SelectItem(tm.GetYear() - tS.GetYear());
	m_cbMonS->SelectItem(tS.GetMonth() - 1);
	m_cbDayS->SelectItem(tS.GetDay() - 1);
	//////////////////////////////////////////////////////////////////////////

	CListLabelElementUI* pItem = new CListLabelElementUI;
	pItem->SetText(SHAREDATA.g_strDeptName.c_str());
	pItem->SetUserData(SHAREDATA.g_strDeptId.c_str());
	m_Cclass->Add(pItem);
	m_Cclass->SelectItem(0);

	RefreshPolices();

	return true;
}

CListContainerElementUI* CMediaQueryUI::AddLine()
{
	CDialogBuilder builder;
	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("media_data_list_item.xml"),(UINT)0));
	if( pLine != NULL ) 
	{
		m_pList->InsertItem(m_pList->GetCount(), 60, pLine);
	}
	return pLine;
}

void CMediaQueryUI::OnPlay(CControlUI* pSender)
{
	CListContainerElementUI* pLine = static_cast<CListContainerElementUI*>(pSender->GetParent()->GetParent());
	if(pLine)
	{
		CDuiString strFile = pLine->GetItemAt(2)->GetUserData();
		CDuiString strType = pLine->GetItemAt(3)->GetText();
		if(!strFile.IsEmpty())
		{
			if(strType == _T("AVI") || strType == _T("avi"))
			{
				WELLUI.PlayWnd(m_pHwnd, strFile.GetData());
				//CCommUtils::PlayVideo((TCHAR*)strFile.GetData());
			}
			else if(strType == _T("JPG") || strType == _T("jpg"))
			{
				WELLUI.PlayWnd(m_pHwnd, strFile.GetData());
				//CCommUtils::ShowPictureWindow((TCHAR*)strFile.GetData());
			}
			else
			{
				//ShellExecute(NULL, _T("open"), strFile.GetData(), NULL, NULL, 0);
			}
			CDuiString strMsg;
			strMsg.Format(_T("%s 文件:[%s]"),  pSender->GetText().GetData(), strFile.GetData());
			LOG_DB(strMsg.GetData());
		}
	}
}

void CMediaQueryUI::RefreshPolices()
{
	list<IDNAME> _userInfo;
	_userInfo = QD_DB.QueryUserInfoWithDebID(SHAREDATA.g_strDeptId);
	if(_userInfo.size() < 0)
		return;

	m_CUser->RemoveAll();

	CListLabelElementUI* pListLabelelement = new CListLabelElementUI; 
	pListLabelelement->SetText(_T("--全部--"));
	pListLabelelement->SetFixedHeight(40);
	m_CUser->AddAt(pListLabelelement,0);

	list<IDNAME>::iterator it = _userInfo.begin();
	for(int i = 0;it != _userInfo.end();i++,it ++)
	{
		CListLabelElementUI* pListElement = new CListLabelElementUI;
		pListElement->SetTag(i);;

		m_CUser->Add(pListElement);		
		CDuiString strP;
		strP.Format(_T("%s(%s)"), (*it).policename.c_str(), (*it).policeID.c_str());
		pListElement->SetText(strP);
		pListElement->SetUserData(it->policeID.c_str());
	}
}
