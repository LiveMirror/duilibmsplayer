#pragma once
//#include "../../DvrWndUI.h"

#include <map>
using namespace std;
#include "DvrBaseUI.h"

using namespace DuiLib;

//每一个DVR项的控件指针
typedef struct tagDvrItemCtrl{
	//三个顶子元不素
	CVerticalLayoutUI* pImg;
	CVerticalLayoutUI* pLabel;
	CVerticalLayoutUI* pValue;

	//电池gif
	CContainerUI* pGif;

	//图片中的标签（无设备）
	//CLabelUI* pDev;
	CTextUI* pDev;

	//ID
	CLabelUI* pId;
	//Name
	CLabelUI* pName;
	//盘符
	CLabelUI* pDisk;
	//进度条
	CProgressUI* pProgress;
	CLabelUI* pSpeed;
	//状态
	CLabelUI* pStatus;
	CLabelUI* pFile;

	//文件数
	CLabelUI* pCount;
	//总大小
	CLabelUI* pSize;

}DvrItemCtrl, *PDvrItemCtrl;

//<窗口索引， 每个项的指针>
typedef map<int, DvrItemCtrl> MapDvrCtrl;
typedef MapDvrCtrl::iterator MapDvrCtrlItor;

class DvrListUI : public CDvrBaseUI//public CTileLayoutUI
{
public:
	enum { SCROLL_TIMERID = 10 };

	DvrListUI();
	virtual ~DvrListUI();

	virtual void SetPos(RECT rc);

	virtual void OnLBClick(const POINT& pt);
	virtual void OnRBClick(const POINT& pt);

	//设置Dvr信息，显示出来
	//void SetInfo(CDvrWndUI* pDvr);

	void OnBtnClick(TNotifyUI& msg);
	void UnInit();

	//CDvrWndUI* GetCurDvrWndPtr();
private:
	//调整DVR的位置（充满客户区)
	void SetDvrPos(RECT rc);

	void HitItem(CControlUI* pItem);

	//获取指定行列的控件指针对象
	DvrItemCtrl GetDvrItemCtrl(int row, int col);

	DvrItemCtrl InitDvrItemCtrl(int row, int col);

//	void SetInfo_Active(const DvrItemCtrl& dic, CDvrWndUI* dvr);
//	void SetInfo_Nomal(const DvrItemCtrl& dic, CDvrWndUI* dvr);

//	CDvrWndUI* GetDvrWndPtr(int index);

private:
	int m_enableClick;
	MapDvrCtrl m_mapItem;
};