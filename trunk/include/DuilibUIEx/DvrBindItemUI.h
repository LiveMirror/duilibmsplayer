//////////////////////////////////////////////////////////////////////////
//Describe:
//		绑定窗口
//Date: [2014/11/28 yjt]
//History:
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "DvrBaseUI.h"
#include <map>
using namespace std;


//每一个DVR项的控件指针
typedef struct tagDvrBindItemCtrl{
	COptionUI* pWndEnable;		//窗口是否启用
	COptionUI* pModifyInfo;		//修改信息
	COptionUI* pBindInfo;			//是否绑定
	COptionUI* pRegDvr;			//注册执法仪
}DBICtrl, *PDBICtrl;

//<窗口索引， 每个项的指针>
typedef map<int, DBICtrl> MapDBICtrl;
typedef MapDBICtrl::iterator MapDBICtrlItor;

class CDvrBindItemUI : public CDvrBaseUI
{
public:
	enum { SCROLL_TIMERID = 11 };

	CDvrBindItemUI(void);
	virtual ~CDvrBindItemUI(void);

	virtual void SetPos(RECT rc);
	virtual void OnLBClick(const POINT& pt);
private:
	DBICtrl InitDBICtrl(int row, int col);
	DBICtrl GetDBICtrl(int row, int col);

	bool LoadData();
	bool SaveData();

private:
	MapDBICtrl m_mapItem;

	COptionUI* m_pOptBind;
};