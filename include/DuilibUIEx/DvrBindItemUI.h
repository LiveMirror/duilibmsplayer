//////////////////////////////////////////////////////////////////////////
//Describe:
//		�󶨴���
//Date: [2014/11/28 yjt]
//History:
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "DvrBaseUI.h"
#include <map>
using namespace std;


//ÿһ��DVR��Ŀؼ�ָ��
typedef struct tagDvrBindItemCtrl{
	COptionUI* pWndEnable;		//�����Ƿ�����
	COptionUI* pModifyInfo;		//�޸���Ϣ
	COptionUI* pBindInfo;			//�Ƿ��
	COptionUI* pRegDvr;			//ע��ִ����
}DBICtrl, *PDBICtrl;

//<���������� ÿ�����ָ��>
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