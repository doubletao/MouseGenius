
// MouseGeniusDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MouseGenius.h"
#include "MouseGeniusDlg.h"
#include "afxdialogex.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const CString g_strLButtonDown = _T("左键按下");
static const CString g_strLButtonUp = _T("左键弹起");
static const CString g_strRButtonDown = _T("右键按下");
static const CString g_strRButtonUp = _T("右键弹起");
static const CString g_strSleep = _T("延时");

int g_nClock = 0;

HHOOK hhookMs = NULL;
LRESULT CALLBACK LowLevelMouseProc (INT nCode, WPARAM wParam, LPARAM lParam);
BOOL UninstallKbHook();
BOOL InstallKbHook();

LRESULT CALLBACK LowLevelMouseProc (INT nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT *pkbhs = (MSLLHOOKSTRUCT *)lParam;
	CString strMsg;

	CWnd * pWnd = AfxGetApp()->GetMainWnd();

	if (pWnd && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		CListCtrl * pList = (CListCtrl *)pWnd->GetDlgItem(IDC_LIST_ACTION_RECORD);
		if (pList && ::IsWindow(pList->GetSafeHwnd()))
		{
			switch (nCode)
			{
			case HC_ACTION:
				{
					if (wParam == WM_LBUTTONUP) 
					{
						int nLength = 0;
						strMsg.Format(_T("%s:%d"), g_strSleep, clock() - g_nClock);g_nClock = clock();
						pList->InsertItem(pList->GetItemCount(), strMsg);
						strMsg.Format(_T("%s:%d,%d"), g_strLButtonUp, pkbhs->pt.x, pkbhs->pt.y);
						pList->InsertItem(pList->GetItemCount(), strMsg);
						pList->EnsureVisible(pList->GetItemCount() - 1, FALSE);
					}

					if(wParam == WM_LBUTTONDOWN)
					{
						int nLength = 0;
						strMsg.Format(_T("%s:%d"), g_strSleep, clock() - g_nClock);g_nClock = clock();
						pList->InsertItem(pList->GetItemCount(), strMsg);
						strMsg.Format(_T("%s:%d,%d"), g_strLButtonDown, pkbhs->pt.x, pkbhs->pt.y);
						pList->InsertItem(pList->GetItemCount(), strMsg);
						pList->EnsureVisible(pList->GetItemCount() - 1, FALSE);
					}

					if (wParam == WM_RBUTTONUP) 
					{
						int nLength = 0;
						strMsg.Format(_T("%s:%d"), g_strSleep, clock() - g_nClock);g_nClock = clock();
						pList->InsertItem(pList->GetItemCount(), strMsg);
						strMsg.Format(_T("%s:%d,%d"), g_strRButtonUp, pkbhs->pt.x, pkbhs->pt.y);
						pList->InsertItem(pList->GetItemCount(), strMsg);
						pList->EnsureVisible(pList->GetItemCount() - 1, FALSE);
					}

					if(wParam == WM_RBUTTONDOWN)
					{
						int nLength = 0;
						strMsg.Format(_T("%s:%d"), g_strSleep, clock() - g_nClock);g_nClock = clock();
						pList->InsertItem(pList->GetItemCount(), strMsg);
						strMsg.Format(_T("%s:%d,%d"), g_strRButtonDown, pkbhs->pt.x, pkbhs->pt.y);
						pList->InsertItem(pList->GetItemCount(), strMsg);
						pList->EnsureVisible(pList->GetItemCount() - 1, FALSE);
					}
				}
			default:
				break;
			}
		}
	}
	return CallNextHookEx (NULL, nCode, wParam, lParam);
}

BOOL InstallKbHook( )
{

	if (hhookMs )
		UninstallKbHook();

	hhookMs = SetWindowsHookEx(WH_MOUSE_LL, 
		(HOOKPROC)LowLevelMouseProc, AfxGetApp()->m_hInstance, NULL);

	return(hhookMs != NULL);

}

BOOL UninstallKbHook()
{

	BOOL fOk = FALSE;
	if (hhookMs ) {
		fOk = UnhookWindowsHookEx(hhookMs );
		hhookMs = NULL;
	}

	return(fOk);
}

// CMouseGeniusDlg 对话框

CMouseGeniusDlg::CMouseGeniusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMouseGeniusDlg::IDD, pParent)
	, m_bRunning(FALSE)
	, m_pThreadRunning(NULL)
	, m_EventRunning(FALSE, TRUE, 0, 0)
	, m_bLoop(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	setlocale( LC_CTYPE, "chs" );//设定<ctpye.h>中字符处理方式
}

CMouseGeniusDlg::~CMouseGeniusDlg()
{
	UninstallKbHook();
}

void CMouseGeniusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMouseGeniusDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_RECORD, &CMouseGeniusDlg::OnBnClickedBtnRecord)
	ON_BN_CLICKED(IDC_BTN_RUN, &CMouseGeniusDlg::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CMouseGeniusDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMouseGeniusDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SETTING, &CMouseGeniusDlg::OnBnClickedBtnSetting)
	ON_MESSAGE(MSG_SET_RUNNING_BTN_STATE, OnSetRunningBtnState)
	ON_BN_CLICKED(IDC_CHK_LOOP, &CMouseGeniusDlg::OnBnClickedChkLoop)
END_MESSAGE_MAP()


// CMouseGeniusDlg 消息处理程序

BOOL CMouseGeniusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_LIST_ACTION_RECORD);
	if (pList && ::IsWindow(pList->GetSafeHwnd()))
	{
		CRect rtList;
		pList->GetWindowRect(rtList);
		pList->InsertColumn(0, _T("动作"), 0, rtList.Width() - 30);
		pList->SetExtendedStyle(pList->GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMouseGeniusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMouseGeniusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMouseGeniusDlg::OnBnClickedBtnRecord()
{
	if (m_bRunning)
	{
		AfxMessageBox(_T("运行中不能录制。"));
		return;
	}
	if (hhookMs == NULL)
	{
		InstallKbHook();
		CWnd * pButtonRecord = GetDlgItem(IDC_BTN_RECORD);
		if (pButtonRecord && ::IsWindow(pButtonRecord->GetSafeHwnd()))
		{
			pButtonRecord->SetWindowText(_T("停止录制"));
		}
		g_nClock = clock();
	}
	else
	{
		UninstallKbHook();
		CWnd * pButtonRecord = GetDlgItem(IDC_BTN_RECORD);
		if (pButtonRecord && ::IsWindow(pButtonRecord->GetSafeHwnd()))
		{
			pButtonRecord->SetWindowText(_T("开始录制"));
		}
		g_nClock = clock();
	}
}

void CMouseGeniusDlg::OnBnClickedBtnRun()
{
	if (hhookMs != NULL)
	{
		AfxMessageBox(_T("录制中不能运行。"));
		return;
	}
	if (m_bRunning)
	{
		StopRunning();
	}
	else
	{
		StartRunning();
	}
}

void CMouseGeniusDlg::OnBnClickedBtnSave()
{
	CFileDialog dialog(FALSE, _T(".mgs"), NULL, OFN_FILEMUSTEXIST|OFN_ENABLESIZING, _T("脚本文件(*.txt)|*.txt|"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		CString strName = dialog.GetPathName();
		CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_LIST_ACTION_RECORD);
		if (pList && ::IsWindow(pList->GetSafeHwnd()))
		{
			CStdioFile file;
			file.Open(strName, CFile::modeCreate | CFile::modeWrite);
			int nLineCount = pList->GetItemCount();
			for (int i = 0; i < nLineCount; i++)
			{
				CString strLine;
				strLine = pList->GetItemText(i, 0);
				file.WriteString(strLine);
				file.WriteString(_T("\n"));
			}
			file.Close();
		}
	}
}

const int nBufferSize = 1024 * 1024 * 10;

void CMouseGeniusDlg::OnBnClickedBtnOpen()
{
	CFileDialog dialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_ENABLESIZING, _T("脚本文件(*.txt)|*.txt|"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		CString strName = dialog.GetPathName();
		CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_LIST_ACTION_RECORD);
		if (pList && ::IsWindow(pList->GetSafeHwnd()))
		{
			pList->DeleteAllItems();
			CStdioFile file;
			file.Open(strName, CFile::modeRead);
			CString strLine;
			while(file.ReadString(strLine))
			{
				pList->InsertItem(pList->GetItemCount(), strLine);
			}
			file.Close();
		}
	}
}

void CMouseGeniusDlg::OnBnClickedBtnSetting()
{
	AfxMessageBox(_T("功能尚未实现。"));
}

UINT CMouseGeniusDlg::ThreadRunningFun( LPVOID pParam )
{
	CMouseGeniusDlg * pThis = (CMouseGeniusDlg *)pParam;
	if (pThis)
	{
		pThis->PostMessage(MSG_SET_RUNNING_BTN_STATE, 0, 0);
		do
		{
			if (WaitForSingleObject(pThis->m_EventRunning, 0) == WAIT_OBJECT_0)
			{
				break;
			}
			std::vector<ActionRecord> vecRecord = pThis->ActionInfoAnylise();
			for (UINT i = 0; i < vecRecord.size(); i++)
			{
				BOOL bStop = FALSE;
				switch (vecRecord[i].action)
				{
				case EmLButtonDown:
					SetCursorPos(vecRecord[i].nValue1, vecRecord[i].nValue2);
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
					break;
				case EmLButtonUp:
					SetCursorPos(vecRecord[i].nValue1, vecRecord[i].nValue2);
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					break;
				case EmRButtonDown:
					SetCursorPos(vecRecord[i].nValue1, vecRecord[i].nValue2);
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
					break;
				case EmRButtonUp:
					SetCursorPos(vecRecord[i].nValue1, vecRecord[i].nValue2);
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
					break;
				case EmSleep:
					if (WaitForSingleObject(pThis->m_EventRunning, vecRecord[i].nValue1) == WAIT_OBJECT_0)
					{
						bStop = TRUE;
					}
					break;
				default:
					break;
				}
				if (bStop)
				{
					break;
				}
			}
		} while (pThis->m_bLoop);
		pThis->PostMessage(MSG_SET_RUNNING_BTN_STATE, 1, 0);
	}
	return 0;
}

void CMouseGeniusDlg::StartRunning()
{
	StopRunning();
	m_EventRunning.ResetEvent();
	m_pThreadRunning = AfxBeginThread(ThreadRunningFun, this);
}

void CMouseGeniusDlg::StopRunning()
{
	m_EventRunning.SetEvent();
	if (m_pThreadRunning)
	{
		WaitForSingleObject(m_pThreadRunning->m_hThread, INFINITE);
	}
	m_pThreadRunning = NULL;
}

std::vector<ActionRecord> CMouseGeniusDlg::ActionInfoAnylise()
{
	std::vector<ActionRecord> vecRet;
	CListCtrl * pList = (CListCtrl *)GetDlgItem(IDC_LIST_ACTION_RECORD);
	if (pList && ::IsWindow(pList->GetSafeHwnd()))
	{
		int nLineCount = pList->GetItemCount();
		for (int i = 0; i < nLineCount; i++)
		{
			ActionRecord actionRecord;
			CString strLine = pList->GetItemText(i, 0);;
			int nColonPos = strLine.Find(_T(":"));
			if (nColonPos < 0)
			{
				//没找到冒号，不规范，不玩了
				continue;
			}
			int nCommaPos = strLine.Find(_T(","));
			CString strAction = strLine.Mid(0, nColonPos);
			if (strAction == g_strSleep)
			{
				actionRecord.action = EmSleep;
			}
			else if (strAction == g_strLButtonDown)
			{
				actionRecord.action = EmLButtonDown;
			}
			else if (strAction == g_strLButtonUp)
			{
				actionRecord.action = EmLButtonUp;
			}
			else if (strAction == g_strRButtonDown)
			{
				actionRecord.action = EmRButtonDown;
			}
			else if (strAction == g_strRButtonUp)
			{
				actionRecord.action = EmRButtonUp;
			}
			else
			{
				//动作写错了，不玩了
				continue;
			}
			if (actionRecord.action != EmSleep && nCommaPos <= nColonPos)
			{
				//除了sleep之外，逗号和冒号之间一定有东西，否则不规范
				continue;
			}
			if (actionRecord.action == EmSleep)
			{
				CString strSleepTime = strLine.Mid(nColonPos + 1);
				actionRecord.nValue1 = _ttoi(strSleepTime);
			}
			else
			{
				CString strXPos = strLine.Mid(nColonPos + 1, nCommaPos - nColonPos - 1);
				CString strYPos = strLine.Mid(nCommaPos + 1);
				actionRecord.nValue1 = _ttoi(strXPos);
				actionRecord.nValue2 = _ttoi(strYPos);
			}
			vecRet.push_back(actionRecord);
		}
	}
	return vecRet;
}

LRESULT CMouseGeniusDlg::OnSetRunningBtnState( WPARAM wParam, LPARAM lParam )
{
	if (wParam)
	{
		CWnd * pButtonRun = NULL;
		pButtonRun = GetDlgItem(IDC_BTN_RUN);
		if (pButtonRun && ::IsWindow(pButtonRun->GetSafeHwnd()))
		{
			pButtonRun->SetWindowText(_T("开始运行"));
		}
		m_bRunning = FALSE;
	}
	else
	{
		CWnd * pButtonRun = NULL;
		pButtonRun = GetDlgItem(IDC_BTN_RUN);
		if (pButtonRun && ::IsWindow(pButtonRun->GetSafeHwnd()))
		{
			pButtonRun->SetWindowText(_T("停止运行"));
		}
		m_bRunning = TRUE;
	}
	return 0;
}


void CMouseGeniusDlg::OnBnClickedChkLoop()
{
	CButton * pChk = NULL;
	pChk = (CButton *)GetDlgItem(IDC_CHK_LOOP);
	if (pChk && ::IsWindow(pChk->GetSafeHwnd()))
	{
		if (pChk->GetCheck())
		{
			m_bLoop = TRUE;
		}
		else
		{
			m_bLoop = FALSE;
		}
	}
}
