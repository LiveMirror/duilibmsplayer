#pragma once
//#include "include/DuilibUIEx/ControlEx.h"

#import "C:\\Windows\\System32\\wmp.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids 

using namespace DuiLib;

class CPlayFrameWnd: public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:
	CPlayFrameWnd(const wstring& strUrl);
	~CPlayFrameWnd(void);

	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/); 
	void Init();

	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

public:
	CPaintManagerUI m_pm;
	CButtonUI* m_btnClose;

	CLabelUI* m_lblMsg;

	CActiveXUI* m_wmp;

private:
	IWMPPlayer4* m_pPlayer;

	wstring m_url;
};

