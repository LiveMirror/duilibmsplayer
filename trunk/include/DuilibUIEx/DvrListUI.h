#pragma once
//#include "../../DvrWndUI.h"

#include <map>
using namespace std;
#include "DvrBaseUI.h"

using namespace DuiLib;

//ÿһ��DVR��Ŀؼ�ָ��
typedef struct tagDvrItemCtrl{
	//��������Ԫ����
	CVerticalLayoutUI* pImg;
	CVerticalLayoutUI* pLabel;
	CVerticalLayoutUI* pValue;

	//���gif
	CContainerUI* pGif;

	//ͼƬ�еı�ǩ�����豸��
	//CLabelUI* pDev;
	CTextUI* pDev;

	//ID
	CLabelUI* pId;
	//Name
	CLabelUI* pName;
	//�̷�
	CLabelUI* pDisk;
	//������
	CProgressUI* pProgress;
	CLabelUI* pSpeed;
	//״̬
	CLabelUI* pStatus;
	CLabelUI* pFile;

	//�ļ���
	CLabelUI* pCount;
	//�ܴ�С
	CLabelUI* pSize;

}DvrItemCtrl, *PDvrItemCtrl;

//<���������� ÿ�����ָ��>
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

	//����Dvr��Ϣ����ʾ����
	//void SetInfo(CDvrWndUI* pDvr);

	void OnBtnClick(TNotifyUI& msg);
	void UnInit();

	//CDvrWndUI* GetCurDvrWndPtr();
private:
	//����DVR��λ�ã������ͻ���)
	void SetDvrPos(RECT rc);

	void HitItem(CControlUI* pItem);

	//��ȡָ�����еĿؼ�ָ�����
	DvrItemCtrl GetDvrItemCtrl(int row, int col);

	DvrItemCtrl InitDvrItemCtrl(int row, int col);

//	void SetInfo_Active(const DvrItemCtrl& dic, CDvrWndUI* dvr);
//	void SetInfo_Nomal(const DvrItemCtrl& dic, CDvrWndUI* dvr);

//	CDvrWndUI* GetDvrWndPtr(int index);

private:
	int m_enableClick;
	MapDvrCtrl m_mapItem;
};