#include "stdafx.h"
#include "PlayFrameWnd.h"
//#include "wmp.tlh"

CPlayFrameWnd::CPlayFrameWnd(const wstring& strUrl)
	: m_pPlayer(NULL)
	, m_url(strUrl)
{
}


CPlayFrameWnd::~CPlayFrameWnd(void)
{
}

LPCTSTR CPlayFrameWnd::GetWindowClassName() const 
{ 
	return _T("UIPlayFrame"); 
}

UINT CPlayFrameWnd::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
}

void CPlayFrameWnd::OnFinalMessage(HWND /*hWnd*/) 
{ 
	m_pm.RemovePreMessageFilter(this);
	delete this;
}

void CPlayFrameWnd::Init() 
{
	m_btnClose = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnClose")));
	m_lblMsg = static_cast<CLabelUI*>(m_pm.FindControl(_T("lblMsg")));
	m_wmp = static_cast<CActiveXUI*>(m_pm.FindControl(_T("wmplayer")));
	if( m_wmp ) 
	{
		m_wmp->GetControl(IID_IWMPPlayer4, (void**)&m_pPlayer); 

		m_pPlayer->put_URL((BSTR)m_url.c_str());
	}
}

void CPlayFrameWnd::Notify(TNotifyUI& msg)
{
	if( msg.sType == DUI_MSGTYPE_CLICK ) 
	{
		if( msg.pSender == m_btnClose ) 
		{
			m_pPlayer->close();
			Close();
			return;
		}
	}
}

LRESULT CPlayFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	//CDialogBuilderCallbackEx cb;
	CControlUI* pRoot = builder.Create(_T("playwindow.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CPlayFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( ::IsIconic(*this) ) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CPlayFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPlayFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CPlayFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcCaption.left && pt.x < rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CPlayFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		DuiLib::CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CPlayFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CPlayFrameWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	return S_OK;
}
