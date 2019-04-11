// PetriNodeCategoryMFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PetriNodeCategoryMFC.h"
#include "PetriNodeCategoryMFCDlg.h"
#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPetriNodeCategoryMFCDlg 对话框




CPetriNodeCategoryMFCDlg::CPetriNodeCategoryMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPetriNodeCategoryMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPetriNodeCategoryMFCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, ID_ANALYZE, m_btnAnalyze);
    DDX_Control(pDX, ID_CLOSE, m_btnClose);
    DDX_Control(pDX, IDC_ML, m_checkML);
    DDX_Control(pDX, IDC_MLSTAR, m_checkMLSTAR);
    DDX_Control(pDX, IDC_MLSTARM, m_checkMLSTARM);
    DDX_Control(pDX, IDC_MTSI, m_checkMTSI);
    DDX_Control(pDX, IDC_MFBM, m_checkMFBM);
    DDX_Control(pDX, IDC_MFBMSTAR, m_checkMFBMSTAR);
    DDX_Control(pDX, IDC_MFBMSTARM, m_checkMFBMSTARM);
}

BEGIN_MESSAGE_MAP(CPetriNodeCategoryMFCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(ID_ANALYZE, &CPetriNodeCategoryMFCDlg::OnBnClickedAnalyze)
    ON_BN_CLICKED(ID_CLOSE, &CPetriNodeCategoryMFCDlg::OnBnClickedClose)
    ON_BN_CLICKED(IDC_OPENFILE, &CPetriNodeCategoryMFCDlg::OnBnClickedOpenfile)
	ON_EN_CHANGE(IDC_EDIT_UNTRAN, &CPetriNodeCategoryMFCDlg::OnEnChangeEditUntran)
END_MESSAGE_MAP()


// CPetriNodeCategoryMFCDlg 消息处理程序

BOOL CPetriNodeCategoryMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    m_checkML.SetCheck(1);
    m_checkMLSTARM.SetCheck(1);
    m_checkMTSI.SetCheck(1);
    m_checkMFBM.SetCheck(1);
    m_checkMFBMSTARM.SetCheck(1);
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPetriNodeCategoryMFCDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPetriNodeCategoryMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPetriNodeCategoryMFCDlg::OnBnClickedAnalyze()
{
    CString csFilePath;

    GetDlgItem(IDC_EDIT_PATH)->GetWindowText(csFilePath);

    if (csFilePath.GetLength() == 0)
    {
        MessageBoxA("输入文件名不能为空", "错误", MB_OK);
        return;
    }

    CGraph *G = new CGraph;

    //clock_t tBegin = clock();

	GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T("正在读入数据..."));
	
	CString csUnconTran;
	GetDlgItem(IDC_EDIT_UNTRAN)->GetWindowText(csUnconTran);
	G->SetUnconTranSet(csUnconTran.GetBuffer());//初始化不可控变迁集合

    if (G->CreateGraphFromFile(csFilePath.GetBuffer()))//从文件中读入构建图的数据
    {
        GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T("正在分析数据..."));

        //G->PrintEdgeNode();//打印节点
        if (G->GetSConnection())//计算强连通分量
        {
			G->SaveAsTempFile();//保存此次输入的SCC，用作下次输入
			bool isContinue = G->GetConFlag();
			while (isContinue)
			{
				delete G;
				G = new CGraph;
				G->CreateGraphFromFile("tempinput.txt");
				G->GetSConnection();
				isContinue = G->GetConFlag();
				if (isContinue)
				{
					G->SaveAsTempFile();
				}
			}

			G->CreateGraphFromFile(csFilePath.GetBuffer());//重新使用原始输入文件构造可达图


			csFilePath = csFilePath.GetBufferSetLength(csFilePath.GetLength()-4);
			if (1 == m_checkML.GetCheck())
			{
				CString csTemp;
				csTemp.Format("%sML_uncontrollable.txt", csFilePath.GetBuffer());
				G->SaveML(csTemp.GetBuffer());//将合法状态集合保存到ML.txt中
			}
			if (1 == m_checkMTSI.GetCheck())
			{
				CString csTemp;
				csTemp.Format("%sMTSI_uncontrollable.txt", csFilePath.GetBuffer());
				G->SaveMTSI(csTemp.GetBuffer());//保存MTSI
			}
			if (1 == m_checkMFBM.GetCheck())
			{
				CString csTemp;
				csTemp.Format("%sMFBM_uncontrollable.txt", csFilePath.GetBuffer());
				G->SaveMFBM(csTemp.GetBuffer());//保存MFBM
			}
			if (1 == m_checkMLSTAR.GetCheck())
			{
				CString csTemp;
				csTemp.Format("%sMLStar_uncontrollable.txt", csFilePath.GetBuffer());
				G->SaveMLStar(csTemp.GetBuffer());//保存ML*
			}
			if (1 == m_checkMLSTARM.GetCheck())
			{
				CString csTemp;
				csTemp.Format("%sMLStarMatrix_uncontrollable.txt", csFilePath.GetBuffer());
				G->SaveMLStar(csTemp.GetBuffer(), 1);//矩阵格式保存ML*
			}
			if (1 == m_checkMFBMSTAR.GetCheck())
			{
				CString csTemp;
				csTemp.Format("%sMFBMStar_uncontrollable.txt", csFilePath.GetBuffer());
				G->SaveMFBMStar(csTemp.GetBuffer());//保存MFBM*
			}
			if (1 == m_checkMFBMSTARM.GetCheck())
			{
				CString csTemp;
				csTemp.Format("%sMFBMStarMatrix_uncontrollable.txt", csFilePath.GetBuffer());
				G->SaveMFBMStar(csTemp.GetBuffer(), 1);//矩阵格式保存MFBM*
			}

			GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T(""));
			MessageBoxA("数据分析完成", "提示", MB_OK);
        }
		else
		{
			GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T(""));
			MessageBoxA("M0节点被从SCC中删除！", "错误", MB_OK);
		}
		
    }
    //cout<<(clock()-tBegin)/1000.0<<endl;
    delete G;
}

void CPetriNodeCategoryMFCDlg::OnBnClickedClose()
{
    OnCancel();
}

void CPetriNodeCategoryMFCDlg::OnBnClickedOpenfile()
{
    CFileDialog c_OpenLocalPicDlg(TRUE);
    CString csFileName;
    CString csFilePath;
    c_OpenLocalPicDlg.m_ofn.lpstrFilter = _T("All Files(*.*)\0*.*\0\0");

    if (IDOK == c_OpenLocalPicDlg.DoModal())
    {
        csFileName = c_OpenLocalPicDlg.GetFileName();
        csFilePath = c_OpenLocalPicDlg.GetPathName();

        GetDlgItem(IDC_EDIT_PATH)->SetWindowText(csFilePath);//显示文件位置
    }
}

void CPetriNodeCategoryMFCDlg::OnEnChangeEditUntran()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
