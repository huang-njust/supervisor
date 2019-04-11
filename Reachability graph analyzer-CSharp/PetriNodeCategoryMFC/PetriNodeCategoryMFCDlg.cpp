// PetriNodeCategoryMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PetriNodeCategoryMFC.h"
#include "PetriNodeCategoryMFCDlg.h"
#include "Graph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPetriNodeCategoryMFCDlg �Ի���




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
    DDX_Control(pDX, IDC_MLSTAR_NL, m_checkMLSTAR_NL);
    DDX_Control(pDX, IDC_MLSTARM_NL, m_checkMLSTARM_NL);
    DDX_Control(pDX, IDC_MFBMSTAR_NL, m_checkMFBMSTAR_NL);
    DDX_Control(pDX, IDC_MFBMSTARM_NL, m_checkMFBMSTARM_NL);
}

BEGIN_MESSAGE_MAP(CPetriNodeCategoryMFCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(ID_ANALYZE, &CPetriNodeCategoryMFCDlg::OnBnClickedAnalyze)
    ON_BN_CLICKED(ID_CLOSE, &CPetriNodeCategoryMFCDlg::OnBnClickedClose)
    ON_BN_CLICKED(IDC_OPENFILE, &CPetriNodeCategoryMFCDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_MLSTAR_NL, &CPetriNodeCategoryMFCDlg::OnBnClickedMlstarNl)
	ON_BN_CLICKED(IDC_MFBMSTAR_NL, &CPetriNodeCategoryMFCDlg::OnBnClickedMfbmstarNl)
	ON_BN_CLICKED(IDC_MLSTARM, &CPetriNodeCategoryMFCDlg::OnBnClickedMlstarm)
END_MESSAGE_MAP()


// CPetriNodeCategoryMFCDlg ��Ϣ�������

BOOL CPetriNodeCategoryMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    m_checkML.SetCheck(1);
	m_checkMFBM.SetCheck(1);

    m_checkMLSTARM.SetCheck(1);   
    m_checkMFBMSTARM.SetCheck(1);

	m_checkMLSTARM_NL.SetCheck(1);   
    m_checkMFBMSTARM_NL.SetCheck(1);

	m_checkMTSI.SetCheck(1);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPetriNodeCategoryMFCDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
        MessageBoxA("�����ļ�������Ϊ��", "����", MB_OK);
        return;
    }

    CGraph *G = new CGraph;

    //clock_t tBegin = clock();

	GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T("���ڶ�������..."));

    if (G->CreateGraphFromFile(csFilePath.GetBuffer()))//���ļ��ж��빹��ͼ������
    {
        GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T("���ڷ�������..."));

        //G->PrintEdgeNode();//��ӡ�ڵ�
        G->GetSConnection();//����ǿ��ͨ����

		csFilePath = csFilePath.GetBufferSetLength(csFilePath.GetLength()-4);
        if (1 == m_checkML.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_ML.txt", csFilePath.GetBuffer());
            G->SaveML(csTemp.GetBuffer());//���Ϸ�״̬���ϱ��浽ML.txt��
        }
        if (1 == m_checkMTSI.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_MTSI.txt", csFilePath.GetBuffer());
            G->SaveMTSI(csTemp.GetBuffer());//����MTSI
        }
        if (1 == m_checkMFBM.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_MFBM.txt", csFilePath.GetBuffer());
            G->SaveMFBM(csTemp.GetBuffer());//����MFBM
        }
        if (1 == m_checkMLSTAR.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_ML_Star.txt", csFilePath.GetBuffer());
            G->SaveMLStar(csTemp.GetBuffer());//����ML*
        }
        if (1 == m_checkMLSTARM.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_ML_Star_Matrix.txt", csFilePath.GetBuffer());
            G->SaveMLStar(csTemp.GetBuffer(), 1);//�����ʽ����ML*
        }
        if (1 == m_checkMFBMSTAR.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_MFBM_Star.txt", csFilePath.GetBuffer());
            G->SaveMFBMStar(csTemp.GetBuffer());//����MFBM*
        }
        if (1 == m_checkMFBMSTARM.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_MFBM_Star_Matrix.txt", csFilePath.GetBuffer());
            G->SaveMFBMStar(csTemp.GetBuffer(), 1);//�����ʽ����MFBM*
        }
    }

    delete G;

    //����NoLastPA��ML*��MFBM*
    CString csLastPA;
    GetDlgItem(IDC_EDIT_PATH)->GetWindowText(csFilePath);
    GetDlgItem(IDC_EDIT_LASTPA)->GetWindowText(csLastPA);
    CGraph *GNoLastPA = new CGraph;
    GNoLastPA->CalLastPA(csLastPA.GetBuffer());
    if (GNoLastPA->CreateGraphFromFile(csFilePath.GetBuffer()))//���ļ��ж��빹��ͼ������
    {
        //G->PrintEdgeNode();//��ӡ�ڵ�
        GNoLastPA->GetSConnection();//����ǿ��ͨ����

        csFilePath = csFilePath.GetBufferSetLength(csFilePath.GetLength()-4);
       
        if (1 == m_checkMLSTAR_NL.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_ML_C.txt", csFilePath.GetBuffer());
            GNoLastPA->SaveMLStar(csTemp.GetBuffer());//����ML*
        }
        if (1 == m_checkMLSTARM_NL.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_ML_C_Matrix.txt", csFilePath.GetBuffer());
            GNoLastPA->SaveMLStar(csTemp.GetBuffer(), 1);//�����ʽ����ML*
        }
        if (1 == m_checkMFBMSTAR_NL.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_MFBM_C.txt", csFilePath.GetBuffer());
            GNoLastPA->SaveMFBMStar(csTemp.GetBuffer());//����MFBM*
        }
        if (1 == m_checkMFBMSTARM_NL.GetCheck())
        {
            CString csTemp;
            csTemp.Format("%s_MFBM_C_Matrix.txt", csFilePath.GetBuffer());
            GNoLastPA->SaveMFBMStar(csTemp.GetBuffer(), 1);//�����ʽ����MFBM*
        }

        GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T(""));
        MessageBoxA("���ݷ������", "��ʾ", MB_OK);
    }
    //cout<<(clock()-tBegin)/1000.0<<endl;
    delete GNoLastPA;
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

        GetDlgItem(IDC_EDIT_PATH)->SetWindowText(csFilePath);//��ʾ�ļ�λ��
    }
}

void CPetriNodeCategoryMFCDlg::OnBnClickedMlstarNl()
{
	// TODO: Add your control notification handler code here
}

void CPetriNodeCategoryMFCDlg::OnBnClickedMfbmstarNl()
{
	// TODO: Add your control notification handler code here
}

void CPetriNodeCategoryMFCDlg::OnBnClickedMlstarm()
{
	// TODO: Add your control notification handler code here
}
