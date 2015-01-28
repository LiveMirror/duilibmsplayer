#ifndef _DVR_INFO_UI_H_
#define _DVR_INFO_UI_H_

#include <string>
using namespace std;
using namespace DuiLib;

#include "../../DataBase/QD_Tables/TDvr.h"

class CDvrInfoUI: public CContainerUI
{
public:
	CDvrInfoUI(void);
	virtual ~CDvrInfoUI(void);

	bool Init(CPaintManagerUI& pm);

	void Notify(TNotifyUI& msg);
private:
	void OnClick(TNotifyUI& msg);
	void OnFocus(TNotifyUI& msg);
	void OnItemSelect(TNotifyUI &msg);

	void OnItemSelectMuf();

	void SetDvrInfo(const CTDvr &dvr);
	CTDvr GetDvrInfo();
	void ClearDvrInfo();
	void SetEnable(bool isEnable);

	void OnSave();
	void OnImport();
	void OnDel();	
	void OnAdd();
	void OnEdit();

	void OnRefresh();

	bool StrIsNum(const wstring& str);

	//初始化dvr的厂商和型号
	void InitDvrMM();
public:		 
	CButtonUI* m_btnSave;
	
	CButtonUI*	m_btnEdit;
	CButtonUI*	m_btnAdd;
	CButtonUI*	m_btnDel;
	CButtonUI*	m_btnImport;

	CButtonUI* m_btnRefresh;

	CComboBoxUI* m_pDept;

	CListUI*	m_pDvrList;

	COptionUI*	m_optEnable;
	COptionUI*	m_optDisable;

	CEditUI*	m_edtPid;
	CEditUI*	m_edtDevid;
	CComboBoxUI*	 m_cbMuf;
	CComboBoxUI*	 m_cbModel;
};


#endif // !_DVR_INFO_UI_H_
