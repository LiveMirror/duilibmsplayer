#ifndef __CONTROLEX_H__
#define __CONTROLEX_H__

#include <vector>
#include <math.h>

#include "DvrListUI.h"
#include "DvrBindItemUI.h"
#include "LogQueryUI.h"
#include "MediaQueryUI.h"
#include "DvrInfoUI.h"


class FtpConfigUI : public CContainerUI
{
public:
	FtpConfigUI()
	{
		CDialogBuilder builder;
		CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("FTPSetting.xml"), (UINT)0));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};
class WebConfigUI : public CContainerUI
{
public:
	WebConfigUI()
	{
		CDialogBuilder builder;
		CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("WEBSetting.xml"), (UINT)0));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};
class DataConfigUI : public CContainerUI
{
public:
	DataConfigUI()
	{
		CDialogBuilder builder;
		CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("DATASetting.xml"), (UINT)0));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};

class OtherConfigUI : public CContainerUI
{
public:
	OtherConfigUI()
	{
		CDialogBuilder builder;
		CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("OtherConfig.xml"), (UINT)0));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};

class CPoliceInfoUI: public CContainerUI
{
public:
	CPoliceInfoUI()
	{
		CDialogBuilder builder;
		CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("PoliceInfo.xml"), (UINT)0));
		if( pComputerExamine ) {
			this->Add(pComputerExamine);
		}
		else {
			this->RemoveAll();
			return;
		}
	}
};


class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
	CControlUI* CreateControl(LPCTSTR pstrClass) 
	{
		if( _tcscmp(pstrClass, _T("DvrList")) == 0 ) 
		{
			return new DvrListUI;
		}
		if( _tcscmp(pstrClass, _T("DvrBindItem")) == 0 ) 
		{
			return new CDvrBindItemUI;
		}
		else if( _tcscmp(pstrClass, _T("MediaQuery")) == 0)
		{
			return new CMediaQueryUI;
		}
		else if( _tcscmp(pstrClass, _T("LogQuery")) == 0)
		{
			return new CLogQueryUI;
		}
		else if( _tcscmp(pstrClass, _T("PoliceInfo")) == 0)
		{
			return new CPoliceInfoUI;
		}
		else if (_tcscmp(pstrClass, _T("FTPSetting")) == 0)
		{
			return new FtpConfigUI;
		}
		else if (_tcscmp(pstrClass, _T("WEBSetting")) == 0)
		{
			return new WebConfigUI;
		}
		else if (_tcscmp(pstrClass, _T("databasesetting")) == 0)
		{
			return new DataConfigUI;
		}
		else if (_tcscmp(pstrClass, _T("OtherConfig")) == 0)
		{
			return new OtherConfigUI;
		}
		else if( _tcscmp(pstrClass, _T("DvrInfo")) == 0)
		{
			return new CDvrInfoUI;
		}
		return NULL;
	}
};

void SetVScrollBar(CScrollBarUI* pScroll);

#endif __CONTROLEX_H__
