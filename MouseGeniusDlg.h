
// MouseGeniusDlg.h : ͷ�ļ�
//

#pragma once


// CMouseGeniusDlg �Ի���
class CMouseGeniusDlg : public CDialogEx
{
// ����
public:
	CMouseGeniusDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMouseGeniusDlg();

// �Ի�������
	enum { IDD = IDD_MOUSEGENIUS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
