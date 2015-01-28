#include "stdafx.h"
#include "ControlEx.h"

void SetVScrollBar(CScrollBarUI* pScroll)
{
	if(pScroll == NULL)
		return;

	pScroll->SetFixedWidth(40);
	pScroll->SetButton1NormalImage(_T("file='scrollbar.bmp' source='0,90,16,106' mask='#FFFF00FF'"));
	pScroll->SetButton1HotImage(_T("file='scrollbar.bmp' source='18,90,34,106' mask='#FFFF00FF'"));
	pScroll->SetButton1PushedImage(_T("file='scrollbar.bmp' source='36,90,52,106' mask='#FFFF00FF'"));
	pScroll->SetButton1DisabledImage(_T("file='scrollbar.bmp' source='54,90,70,106' mask='#FFFF00FF'"));
	pScroll->SetButton2NormalImage(_T("file='scrollbar.bmp' source='0,108,16,124' mask='#FFFF00FF'"));
	pScroll->SetButton2HotImage(_T("file='scrollbar.bmp' source='18,108,34,124' mask='#FFFF00FF'"));
	pScroll->SetButton2PushedImage(_T("file='scrollbar.bmp' source='36,108,52,124' mask='#FFFF00FF'"));
	pScroll->SetButton2DisabledImage(_T("file='scrollbar.bmp' source='54,108,70,124' mask='#FFFF00FF'"));
	pScroll->SetThumbNormalImage(_T("file='scrollbar.bmp' source='0,126,16,142' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetThumbHotImage(_T("file='scrollbar.bmp' source='18,126,34,142' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetThumbPushedImage(_T("file='scrollbar.bmp' source='36,126,52,142' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetThumbDisabledImage(_T("file='scrollbar.bmp' source='54,126,70,142' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetRailNormalImage(_T("file='scrollbar.bmp' source='0,144,16,160' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetRailHotImage(_T("file='scrollbar.bmp' source='18,144,34,160' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetRailPushedImage(_T("file='scrollbar.bmp' source='36,144,52,160' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetRailDisabledImage(_T("file='scrollbar.bmp' source='54,144,70,160' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetBkNormalImage(_T("file='scrollbar.bmp' source='0,162,16,178' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetBkHotImage(_T("file='scrollbar.bmp' source='18,162,34,178' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetBkPushedImage(_T("file='scrollbar.bmp' source='36,162,52,178' corner='2,2,2,2' mask='#FFFF00FF'"));
	pScroll->SetBkDisabledImage(_T("file='scrollbar.bmp' source='54,162,70,178' corner='2,2,2,2' mask='#FFFF00FF'"));
}