#ifndef _MEDIA_QUERY_UI_H_
#define _MEDIA_QUERY_UI_H_

#include "../../DataBase/QD_Tables/TMediaInfo.h"
#include <string>
#include <list>
using namespace std;

using namespace DuiLib;

class CListUIEx;

//查询界面
class CMediaQueryUI : public CContainerUI
{
public:
	CMediaQueryUI();
	~CMediaQueryUI();

	bool Init(HWND hWnd, CPaintManagerUI& m_pm);

	void Notify(TNotifyUI& msg);

	void RefreshPolices();

private:
	void InitMediaQueryPage();
	bool InitMediaQueryPageData();
	list<CTMediaInfo> GetMediaInfo(const wstring& szValue);
	bool ShowMediaQueryData(list<CTMediaInfo>& listData);
	void OnBtnMediaQuery();

	CListContainerElementUI* AddLine();
	void OnPlay(CControlUI* pSender);
private:
	CButtonUI* m_btnQuery;
	CListUIEx* m_pList;
	CComboUI* m_Cclass;
	CComboUI* m_CUser;

	CComboUI* m_cbYearS;
	CComboUI* m_cbMonS;
	CComboUI* m_cbDayS;
	CComboUI* m_cbYearE;
	CComboUI* m_cbMonE;
	CComboUI* m_cbDayE;

	CComboUI* m_CType;

	//显示消息
	CLabelUI* m_pLblMsg;
private:
	HWND m_pHwnd;
};

#endif