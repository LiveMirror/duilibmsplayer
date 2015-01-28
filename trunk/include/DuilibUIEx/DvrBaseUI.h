#ifndef _DVR_BASE_H_
#define _DVR_BASE_H_

using namespace DuiLib;

class CDvrBaseUI : public CTileLayoutUI
{
public:
	enum { SCROLL_TIMERID = 10 };

	CDvrBaseUI(int nRows, int nCols);
	~CDvrBaseUI(void);

	virtual void DoEvent(TEventUI& event);

	virtual void OnLBClick(const POINT& pt);
	virtual void OnRBClick(const POINT& pt);
	virtual CControlUI* HitPtInItem(const POINT& pt);

	//根据行列获取对应项
	virtual CContainerUI* GetItem(int row, int col);

	void GetSelectPos(int& row, int& col);
	void ClearSelectPos();

	void CDvrBaseUI::BKColor(int row, int column, DWORD boderColor, DWORD bkcolor);

protected:
	int m_nRows;
	int m_nCols;

	int m_selectRow;
	int m_selectCol;
};

#endif //_DVR_BASE_H_