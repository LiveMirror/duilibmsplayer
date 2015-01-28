#include "StdAfx.h"
#include <vector>
#include <math.h>
#include "../../DataDef.h"
#include "DvrListUI.h"
#include "../UIHelper.h"
#include "../../ShareData.h"
#include "../../DvrWndMgr.h"
#include "../../DataBase/QDDvrDB.h"
#include "../../config/DvrBindConfig.h"
#include "../../DataBase/DvrList.h"
#include "../../WellCommon/CommUtils.h"

//DVR图片的宽高比例
#define DVR_IMG_SCALE ((float)243/401)
//充电gif的比例
#define CD_IMG_SCALE ((float)167/80)

inline double CalculateDelay(double state) {
	return pow(state, 2);
}

DvrListUI::DvrListUI()
	: CDvrBaseUI(SHAREDATA.g_dvrProp.numRow, SHAREDATA.g_dvrProp.numCol)
	, m_enableClick(0)
{
	for(int i = 0; i < SHAREDATA.g_dvrProp.numDvr; ++i) 
	{
		CDialogBuilder builder;
		CContainerUI* pDesk = static_cast<CContainerUI*>(builder.Create(_T("dvr.xml"), (UINT)0));
		if( pDesk ) 
		{
			pDesk->SetName(_T("dvr") + i);
			this->Add(pDesk);
			
			//初始化所有控件的指针
			int row = i / m_nCols;
			int col = i % m_nCols;
			m_mapItem[i] = InitDvrItemCtrl(row, col);
		}
		else 
		{
			this->RemoveAll();
			return;
		}
	}

}

DvrListUI::~DvrListUI()
{
	//UnInit();
}

void DvrListUI::UnInit()
{
	this->RemoveAll();
}

void DvrListUI::SetPos(RECT rc)
{
	if( GetCount() > 0 ) 
	{
		//固定设为4列
		SetColumns(m_nCols);
	}

	CTileLayoutUI::SetPos(rc);

	SetDvrPos(rc);
}

void DvrListUI::SetDvrPos(RECT rc)
{
	//最小化到任务栏时
	if(rc.top == rc.bottom)
	{
		return;
	}

	//固定列数设置为4的方法
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	
	//行列添加间隙
	int nOffSet = 10;
	
	width = (width - nOffSet*(m_nCols + 1)) / m_nCols;
	height = (height - nOffSet*(m_nRows + 1)) / m_nRows;
	//值的宽度 = 单个dvr的宽度　－　标签30 - 间隔10 - 图片的宽
	int widVal = width - height * DVR_IMG_SCALE - 20;
	for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_items[it1]);
		if( !pControl->IsVisible() ) 
			continue;
		if( pControl->IsFloat() ) {
			SetFloatPos(it1);
			continue;
		}

		RECT rcPos;
		rcPos.left = (it1%m_nCols)*(width) + nOffSet; //4是左边距
		if(it1 % m_nCols < m_nCols)
		{
			rcPos.left += nOffSet * (it1 % m_nCols);
		}
		rcPos.right = rcPos.left + width;
		rcPos.top = (it1/m_nCols)*height + 85 + nOffSet;  //90是标题的高度
		if(it1/m_nRows < m_nCols)
		{
			rcPos.top += nOffSet * ((it1 / m_nCols) + 1);
		}
		rcPos.bottom = rcPos.top + height;

		pControl->SetPos(rcPos);

		//注这边不能用findctrol,因为所有的dvr子控件的id是一样的，会造成只有最后一个有效
		//这里和相关的布局层数有关系
		CVerticalLayoutUI* pDvr = static_cast<CVerticalLayoutUI*>(pControl);

		//设置圆角
		pDvr->SetBorderSize(1);
		SIZE sz;
		sz.cx = 5;
		sz.cy = 5;
		pDvr->SetBorderRound(sz);

		CHorizontalLayoutUI* pParent = static_cast<CHorizontalLayoutUI*>(pDvr->GetItemAt(0));
		if(pParent)
		{
			pParent->SetFixedHeight(height *2/ 5);
			pParent->SetFixedWidth(width);

			CVerticalLayoutUI* pImg = static_cast<CVerticalLayoutUI*>(pParent->GetItemAt(0));
			if(pImg)
			{
				pImg->SetFixedWidth(width / 3);

				//设置GIF
				CHorizontalLayoutUI* pHLGif = static_cast<CHorizontalLayoutUI*>(pImg->GetItemAt(0));
				CContainerUI* pCon = static_cast<CContainerUI*>(pHLGif->GetItemAt(0));
				CButtonGifUI* pGif = static_cast<CButtonGifUI*>(pCon->GetItemAt(0));
				if(pGif)
				{
					RECT rcInset = pHLGif->GetInset();
					int gW = width / 3 - (rcInset.left + rcInset.right);		//减去边距
					int gH = gW / CD_IMG_SCALE;
					pGif->SetFixedWidth(gW);
					pGif->SetFixedHeight(gH);
				}
			}

			DvrItemCtrl dic = m_mapItem[it1];
			if(dic.pValue)		dic.pValue->SetFixedWidth(widVal);
			if(dic.pProgress)
			{
				RECT rcPro;
				//使进度条对齐
				rcPro.top = dic.pProgress->GetParent()->GetHeight();
				dic.pProgress->SetFixedHeight(rcPro.top * 0.8);
				rcPro.top = rcPro.bottom = rcPro.top * 0.1;
				rcPro.left = rcPro.right = 5;
				dic.pProgress->SetPadding(rcPro);
				if(dic.pProgress->GetHeight() > 30)
				{
					rcPro.top = dic.pProgress->GetHeight() * 8 / 32;
					dic.pProgress->SetTextPadding(rcPro);
				}
			}
		}

		//下半部分占2/5
		CVerticalLayoutUI* pvlUI3 = static_cast<CVerticalLayoutUI*>(pDvr->GetItemAt(2));
		if(pvlUI3)
		{
			pvlUI3->SetFixedWidth(width);
			int nH = height / 2;
			pvlUI3->SetFixedHeight(nH);
			CHorizontalLayoutUI* pItem0 = static_cast<CHorizontalLayoutUI*>(pvlUI3->GetItemAt(0));
			if(pItem0)
			{
				pItem0->SetFixedHeight(nH / 4 - 1);
			}
			CHorizontalLayoutUI* pItem1 = static_cast<CHorizontalLayoutUI*>(pvlUI3->GetItemAt(1));
			if(pItem1)
			{
				pItem1->SetFixedHeight(nH / 4 - 1);
			}
			CHorizontalLayoutUI* pItem2 = static_cast<CHorizontalLayoutUI*>(pvlUI3->GetItemAt(2));
			if(pItem2)
			{
				pItem2->SetFixedHeight(nH / 4 - 1);
				CProgressUI * pPro = static_cast<CProgressUI*>(pItem2->GetItemAt(1));
				if(pPro)
				{
					pPro->SetFixedHeight(nH / 4 - 3);
				}
			}
			CHorizontalLayoutUI* pItem3 = static_cast<CHorizontalLayoutUI*>(pvlUI3->GetItemAt(3));
			if(pItem3)
			{
				pItem3->SetFixedHeight(nH / 4 + 3);
			}
		}
	}
}

