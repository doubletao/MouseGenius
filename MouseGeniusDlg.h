
// MouseGeniusDlg.h : 头文件
//

#pragma once


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

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRecord();
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnSetting();
};
