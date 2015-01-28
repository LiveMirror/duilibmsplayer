#include "stdafx.h"
#include "DvrInfoUI.h"

#include "../../ShareData.h"
#include <sstream> 
#include <fstream>
#include <list>
#include "../../DataBase/QDDvrDB.h"
#include "../../DataBase/DvrList.h"
#include "../../dvr/DvrFactory.h"

CDvrInfoUI::CDvrInfoUI(void)
{
	CDialogBuilder builder;
	CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("DvrInfo.xml"), (UINT)0));
	if( pComputerExamine ) {
		this->Add(pComputerExamine);
	}
	else {
		this->RemoveAll();
		return;
	}
}


CDvrInfoUI::~CDvrInfoUI(void)
{
}

bool CDvrInfoUI::Init( CPaintManagerUI& m_pm) 
{
	m_btnSave = static_cast<CButtonUI*>(m_pm.FindControl(_T("saveDvr")));
	m_pDept = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("cbdeptdvr")));

	m_pDvrList = static_cast<CListUI*>(m_pm.FindControl(_T("dvrlist")));

	m_btnEdit = static_cast<CButtonUI*>(m_pm.FindControl(_T("editDvr")));
	m_btnAdd = static_cast<CButtonUI*>(m_pm.FindControl(_T("addDvr")));
	m_btnDel = static_cast<CButtonUI*>(m_pm.FindControl(_T("deleteDvr")));
	m_btnImport = static_cast<CButtonUI*>(m_pm.FindControl(_T("importDvr")));

	m_btnRefresh = static_cast<CButtonUI*>(m_pm.FindControl(_T("refresh")));

	m_optEnable = static_cast<COptionUI*>(m_pm.FindControl(_T("devenable")));
	m_optDisable = static_cast<COptionUI*>(m_pm.FindControl(_T("devdisable")));

	m_edtPid = static_cast<CEditUI*>(m_pm.FindControl(_T("edtpid")));
	m_edtDevid = static_cast<CEditUI*>(m_pm.FindControl(_T("edtdevid")));
	m_cbMuf = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("cbmuf")));
	m_cbModel = static_cast<CComboBoxUI*>(m_pm.FindControl(_T("cbmodel")));

	CListLabelElementUI* pItem = new CListLabelElementUI;
	pItem->SetText(SHAREDATA.g_strDeptName.c_str());
	pItem->SetUserData(SHAREDATA.g_strDeptId.c_str());
	m_pDept->Add(pItem);
	m_pDept->SelectItem(0);

	OnRefresh();

	InitDvrMM();

	return true;
}

void CDvrInfoUI::Notify(TNotifyUI& msg)
{
	if( msg.sType == DUI_MSGTYPE_CLICK) 
	{
		OnClick(msg);
	}
	else if(msg.sType == DUI_MSGTYPE_SETFOCUS)
	{
		OnFocus(msg);
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		OnItemSelect(msg);
	}
}

void CDvrInfoUI::OnClick(TNotifyUI &msg)
{
	if( msg.pSender == m_btnSave)
	{
		OnSave();
	}
	else if( msg.pSender == m_btnEdit)
	{	
		OnEdit();
	}		

	else if( msg.pSender == m_btnAdd)
	{		
		OnAdd();
	}				

	else if( msg.pSender == m_btnDel)
	{	
		OnDel();	
	}				
	else if( msg.pSender == m_btnImport)
	{
		OnImport();
	}
	else if( msg.pSender == m_btnRefresh)
	{
		OnRefresh();
	}

}

void CDvrInfoUI::OnRefresh()
{
	m_pDvrList->RemoveAll();
	//初始化窗口信息
	list<CTDvr> lsDvr = QD_DB.QueryDvr();
	list<CTDvr>::iterator it = lsDvr.begin();
	for (int i = 0 ; it != lsDvr.end(); ++it, ++i)
	{
		//過濾已刪除的數據
		if((*it).getDelFlag() == 1)
			continue;

		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);;

		m_pDvrList->Add(pListElement);
		pListElement->SetText(0, (*it).getDvrId().c_str());
		pListElement->SetText(1, (*it).getPoliceId().c_str());
	}
	SetEnable(false);
}

void CDvrInfoUI::OnSave()
{
	CTDvr dvr = GetDvrInfo();
	if((dvr.getPoliceId().size() != 6) || (false ==StrIsNum(dvr.getPoliceId())))
	{
		SHAREDATA.g_pMainFrame->Alert(_T("警员id填写有误（6位），请重新填写"));
		m_edtPid->SetFocus();
		return;
	}

	if(true == QD_DB.PutDvr(dvr))
	{
		SHAREDATA.g_pMainFrame->Alert(_T("操作成功！"), _T("提示"));		
		OnRefresh();
		DVR_LIST.Refresh();
	}
	else
	{
		SHAREDATA.g_pMainFrame->Alert(_T("操作失败！"), _T("提示"));		
	}
}

void CDvrInfoUI::OnDel()
{
	if(-1 == m_pDvrList->GetCurSel())
	{
		SHAREDATA.g_pMainFrame->Alert(_T("请先选择要删除的设备"));
		return;
	}

	if(true == SHAREDATA.g_pMainFrame->Alert(_T("确认删除?"),_T("删除")))
	{
		CTDvr dvr = GetDvrInfo();
		dvr.setDelFlag(1);
		//if(true == QD_DB.DeletePoliceInfo( m_edtDevid->GetText().GetData()))
		if(true == QD_DB.PutDvr(dvr))
		{
			SHAREDATA.g_pMainFrame->Alert(_T("删除数据成功！"),_T("提示"));
		}
		int index = m_pDvrList->GetCurSel();
		if(index >= 0)
		{
			CListTextElementUI *pEle = static_cast<CListTextElementUI *>(m_pDvrList->GetItemAt(index));
			m_pDvrList->Remove(pEle);
		}
		ClearDvrInfo();
	}
}