void DvrListUI::OnLBClick(const POINT& pt)
{
	if(SHAREDATA.g_pMainFrame == NULL)
		return;

	CControlUI* pControl = HitPtInItem(pt);
	if( NULL == pControl)
	{
		return;
	}

	CDvrWndUI* pDvrWnd = GetCurDvrWndPtr();
	if(pDvrWnd == NULL)
	{
		return;
	}

	DvrItemCtrl dic = GetDvrItemCtrl(m_selectRow, m_selectCol);
	CDuiString strId = dic.pId->GetText();
	
	HWND tHandle = SHAREDATA.g_pMainFrame->GetHWND();

	if(!strId.IsEmpty() && !CCommUtils::IsNumber0(strId.GetData()))
	{
		bool bReg = DVR_LIST.HasDvr(strId.GetData());
		if(!bReg)
		{	
			if(WELLUI.Check(SHAREDATA.g_strPasswod.c_str(), tHandle, _T("请输入工作站密码")))
			{
				bool bRlt = pDvrWnd->SetRegistered();
				//WELLUI.Alert(bRlt ? _T("执法仪注册成功") : _T("执法仪注册失败"), _T("提示"), _T(""), tHandle);
			}
			else
			{
				//WELLUI.Alert(_T("执法仪注册失败"), _T("提示"), _T(""), tHandle);
			}
		}
		else
		{
			//查询上传数据
			list<CTMediaInfo> listInfo = QD_DB.QueryMediaInfo(strId.GetData());
			if(listInfo.size() > 0)
			{
				WELLUI.ShowDvrDataWnd(tHandle, &listInfo);
			}
		}
	}
}
void DvrListUI::OnRBClick(const POINT& pt)
{	
	if(SHAREDATA.g_pMainFrame == NULL)
		return;

	CControlUI* pControl = HitPtInItem(pt);
	if( NULL == pControl)
	{
		return;
	}

// 	CDvrWndUI* pDvrWnd = GetCurDvrWndPtr();
// 	if(pDvrWnd && pDvrWnd->GetDevPtr() && pDvrWnd->IsEnable() && pDvrWnd->IsModifyStatus())
// 	{
// 		HWND tHandle = SHAREDATA.g_pMainFrame->GetHWND();
// 		WELLUI.DeviceWnd(tHandle, pDvrWnd);
// 		return;
// 	}
}

