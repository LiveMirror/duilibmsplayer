#ifndef _LOG_QUERY_UI_H_
#define _LOG_QUERY_UI_H_

using namespace DuiLib;
#include<list>
using namespace std;

//#include "../../DataBase/QD_Tables/TDvrLog.h"
//#include "../../DataBase/QD_Tables/TPlatformLog.h"
//#include "../../DataBase/QD_Tables/TWorkstationStatus.h"

//日志查询界面
class CLogQueryUI : public CContainerUI
{
public:
	CLogQueryUI();
	~CLogQueryUI();

	bool Init(CPaintManagerUI& m_pm);

	void Notify(TNotifyUI& msg);

	void RefreshPolices();

private:
	bool InitLogQueryPageData();
	bool ShowDvrLogData(const list<CTDvrLog>& dvrLog);
	bool ShowPlatformLogData(const list<CTPlatformLog>& pfLog);
	bool ShowWorkstationStatusData(const list<CTWorkstationStatus>& data);
	void OnBtnLogQuery();
	void OnItemSelectTypeLog();

	void OnItemSelect(TNotifyUI &msg);

private:
	CButtonUI* m_btnQueryLog;

	CListUI* m_pListDvrLog;
	CListUI* m_pListPlatformLog;
	CListUI* m_plstWSStatusLog;

	CComboUI* m_cbTypeLog;
	CComboUI* m_cbPoliceLog;
	CLabelUI* m_lblPoliceLog;

	CComboUI* m_cbYearSLog;
	CComboUI* m_cbMonSLog;
	CComboUI* m_cbDaySLog;
	CComboUI* m_cbYearELog;
	CComboUI* m_cbMonELog;
	CComboUI* m_cbDayELog;

	int m_nLogType;		//0:dvr 1:platform

	//显示消息
	CLabelUI* m_pLblMsg;
};


#endif