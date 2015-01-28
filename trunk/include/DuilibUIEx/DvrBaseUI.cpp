#include "stdafx.h"
#include "DvrBaseUI.h"
#include "../../ShareData.h"

CDvrBaseUI::CDvrBaseUI(int nRows, int nCols)
	: m_nRows(nRows)
	, m_nCols(nCols)
{
	ClearSelectPos();
}

CDvrBaseUI::~CDvrBaseUI()
{

}

void CDvrBaseUI::DoEvent(TEventUI& event) 
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CTileLayoutUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_TIMER && event.wParam == SCROLL_TIMERID )
	{
		return;
	}
	if( event.Type == UIEVENT_BUTTONDOWN && IsEnabled() )
	{
		OnLBClick(event.ptMouse);
		return;
	}
	if ( event.Type == UIEVENT_RBUTTONDOWN && IsEnabled())
	{
		OnRBClick(event.ptMouse);
		return ;
	}
	if( event.Type == UIEVENT_BUTTONUP )
	{
		return;
	}
	if( event.Type == UIEVENT_SCROLLWHEEL )
	{
		return;
	}

	CTileLayoutUI::DoEvent(event);
}

void CDvrBaseUI::OnLBClick(const POINT& pt)
{
	CControlUI* pControl = HitPtInItem(pt);
	if( NULL == pControl)
	{
		return;
	}

}

void CDvrBaseUI::OnRBClick(const POINT& pt)
{
	CControlUI* pControl = HitPtInItem(pt);
	if( NULL == pControl)
	{
		return;
	}
}

CControlUI* CDvrBaseUI::HitPtInItem(const POINT& pt)
{
	CControlUI* pControl = NULL;
	for( int it1 = 0; it1 < m_items.GetSize(); it1++ ) 
	{
		pControl = static_cast<CControlUI*>(m_items[it1]);
		if( !pControl->IsVisible() ) 
			continue;
// 		if( pControl->IsFloat() ) {
// 			SetFloatPos(it1);
// 			continue;
// 		}

		RECT rcPos = pControl->GetPos();

		if( pt.x > rcPos.left && pt.x < rcPos.right && pt.y > rcPos.top && pt.y < rcPos.bottom )
		{
			//选择的行列
			m_selectRow = it1 / m_nCols;
			m_selectCol = it1 % m_nCols;
			return pControl;
		}
		else
		{
			m_selectRow = -1;
			m_selectCol = -1;
		}
	}
	pControl = NULL;

	return pControl;
}

//根据行列获取对应项
CContainerUI* CDvrBaseUI::GetItem(int row, int col)
{
	if(row < 0 || col < 0 || row >m_nRows || col > m_nCols)
	{
		return NULL;
	}

	int nIndex = (row  * m_nCols) +  col;
	CContainerUI* pControl = static_cast<CContainerUI*>(m_items[nIndex]);
	if( NULL == pControl && !pControl->IsVisible())
	{
		return NULL;
	}
	return pControl;
}

void CDvrBaseUI::GetSelectPos(int& row, int& col)
{
	row = m_selectRow;
	col = m_selectCol;
}

//设置背景色
void CDvrBaseUI::BKColor(int row, int column, DWORD boderColor, DWORD bkcolor)
{
	CContainerUI* pControl = GetItem(row, column);
	if(pControl == NULL)
	{
		return;
	}
	pControl->SetBorderColor(boderColor);
	pControl->SetBkColor(bkcolor);
}

void CDvrBaseUI::ClearSelectPos()
{
	m_selectRow = -1;
	m_selectCol = -1;
}