void CDvrInfoUI::OnAdd()
{
	SetEnable(true);
	ClearDvrInfo();
	m_edtDevid->SetFocus();
}

void CDvrInfoUI::OnEdit()
{
	if(-1 == m_pDvrList->GetCurSel())
	{
		SHAREDATA.g_pMainFrame->Alert(_T("请先选择要修改的设备"));
		return;
	}
	SetEnable(true);
}

void CDvrInfoUI::OnImport()
{

}

void CDvrInfoUI::OnFocus(TNotifyUI &msg)
{

}

bool CDvrInfoUI::StrIsNum(const wstring& str)
{
	bool ret = true;
	for (int i = 0;i < str.size();i++)
	{
		if(str[i] < '0' || str[i] > '9')
		{
			ret = false;
			break;
		}
	}
	return ret;
}

void CDvrInfoUI::OnItemSelect(TNotifyUI &msg)
{
	if (msg.pSender == m_pDvrList)
	{
		CListTextElementUI *pEle = static_cast<CListTextElementUI *>(m_pDvrList->GetItemAt(m_pDvrList->GetCurSel()));
		wstring devId =  pEle->GetText(0);
		if(devId.empty())
			return;

		list<CTDvr> lstDvr = QD_DB.QueryDvr(devId);
		if(lstDvr.size() < 1)
			return;

		SetEnable(true);
		SetDvrInfo((*lstDvr.begin()));
		SetEnable(false);
	}
	else if(msg.pSender == m_cbMuf)
	{
		OnItemSelectMuf();
	}
}

void CDvrInfoUI::SetDvrInfo(const CTDvr &dvr)
{
	m_edtDevid->SetText(dvr.getDvrId().c_str());
	m_edtDevid->SetUserData(dvr.getDvrCode().c_str());
	m_edtPid->SetText(dvr.getPoliceId().c_str());
	if(dvr.getEnableFlag() == 1)
	{
		m_optEnable->Selected(true);
	}
	else
	{
		m_optDisable->Selected(true);
	}

	m_cbMuf->SetEnabled(true);
	int index = 0;
	CDuiString strt = dvr.getManufacturer().c_str();
	for( int i = 0 ; i < m_cbMuf->GetCount(); ++i)
	{
		if(m_cbMuf->GetItemAt(i)->GetText() == strt)
		{
			m_cbMuf->SelectItem(i);
			break;
		}
	}

	m_cbModel->SetEnabled(true);
	index = 0;
	for (int i = 0; i < m_cbModel->GetCount(); ++i)
	{
		if(m_cbModel->GetItemAt(i)->GetText() == dvr.getModel().c_str())
		{
			m_cbModel->SelectItem(i);
			break;;
		}
	}
}

CTDvr CDvrInfoUI::GetDvrInfo()
{
	CTDvr dvr;
	dvr.setDvrId( m_edtDevid->GetText().GetData());
	dvr.setDvrCode( m_edtDevid->GetUserData().GetData());
	dvr.setPoliceId(m_edtPid->GetText().GetData());
	dvr.setEnableFlag(m_optEnable->IsSelected());
	dvr.setManufacturer( m_cbMuf->GetText().GetData());
	dvr.setModel(m_cbModel->GetText().GetData());
	dvr.setUnit(SHAREDATA.g_strDeptId);
	dvr.setDelFlag(0);
	return dvr;
}

void CDvrInfoUI::ClearDvrInfo()
{
	m_edtDevid->SetText(_T(""));
	m_edtPid->SetText(_T(""));
	m_optDisable->Selected(true);
	m_cbMuf->SetText(_T(""));
	m_cbModel->SetText(_T(""));
}

void CDvrInfoUI::SetEnable(bool isEnable)
{
	m_edtDevid->SetEnabled(isEnable);
	m_edtPid->SetEnabled(isEnable);
	m_optEnable->SetEnabled(isEnable);
	m_optDisable->SetEnabled(isEnable);
	m_cbMuf->SetEnabled(isEnable);
	m_cbModel->SetEnabled(isEnable);

	m_btnEdit->SetEnabled(!isEnable);
	m_btnAdd->SetEnabled(!isEnable);
	m_btnDel->SetEnabled(!isEnable);

	m_btnSave->SetVisible(isEnable);
}

void CDvrInfoUI::InitDvrMM()
{
	vector<wstring> vecMuf = DVR_FACTORY.GetMufs();
	for (int i = 0; i < vecMuf.size(); ++i)
	{
		CListLabelElementUI* pItem = new CListLabelElementUI;
		pItem->SetText(vecMuf[i].c_str());
		m_cbMuf->Add(pItem);
	}
}

void CDvrInfoUI::OnItemSelectMuf()
{
	int index = m_cbMuf->GetCurSel();
	if(index >= 0 && index < m_cbMuf->GetCount())
	{
		vector<wstring> vecModels = DVR_FACTORY.GetModels(m_cbMuf->GetItemAt(index)->GetText().GetData());
		m_cbModel->RemoveAll();
		for ( int i = 0; i < vecModels.size(); ++i)
		{
			CListLabelElementUI* pItem = new CListLabelElementUI;
			pItem->SetText(vecModels[i].c_str());
			m_cbModel->Add(pItem);
		}
	}
}