//改变点击项的边框颜色
void DvrListUI::HitItem(CControlUI* pItem)
{
	pItem->SetBorderColor(CLR_ACTIVR_BORDER);
	pItem->SetBkColor(CLR_ACTIVR_BK);

	//把其它的颜色改回蓝色
	for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) 
	{
		CControlUI* pControlT = static_cast<CControlUI*>(m_items[it1]);
		if( !pControlT->IsVisible() ) 
			continue;
		if( pControlT->IsFloat() ) {
			SetFloatPos(it1);
			continue;
		}

		if(pItem != pControlT)
		{
			pControlT->SetBorderColor(CLR_ACTIVR_BORDER);
			//背景色
			pControlT->SetBkColor(CLR_ACTIVR_BK);
		}
	}
}

DvrItemCtrl DvrListUI::InitDvrItemCtrl(int row, int col)
{	
	DvrItemCtrl dic;
	memset(&dic, 0, sizeof(DvrItemCtrl));
	
	CContainerUI* pControl = GetItem(row, col);
	if(pControl == NULL)
	{
		return dic;
	}

	//级数： 即item at的索引， 看dvr.xml的嵌套层数
	CHorizontalLayoutUI* pParent = static_cast<CHorizontalLayoutUI*>(pControl->GetItemAt(0));
	if(pParent == NULL)
		return dic;

	//////////////////////////////////////////////////////////////////////////
	//上半部分
	CVerticalLayoutUI* pImg = static_cast<CVerticalLayoutUI*>(pParent->GetItemAt(0));
	if(pImg)
	{
		dic.pImg = pImg;
		CVerticalLayoutUI* pImg0 = static_cast<CVerticalLayoutUI*>(pImg->GetItemAt(0));
		if(pImg0)
		{
			//里面包含着CButtonGifUI
			CContainerUI* pGif = static_cast<CContainerUI*>(pImg0->GetItemAt(0));
			if(pGif)
			{
				dic.pGif = pGif;
				//dic.pGif = static_cast<CButtonGifUI*>(pGif->GetItemAt(0));
				pGif->SetVisible(false);
			}
			dic.pDev = static_cast<CTextUI*>(pImg0->GetItemAt(1));
		}
	}

	//分割符
	CVerticalLayoutUI* pSplit = static_cast<CVerticalLayoutUI*>(pParent->GetItemAt(1));

	dic.pValue = static_cast<CVerticalLayoutUI*>(pParent->GetItemAt(2));
	if(dic.pValue)
	{
		CVerticalLayoutUI* pValue = dic.pValue;

		int index = 0;
		CHorizontalLayoutUI* pItem = static_cast<CHorizontalLayoutUI*>(pValue->GetItemAt(index));
		if(pItem)
		{
			dic.pId = static_cast<CLabelUI*>(pItem->GetItemAt(1));
		}
		index+=1;
		pItem = static_cast<CHorizontalLayoutUI*>(pValue->GetItemAt(index));
		if(pItem)
		{
			dic.pName = static_cast<CLabelUI*>(pItem->GetItemAt(1));
		}
		index+=1;
		pItem = static_cast<CHorizontalLayoutUI*>(pValue->GetItemAt(index));
		if(pItem)
		{
			dic.pDisk = static_cast<CLabelUI*>(pItem->GetItemAt(1));
		}
		index+=1;
		pItem = static_cast<CHorizontalLayoutUI*>(pValue->GetItemAt(index));
		if(pItem)
		{
			dic.pSpeed = static_cast<CLabelUI*>(pItem->GetItemAt(1));
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	//下半部分
	pParent = static_cast<CHorizontalLayoutUI*>(pControl->GetItemAt(2));
	if(pParent == NULL)
		return dic;

	int index = 0;
	CHorizontalLayoutUI* pItem = static_cast<CHorizontalLayoutUI*>(pParent->GetItemAt(index));
	if(pItem)
	{
		dic.pStatus = static_cast<CLabelUI*>(pItem->GetItemAt(1));
	}
	index +=1;
	pItem = static_cast<CHorizontalLayoutUI*>(pParent->GetItemAt(index));
	if(pItem)
	{
		dic.pFile = static_cast<CLabelUI*>(pItem->GetItemAt(1));
	}
	index+=1;
	pItem = static_cast<CHorizontalLayoutUI*>(pParent->GetItemAt(index));
	if(pItem)
	{
		dic.pProgress = static_cast<CProgressUI*>(pItem->GetItemAt(1));
	}
	index+=1;
	pItem = static_cast<CHorizontalLayoutUI*>(pParent->GetItemAt(index));
	if(pItem)
	{
		CHorizontalLayoutUI* pI = static_cast<CHorizontalLayoutUI*>(pItem->GetItemAt(0));
		if(pI)
			dic.pCount = static_cast<CButtonUI*>(pI->GetItemAt(1));
		pI = static_cast<CHorizontalLayoutUI*>(pItem->GetItemAt(1));
		if(pI)
			dic.pSize = static_cast<CButtonUI*>(pI->GetItemAt(1));
	}

	return dic;
}

DvrItemCtrl DvrListUI::GetDvrItemCtrl(int row, int col)
{
	int index = row * m_nCols + col;
	return m_mapItem[index];
}

void DvrListUI::SetInfo(CDvrWndUI* pDvr)
{
	DvrItemCtrl dic = GetDvrItemCtrl(pDvr->GetRow(), pDvr->GetCol());
	if(pDvr->IsActive())
	{
		//显示
		SetInfo_Active(dic, pDvr);
		dic.pStatus->SetTextColor(CLR_ACTIVR_BORDER);
	}
	else
	{
		SetInfo_Nomal(dic, pDvr);
		dic.pStatus->SetTextColor(CLR_NOMAL_BORDER);
	}

}

void DvrListUI::SetInfo_Active(const DvrItemCtrl& dic, CDvrWndUI* dvr)
{
	dic.pDev->SetText(_T("工作中..."));
	dic.pDev->SetVisible(false);

	dic.pId->SetText(dvr->m_data.strId.c_str());
	dic.pName->SetText(dvr->m_data.strName.c_str());
	dic.pDisk->SetText(dvr->m_data.strDisk.c_str());


	if(dvr->m_data.nFileCount >= 0)
	{
		dic.pProgress->SetVisible(true);
		dic.pProgress->SetMinValue(0);
		int _value = dvr->m_data.nTransFileCount;
		dic.pProgress->SetValue(_value);
		dic.pProgress->SetMaxValue(dvr->m_data.nFileCount);
	}

	CDuiString strT;
	strT.Format(_T("%d"),  dvr->m_data.nTransFileCount);
	dic.pCount->SetText(strT);

	dic.pSpeed->SetText(dvr->m_data.strSpeed.c_str());
	dic.pStatus->SetText(dvr->m_data.strStatus.c_str());
	dic.pFile->SetText(dvr->m_data.strFile.c_str());
	dic.pGif->SetVisible(true);

	CString strSize;
	LONGLONG lnSize = dvr->GetTransFileSize();
	double  fsize = (double)lnSize / 1024.0 / 1024.0;
	strSize.Format(_T("%.0fMB"), fsize);
	dic.pSize->SetText(strSize);
}

void DvrListUI::SetInfo_Nomal(const DvrItemCtrl& dic, CDvrWndUI* dvr)
{
	if(!dvr->IsLocaltion())
	{
		dic.pDev->SetTextColor(0xFF0000FF);
		dic.pDev->SetText(_T("未定位"));
	}
	else
	{
		if(SHAREDATA.g_dbgLevel == LOCATION)
		{
			dic.pDev->SetTextColor(0xFF00FF00);
			dic.pDev->SetText(_T("已定位"));
		}
		else
			dic.pDev->SetText(_T("无设备"));
	}

	dic.pId->SetText(dvr->m_data.strId.c_str());
	dic.pName->SetText(dvr->m_data.strName.c_str());
	dic.pDisk->SetText(dvr->m_data.strDisk.c_str());

	dic.pProgress->SetVisible(false);
	int _value = dvr->m_data.nTransFileCount;
	dic.pProgress->SetValue(_value);
	dic.pProgress->SetText(_T(""));

	dic.pSpeed->SetText(_T(""));
	dic.pStatus->SetText(dvr->m_data.strStatus.c_str());
	dic.pFile->SetText(dvr->m_data.strFile.c_str());
	dic.pGif->SetVisible(false);
	dic.pDev->SetVisible(true);
	dic.pSize->SetText(_T(""));
	dic.pCount->SetText(_T(""));
}

void DvrListUI::OnBtnClick(TNotifyUI& msg)
{
	CDuiString name = msg.pSender->GetName();
	if(name == _T("btnStop"))
	{
		int index = -1;
		MapDvrCtrlItor it = m_mapItem.begin();
		for( ; it != m_mapItem.end(); ++it)
		{
//			if( (*it).second.pStop == msg.pSender)
			{
				index = (*it).first;
				break;
			}
		}
		
		//StopDvr(index);
	}
}



CDvrWndUI* DvrListUI::GetDvrWndPtr(int index)
{
	return CDvrWndMgr::Instance().GetDvrWndUIPtr(index);
}

CDvrWndUI* DvrListUI::GetCurDvrWndPtr()
{
	int index = m_selectRow * m_nCols + m_selectCol;
	return GetDvrWndPtr(index);
}