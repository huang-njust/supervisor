// PetriNodeCategoryMFCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CPetriNodeCategoryMFCDlg �Ի���
class CPetriNodeCategoryMFCDlg : public CDialog
{
// ����
public:
	CPetriNodeCategoryMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PETRINODECATEGORYMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    afx_msg void OnBnClickedAnalyze();
    afx_msg void OnBnClickedClose();
    afx_msg void OnBnClickedOpenfile();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CButton m_btnAnalyze;// ������ť
    CButton m_btnClose;// �رհ�ť
    CButton m_checkML;
    CButton m_checkMLSTAR;
    CButton m_checkMLSTARM;
    CButton m_checkMTSI;
    CButton m_checkMFBM;
    CButton m_checkMFBMSTAR;
    CButton m_checkMFBMSTARM;
	afx_msg void OnEnChangeEditUntran();
};
