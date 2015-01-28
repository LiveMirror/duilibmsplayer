#include "stdafx.h"
#include "DvrBindItemUI.h"
#include "../../ShareData.h"
#include "../../config/DvrBindConfig.h"

CDvrBindItemUI::CDvrBindItemUI(void)
	: CDvrBaseUI(SHAREDATA.g_dvrProp.numRow, SHAREDATA.g_dvrProp.numCol)
{
	for(int i = 0; i < SHAREDATA.g_dvrProp.numDvr; ++i) 
	{
		CDialogBuilder builder;
		CContainerUI* pDesk = static_cast<CContainerUI*>(builder.Create(_T("DvrBindItem.xml"), (UINT)0));
		if( pDesk ) 
		{
			pDesk->SetName(_T("dvrbinditem") + i);
			//上面为创建DvrBindItem.xml控件，下面将他们加入容器中
			this->Add(pDesk);

			//初始化所有控件的指针
			int row = i / m_nCols;
			int col = i % m_nCols;
			m_mapItem[i] = InitDBICtrl(row, col);  //得到每个控件中的所有子空间的指针
		}
		else 
		{
			this->RemoveAll();
			return;
		}
	}

	LoadData();//得到配置文件中的绑定信息
}

CDvrBindItemUI::~CDvrBindItemUI(void)
{
	SaveData(); //将绑定信息写入到文件中
}

DBICtrl CDvrBindItemUI::InitDBICtrl(int row, int col)
{
	DBICtrl dic;
	memset(&dic, 0, sizeof(DBICtrl));

	CContainerUI* pControl = GetItem(row, col);
	if(pControl == NULL)
	{
		return dic;
	}

	CHorizontalLayoutUI* pItem = static_cast<CHorizontalLayoutUI*>(pControl->GetItemAt(0));
	if(pItem)
	{
		dic.pWndEnable = static_cast<COptionUI*>(pItem->GetItemAt(0));
	}

	pItem = static_cast<CHorizontalLayoutUI*>(pControl->GetItemAt(1));
	if(pItem)
	{
		dic.pModifyInfo = static_cast<COptionUI*>(pItem->GetItemAt(0));
	}

	pItem = static_cast<CHorizontalLayoutUI*>(pControl->GetItemAt(2));
	if(pItem)
	{
		dic.pBindInfo = static_cast<COptionUI*>(pItem->GetItemAt(0));
	}

	pItem = static_cast<CHorizontalLayoutUI*>(pControl->GetItemAt(3));
	if(pItem)
	{
		dic.pRegDvr = static_cast<COptionUI*>(pItem->GetItemAt(0));
	}

	return dic;
}

DBICtrl CDvrBindItemUI::GetDBICtrl(int row, int col)
{
	int index = row * m_nCols + col;
	return m_mapItem[index];
}

void CDvrBindItemUI::SetPos(RECT rc)
{
	//得到控件的数量
	if( GetCount() > 0 ) 
	{
		//固定设为4列
		SetColumns(m_nCols);
	}

	//固定列数设置为4的方法
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	//行列添加间隙
	int nOffSet = 5;

	width = (width - nOffSet*(m_nCols+ 1)) / m_nCols;
	height = (height - nOffSet*(m_nRows + 1)) / m_nRows;

	for( int it1 = 0; it1 < m_items.GetSize() ; it1++ ) 
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
		rcPos.top = (it1/m_nCols)*height + nOffSet;  //90是标题的高度
		if(it1/m_nRows < m_nCols)
		{
			rcPos.top += nOffSet * (it1 / m_nCols);
		}
		rcPos.bottom = rcPos.top + height;

		//设置圆角
		pControl->SetBorderSize(2);
		pControl->SetBorderColor(0xff0000ff);
		pControl->SetFloat(true);
		SIZE sz;
		sz.cx = 5;
		sz.cy = 5;
		pControl->SetBorderRound(sz);

		pControl->SetPos(rcPos);
	}

	CDvrBaseUI::SetPos(rc);
}

void CDvrBindItemUI::OnLBClick(const POINT& pt)
{
	CControlUI* pControl = HitPtInItem(pt);
	if( NULL == pControl)
	{
		return;
	}
	
	DBICtrl dbiCtrl= GetDBICtrl(m_selectRow, m_selectCol);
	RECT rcCtrl = pControl->GetPos();
	RECT rcBind = dbiCtrl.pBindInfo->GetPos();
	//选择框后面的一栏（包括label)
	rcBind.right = rcCtrl.right - 10;
	rcBind.left = rcCtrl.left + 10;
	if(PtInRect(&rcBind, pt))
	{
		/*bool bSelected = dbiCtrl.pBindInfo->IsSelected();
		if(!bSelected)
		{
		int index = m_selectRow * m_nCols + m_selectCol;
		WndBindInfo wbi = CONF_BIND.GetBindInfo(index);
		if(!wbi.strDevId.empty())
		{
		CString strT;
		strT.Format(_T("设备[%s]绑定在此窗口，将取消绑定!"), wbi.strDevId.c_str());
		if(SHAREDATA.g_pMainFrame->Alert(strT))
		{
		wbi.bBind = true;
		wbi.strDevId.clear();
		CONF_BIND.SetBindInfo(index, wbi);
		dbiCtrl.pBindInfo->Selected(true);
		}
		else
		{
		return;
		}
		}
		dbiCtrl.pBindInfo->Selected(true);
		}
		else
		{
		dbiCtrl.pBindInfo->Selected(false);
		}*/
	}
}


bool CDvrBindItemUI::LoadData()
{
	for ( int i = 0; i < m_nRows * m_nCols; ++i)
	{
		WndBindInfo wbi = CONF_BIND.GetBindInfo(i);
		DBICtrl &dibc = m_mapItem[i];

		dibc.pWndEnable->Selected(wbi.bEnable);
		dibc.pModifyInfo->Selected(wbi.bModified);
		dibc.pBindInfo->Selected(wbi.bBind);
	}

	return true;
}

bool CDvrBindItemUI::SaveData()
{
	for ( int i = 0; i < m_nRows * m_nCols; ++i)
	{
		WndBindInfo wbi = CONF_BIND.GetBindInfo(i);
		DBICtrl &dibc = m_mapItem[i];

		wbi.bEnable = dibc.pWndEnable->IsSelected();
		wbi.bModified = dibc.pModifyInfo->IsSelected();
		wbi.bBind = dibc.pBindInfo->IsSelected();

		CONF_BIND.SetBindInfo(i, wbi);
	}
	return true;
}
