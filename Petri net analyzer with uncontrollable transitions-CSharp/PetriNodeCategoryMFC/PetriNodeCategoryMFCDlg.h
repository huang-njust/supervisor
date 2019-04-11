// PetriNodeCategoryMFCDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CPetriNodeCategoryMFCDlg 对话框
class CPetriNodeCategoryMFCDlg : public CDialog
{
// 构造
public:
	CPetriNodeCategoryMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PETRINODECATEGORYMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    afx_msg void OnBnClickedAnalyze();
    afx_msg void OnBnClickedClose();
    afx_msg void OnBnClickedOpenfile();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CButton m_btnAnalyze;// 分析按钮
    CButton m_btnClose;// 关闭按钮
    CButton m_checkML;
    CButton m_checkMLSTAR;
    CButton m_checkMLSTARM;
    CButton m_checkMTSI;
    CButton m_checkMFBM;
    CButton m_checkMFBMSTAR;
    CButton m_checkMFBMSTARM;
	afx_msg void OnEnChangeEditUntran();
};
