
// MouseGeniusDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MouseGenius.h"
#include "MouseGeniusDlg.h"
#include "afxdialogex.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const CString g_strLButtonDown = _T("�������");
static const CString g_strLButtonUp = _T("�������");
static const CString g_strRButtonDown = _T("�Ҽ�����");
static const CString g_strRButtonUp = _T("�Ҽ�����");
static const CString g_strSleep = _T("��ʱ");

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
		CEdit * pEdit = (CEdit *)pWnd->GetDlgItem(IDC_EDIT_SCRIPT);
		if (pEdit && ::IsWindow(pEdit->GetSafeHwnd()))
		{
			switch (nCode)
			{
			case HC_ACTION:
				{
					if (wParam == WM_LBUTTONUP) 
					{
						int nLength = 0;
						strMsg.Format(_T("%s:%d\n"), g_strSleep, clock() - g_nClock);g_nClock = clock();
						nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);  
						pEdit->SetSel(nLength,  nLength);  
						pEdit->ReplaceSel(strMsg);
						strMsg.Format(_T("%s:%d,%d\n"), g_strLButtonUp, pkbhs->pt.x, pkbhs->pt.y);
						nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);  
						pEdit->SetSel(nLength,  nLength);  
						pEdit->ReplaceSel(strMsg);
					}

					if(wParam == WM_LBUTTONDOWN)
					{
						int nLength = 0;
						strMsg.Format(_T("%s:%d\n"), g_strSleep, clock() - g_nClock);g_nClock = clock();
						nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);  
						pEdit->SetSel(nLength,  nLength);  
						pEdit->ReplaceSel(strMsg);
						strMsg.Format(_T("%s:%d,%d\n"), g_strLButtonDown, pkbhs->pt.x, pkbhs->pt.y);
						nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);  
						pEdit->SetSel(nLength,  nLength);  
						pEdit->ReplaceSel(strMsg);
					}

					if (wParam == WM_RBUTTONUP) 
					{
						int nLength = 0;
						strMsg.Format(_T("%s:%d\n"), g_strSleep, clock() - g_nClock);g_nClock = clock();
						nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);  
						pEdit->SetSel(nLength,  nLength);  
						pEdit->ReplaceSel(strMsg);
						strMsg.Format(_T("%s:%d,%d\n"), g_strRButtonUp, pkbhs->pt.x, pkbhs->pt.y);
						nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);  
						pEdit->SetSel(nLength,  nLength);  
						pEdit->ReplaceSel(strMsg);
					}

					if(wParam == WM_RBUTTONDOWN)
					{
						int nLength = 0;
						strMsg.Format(_T("%s:%d\n"), g_strSleep, clock() - g_nClock);g_nClock = clock();
						nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);  
						pEdit->SetSel(nLength,  nLength);  
						pEdit->ReplaceSel(strMsg);
						strMsg.Format(_T("%s:%d,%d\n"), g_strRButtonDown, pkbhs->pt.x, pkbhs->pt.y);
						nLength = pEdit->SendMessage(WM_GETTEXTLENGTH);  
						pEdit->SetSel(nLength,  nLength);  
						pEdit->ReplaceSel(strMsg);
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

// CMouseGeniusDlg �Ի���

CMouseGeniusDlg::CMouseGeniusDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMouseGeniusDlg::IDD, pParent)
	, bRunning(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	setlocale( LC_CTYPE, "chs" );//�趨<ctpye.h>���ַ�����ʽ
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
END_MESSAGE_MAP()


// CMouseGeniusDlg ��Ϣ�������

BOOL CMouseGeniusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMouseGeniusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMouseGeniusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMouseGeniusDlg::OnBnClickedBtnRecord()
{
	if (bRunning)
	{
		AfxMessageBox(_T("�����в���¼�ơ�"));
		return;
	}
	if (hhookMs == NULL)
	{
		InstallKbHook();
		CWnd * pButtonRecord = GetDlgItem(IDC_BTN_RECORD);
		if (pButtonRecord && ::IsWindow(pButtonRecord->GetSafeHwnd()))
		{
			pButtonRecord->SetWindowText(_T("ֹͣ¼��"));
		}
	}
	else
	{
		UninstallKbHook();
		CWnd * pButtonRecord = GetDlgItem(IDC_BTN_RECORD);
		if (pButtonRecord && ::IsWindow(pButtonRecord->GetSafeHwnd()))
		{
			pButtonRecord->SetWindowText(_T("��ʼ¼��"));
		}
		g_nClock = clock();
	}
}

void CMouseGeniusDlg::OnBnClickedBtnRun()
{
	if (hhookMs != NULL)
	{
		AfxMessageBox(_T("¼���в������С�"));
		return;
	}
	if (bRunning)
	{
		bRunning = FALSE;
		CWnd * pButtonRun = GetDlgItem(IDC_BTN_RUN);
		if (pButtonRun && ::IsWindow(pButtonRun->GetSafeHwnd()))
		{
			pButtonRun->SetWindowText(_T("��ʼ����"));
		}
	}
	else
	{
		bRunning = TRUE;
		CWnd * pButtonRun = GetDlgItem(IDC_BTN_RUN);
		if (pButtonRun && ::IsWindow(pButtonRun->GetSafeHwnd()))
		{
			pButtonRun->SetWindowText(_T("ֹͣ����"));
		}
	}
}

void CMouseGeniusDlg::OnBnClickedBtnSave()
{
	CFileDialog dialog(FALSE, _T(".mgs"), NULL, OFN_FILEMUSTEXIST|OFN_ENABLESIZING, _T("�ű��ļ�(*.mgs)|*.mgs|"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		CString strName = dialog.GetPathName();
		CEdit * pEditScript = (CEdit *)GetDlgItem(IDC_EDIT_SCRIPT);
		if (pEditScript && ::IsWindow(pEditScript->GetSafeHwnd()))
		{
			CStdioFile file;
			file.Open(strName, CFile::modeCreate | CFile::modeWrite);
			int nLineCount = pEditScript->GetLineCount();
			for (int i = 0; i < nLineCount; i++)
			{
				TCHAR strLine[1024];
				memset(strLine, 0, 1024 * sizeof(TCHAR));
				int nSize = pEditScript->GetLine(i, strLine, 1024);
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
	CFileDialog dialog(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_ENABLESIZING, _T("�ű��ļ�(*.mgs)|*.mgs|"), NULL);
	if (dialog.DoModal() == IDOK)
	{
		CString strName = dialog.GetPathName();
		CEdit * pEditScript = (CEdit *)GetDlgItem(IDC_EDIT_SCRIPT);
		if (pEditScript && ::IsWindow(pEditScript->GetSafeHwnd()))
		{
			pEditScript->Clear();
			CStdioFile file;
			file.Open(strName, CFile::modeRead);
			CString strLine;
			while(file.ReadString(strLine))
			{
				int nLength = pEditScript->SendMessage(WM_GETTEXTLENGTH);  
				pEditScript->SetSel(nLength,  nLength);  
				pEditScript->ReplaceSel(strLine + _T("\n"));
			}
			file.Close();
		}
	}
}

void CMouseGeniusDlg::OnBnClickedBtnSetting()
{
	AfxMessageBox(_T("������δʵ�֡�"));
}
