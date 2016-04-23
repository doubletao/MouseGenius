
// MouseGeniusDlg.h : 头文件
//

#pragma once
#include <vector>

const int MSG_SET_RUNNING_BTN_STATE = WM_USER + 1;

enum EmAction
{
	EmErrAction,
	EmLButtonDown,
	EmLButtonUp,
	EmRButtonDown,
	EmRButtonUp,
	EmSleep,
};

struct ActionRecord
{
	EmAction action;
	int nValue1;
	int nValue2;
	ActionRecord()
		: action(EmErrAction)
		, nValue1(0)
		, nValue2(0)
	{}
};

// CMouseGeniusDlg 对话框
class CMouseGeniusDlg : public CDialogEx
{
// 构造
public:
	CMouseGeniusDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMouseGeniusDlg();

// 对话框数据
	enum { IDD = IDD_MOUSEGENIUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	BOOL m_bRunning;
	CEvent m_EventRunning;
	CWinThread * m_pThreadRunning;
	static UINT ThreadRunningFun(LPVOID pParam);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void StartRunning();
	void StopRunning();
	std::vector<ActionRecord> ActionInfoAnylise();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRecord();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnSetting();
	afx_msg LRESULT OnSetRunningBtnState(WPARAM wParam, LPARAM lParam);
};
