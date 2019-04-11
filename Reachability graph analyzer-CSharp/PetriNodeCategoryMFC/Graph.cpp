#include "stdafx.h"
#include "Graph.h"

// ////////////////////////////////////////////////////////////////////////////////
// CGraph
// ���캯��
//
CGraph::CGraph(void)
{
    m_iNodeNum = 0;

    for (int i=0; i!=MAX_N; ++i)
    {
        m_V[i] = NULL;
        m_DFN[i] = 0;
        m_LOW[i] = 0; 
        m_Instack[i] = 0;
        m_Stap[i] = 0;  
        m_Belong[i] = 0;
        m_Stop = 0;
        m_Bcnt = 0;
        m_Dindex = 0;

        for (int j=0; j!=MAX_P; ++j)
        {
            m_PA[i][j] = -1;
            m_PToken[i][j] = -1;
        }
    }

    for (int j=0; j!=MAX_P; ++j)
    {
        m_isLastPA[j] = false;
    }
}

// ////////////////////////////////////////////////////////////////////////////////
// ~CGraph
// ��������
//
CGraph::~CGraph(void)
{
    for (int i=0; i!=m_iNodeNum; ++i)
    {
        EdgeNode *pNode = m_V[i];
        EdgeNode *pTemp = NULL;

        while(NULL != pNode)
        {
            pTemp = pNode;
            pNode = pNode->next;

            delete pTemp;
            pTemp = NULL;
        }
    }
}

// ////////////////////////////////////////////////////////////////////////////////
// CreateGraphFromFile
// ���ļ��������ݣ�����ͼ���ڽӱ�
//
bool CGraph::CreateGraphFromFile(char *filename)
{
    ifstream inputFile;//�����ļ���
    char strInputBuf[1024] = {'\0'};
    int idx = 0;
    char strTran[T_LEN] = {0};
    bool isPreLineToken = false;//ǰһ�������Ƿ���token����

    locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���
    inputFile.open(filename, ios::in);//���ļ�
    locale::global(loc);//�ָ�ȫ��locale,locΪ�����locale����

    if (!inputFile)
    {
        MessageBoxA(NULL, "�����ļ���ʧ�ܣ�", "����", MB_OK);
        return false;
    }

    while (inputFile.getline(strInputBuf,1024))//���ж�������
    {
        //��toks��ǻ�ǰһ������ʱtoken���ݣ���ǰ����:����������ǰ����token����
        if ((NULL!=strstr(strInputBuf,"toks")) || (isPreLineToken && NULL!=strstr(strInputBuf,":")))
        {
            isPreLineToken = true;
            InitTokenArg(idx, strInputBuf);
        }
        else
        {
            //cout<<strInputBuf<<endl;
            isPreLineToken = false;
            int iTemp = SplitStr(strInputBuf, strTran);

            if (-1 != iTemp)//���ݷ���ֵ�ж��Ƿ�������
            {
                if (0 == strcmp(strTran,""))
                {
                    idx = iTemp;
                }
                else
                {
                    InsertEdge(idx, iTemp, strTran);
                }

                memset(strTran, 0, T_LEN);
            }
        }
    }

    inputFile.close();//�ر��ļ���

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////
// SplitStr
// ��������ַ����е���ţ������������
// strTran�����Ǩ�ţ����Ϊ�գ���˵�����ص���״̬��ţ����򷵻ص��Ǿ���strTranת���������
//
int CGraph::SplitStr(char *str, char *strTran)
{
    bool flag = false;
    char *strTemp = NULL;//����״̬���
    int idx = 0;
    int iLen = strlen(str);
    int i;
    int iStageNum = -1;

    if (NULL != strstr(str, "State nr."))//��ǰ״̬�����
    {
        strTemp = new char[10];
        assert(strTemp != NULL);
        memset(strTemp, 0, 10);

        for (i=0; i!=iLen; ++i)
        {
            if (str[i] == ' ')
            {
                flag = true;
            }
            else if (flag)
            {
                strTemp[idx] = str[i];
                ++idx;
            }
        }

        iLen = strlen(strTemp);
        for (i=0; i<iLen-3; ++i)
        {
            strTemp[i] = strTemp[i+3];
        }
        for (; i<iLen; ++i)
        {
            strTemp[i] = '\0';
        }

        iStageNum = atoi(strTemp)-1;//�������
        ++m_iNodeNum;//״̬������1

        delete strTemp;
        strTemp = NULL;
    }
    else if (NULL != strstr(str, "=="))//ת������״̬���
    {
        strTemp = new char[10];
        assert(strTemp != NULL);
        memset(strTemp, 0, 10);

        for (i=0; i!=iLen; ++i)
        {
            if (str[i] == ' ')
            {
                flag = true;
            }
            else if (flag)
            {
                strTemp[idx] = str[i];
                ++idx;
            }
        }

        iLen = strlen(strTemp);
        for (i=0; i<iLen-1; ++i)//��ñ�Ǩ
        {
            strTemp[i] = strTemp[i+1];
        }
        for (; i<iLen; ++i)
        {
            strTemp[i] = '\0';
        }

        iStageNum = atoi(strTemp)-1;//�������

        i = 2;
        while('=' != str[i])
        {
            strTran[i-2] = str[i++];
        }

        delete strTemp;
        strTemp = NULL;
    }

    return iStageNum;
}

// ////////////////////////////////////////////////////////////////////////////////
// InitTokenArg
// �������ַ�����ȡtokenֵ
//
void CGraph::InitTokenArg(int index, char *str)
{
    int idx = 0;
    int iTemp = 0;
    int iLen = strlen(str);
    bool flag = false;
    char strTemp[S_LEN] = {0};//��ʱ�ַ���

    while(m_PToken[index][idx] != -1)
    {
        idx++;
    }

    for (int i=0; i!=iLen; ++i)
    {
        if (' ' == str[i] && flag && 0!=strcmp(strTemp,"") && 0!=strcmp(strTemp,":"))
        {
            m_PToken[index][idx] = atoi(strTemp);
            idx++;
            memset(strTemp, 0, S_LEN);
            iTemp = 0;
        }
        else if (' ' != str[i] && flag)
        {
            strTemp[iTemp] = str[i];
            iTemp++;
        }
        else if (' ' == str[i] && !flag)//��flagΪtrue����ʾ�Ժ������ո�������Ч������strTemp��
        {
            flag = true;
        }
        if (0 == strcmp(strTemp,":"))//���⴦�����tokenֵ
        {
            memset(strTemp, 0, S_LEN);
            iTemp = 0;
        }
    }

     m_PToken[index][idx] = atoi(strTemp);//�������һ��tokenֵ

     InitPAFromToken(index);//��ʼ��PA����
}

// ////////////////////////////////////////////////////////////////////////////////
// InitPAFromToken
// ����tokenֵ��ʼ��PA����
//
void CGraph::InitPAFromToken(int index) 
{
    int idx = 0;

    for (int i=0; m_PToken[0][i]!=-1; ++i)
    {
        if ((0==m_PToken[0][i])&&(!m_isLastPA[i]))//Stage 1��token��Ϊ0��ΪPA
        {
            m_PA[index][idx] = m_PToken[index][i];
            idx++;
        }
    }
}

// ////////////////////////////////////////////////////////////////////////////////
// InsertEdgeNode
// �����ڽӱ�ڵ�
//
void CGraph::InsertEdge(int idx, int data, char *strTran)
{
    EdgeNode *pNode = m_V[idx];
    EdgeNode *pEdge = new EdgeNode;

    pEdge->k = data;
    strcpy_s(pEdge->tran, T_LEN, strTran);

    if (NULL == pNode)//��������ڵ�
    {	
        m_V[idx] = pEdge;
        return;
    }

    while(NULL != pNode->next)//������������ڵ�
    {
        pNode = pNode->next;
    }

    pNode->next = pEdge;
}

// ////////////////////////////////////////////////////////////////////////////////
// PrintEdgeNode
// ��ӡ�ڽӱ�ڵ�
//
void CGraph::PrintEdgeNode()
{
    for (int i=0; i!=m_iNodeNum; ++i)
    {
        EdgeNode *pNode = m_V[i];

        cout<<i+1<<"\t->";

        while(NULL != pNode)
        {
            cout<< pNode->k+1<<"->";
            pNode = pNode->next;
        }
        cout<<endl;
    }
}

// ////////////////////////////////////////////////////////////////////////////////
// Tarjan
// Tarjan�㷨���ͼ��ǿ��ͨ����
//
void CGraph::Tarjan(int i)
{
    int j;
    m_DFN[i]=m_LOW[i]=++m_Dindex;
    m_Instack[i]=true;
    m_Stap[++m_Stop]=i;
    for (EdgeNode *e=m_V[i];e;e=e->next)
    {
        j=e->k;
        if (!m_DFN[j])
        {
            Tarjan(j);
            if (m_LOW[j]<m_LOW[i])
                m_LOW[i]=m_LOW[j];
        }
        else if (m_Instack[j] && m_DFN[j]<m_LOW[i])
            m_LOW[i]=m_DFN[j];
    }
    if (m_DFN[i]==m_LOW[i])
    {
        m_Bcnt++;
        do
        {
            j=m_Stap[m_Stop--];
            m_Instack[j]=false;
            m_Belong[j]=m_Bcnt;
        }
        while (j!=i);
    }
}

// ////////////////////////////////////////////////////////////////////////////////
// GetConnection
// �������ж��㣬����ǿ��ͨ����
//
void CGraph::GetSConnection()
{
    for (int i=0; i<m_iNodeNum; i++)
    {
        if (!m_DFN[i])
        {
            Tarjan(i);
        }
    }
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveML
// ��úϷ�״̬����ML�����浽ML.txt�ļ���
//
void CGraph::SaveML(char *filename)
{
    ofstream outFile;//�����ļ���

    locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���
    outFile.open(filename, ios::out);//���ļ�
    locale::global(loc);//�ָ�ȫ��locale,locΪ�����locale����

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (m_Belong[i] == m_Belong[0])
        {
            SaveStage(outFile, i);
        }
    }

    outFile.close();//�ر��ļ���
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveMTSI
// ���״̬��Ǩ��������MTSI���ϣ����浽filename�ļ���
//
void CGraph::SaveMTSI(char *filename)
{
    ofstream outFile;//�����ļ���

    locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���
    outFile.open(filename, ios::out);//���ļ�
    locale::global(loc);//�ָ�ȫ��locale,locΪ�����locale����

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (m_Belong[i] == m_Belong[0])
        {
            EdgeNode *pNode = m_V[i];

            while(NULL != pNode)
            {
                //��ǰ�ڵ��ǺϷ��ڵ㣬����֮���бߵĽڵ��ǻ��ڵ�
                if (m_Belong[pNode->k]!=m_Belong[0])
                {
                    SaveStage(outFile, i);
                    outFile<<"=="<<pNode->tran<<"=> s"<<pNode->k+1<<endl;
                }

                pNode = pNode->next;
            }
        }
    }

    outFile.close();//�ر��ļ���
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveMFBM
// ���MFBM���ϣ����浽filename�ļ���
//
void CGraph::SaveMFBM(char *filename)
{
    bool *isFBMInclude = new bool[m_iNodeNum];
    ofstream outFile;//�����ļ���

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        isFBMInclude[i] = false;
    }

    locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���
    outFile.open(filename, ios::out);//���ļ�
    locale::global(loc);//�ָ�ȫ��locale,locΪ�����locale����

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (m_Belong[i] == m_Belong[0])
        {
            EdgeNode *pNode = m_V[i];

            while(NULL != pNode)
            {
                //��ǰ�ڵ��ǺϷ��ڵ㣬����֮���бߵĽڵ��ǻ��ڵ�
                if (m_Belong[pNode->k]!=m_Belong[0])
                {
                   if (!isFBMInclude[pNode->k])//��״̬û�б������
                   {
                       isFBMInclude[pNode->k] = true;
                   }
                }

                pNode = pNode->next;
            }
        }
    }

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (isFBMInclude[i])//���Ϊtrue�Ķ���FBM
        {
            SaveStage(outFile, i);
        }
    }

    outFile.close();//�ر��ļ���

    delete [] isFBMInclude;
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveMLStar
// ���ML*��typeΪ1�Ծ��󱣴浽filename�ļ���
//
void CGraph::SaveMLStar(char *filename, int type)
{
    bool *isMLStart = new bool[m_iNodeNum];
    ofstream outFile;//�����ļ���

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        isMLStart[i] = true;
    }

    for (int i=0; i!=m_iNodeNum; ++i)//��DZ�е�״̬���Ϊfalse
    {
        if (m_Belong[i] != m_Belong[0])
        {
            isMLStart[i] = false;
        }
    }

    locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���
    outFile.open(filename, ios::out);//���ļ�
    locale::global(loc);//�ָ�ȫ��locale,locΪ�����locale����

    for (int i=0; i!=m_iNodeNum; ++i)
    {   
        if (isMLStart[i])//m_PA[i]û�б�A-Cover
        {
            for (int j=0; j!=m_iNodeNum; ++j)
            {
                if (isMLStart[j] && i!=j)//m_PA[j]û�б�A-Cover
                {
                    if (isAargGTBarg(i,j))
                    {
                        isMLStart[j] = false;//m_PA[j]��m_PA[i] Cover
                    } 
                }
            }
        }
    }

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (isMLStart[i])//���Ϊtrue�Ķ���ML*Ԫ��
        {
            if (type == 0)
            {
                SaveStage(outFile, i);
            }
            else if (type == 1)
            {
                SaveAsMatrix(outFile, i);
            } 
        }
    }

    outFile.close();//�ر��ļ���

    delete [] isMLStart;
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveMFBMStar
// ���MFBM*��typeΪ1�Ծ��󱣴浽filename�ļ���
//
void CGraph::SaveMFBMStar(char *filename, int type)
{
    bool *isMFBMStart = new bool[m_iNodeNum];
    bool *isFBMInclude = new bool[m_iNodeNum];
    ofstream outFile;//�����ļ���

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        isFBMInclude[i] = false;
    }

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        isMFBMStart[i] = false;
    }

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (m_Belong[i] == m_Belong[0])
        {
            EdgeNode *pNode = m_V[i];

            while(NULL != pNode)
            {
                //��ǰ�ڵ��ǺϷ��ڵ㣬����֮���бߵĽڵ��ǻ��ڵ�
                if (m_Belong[pNode->k]!=m_Belong[0])
                {
                    if (!isFBMInclude[pNode->k])//��״̬û�б������
                    {
                        isFBMInclude[pNode->k] = true;
                    }
                }

                pNode = pNode->next;
            }
        }
    }

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (isFBMInclude[i])//���Ϊtrue�Ķ���FBM
        {
            isMFBMStart[i] = true;
        }
    }

    locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���
    outFile.open(filename, ios::out);//���ļ�
    locale::global(loc);//�ָ�ȫ��locale,locΪ�����locale����

    for (int i=0; i!=m_iNodeNum; ++i)
    {   
        if (isMFBMStart[i])//m_PA[i]û�б�A-Cover
        {
            for (int j=0; j!=m_iNodeNum; ++j)
            {
                if (isMFBMStart[j] && i!=j)//m_PA[j]û�б�A-Cover
                {
                    if (isAargGTBarg(j,i))
                    {
                        isMFBMStart[j] = false;//m_PA[j]��m_PA[i] Cover
                    } 
                }
            }
        }
    }

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (isMFBMStart[i])//���Ϊtrue�Ķ���ML*Ԫ��
        {
            if (type == 0)
            {
                SaveStage(outFile, i);
            }
            else if (type == 1)
            {
                SaveAsMatrix(outFile, i);
            }  
        }
    }

    outFile.close();//�ر��ļ���

    delete [] isMFBMStart;
    delete [] isFBMInclude;
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveStage
// ���Stage nr.����P.nr��toks���ļ�
//
void CGraph::SaveStage(ofstream &outFile, int i)
{
    outFile<<"State nr."<<setw(8)<<i+1<<endl;//��������

    outFile<<"P.nr:";//���������
    for (int j=0; m_PToken[i][j]!=-1; ++j)
    {
        outFile<<setw(4)<<j+1;
    }

    outFile<<endl<<"toks:";//���token��
    for (int j=0; m_PToken[i][j]!=-1; ++j)
    {
        outFile<<setw(4)<<m_PToken[i][j];
    }
    outFile<<endl;
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveAsMatrix
// �Ծ����ʽ���PA
//
void CGraph::SaveAsMatrix(ofstream &outFile, int n)
{
    for (int i=0; m_PA[n][i]!=-1; ++i)
    {
        outFile<<setw(4)<<m_PA[n][i];
    }
    outFile<<endl;
}

// ////////////////////////////////////////////////////////////////////////////////
// isAargGTBarg
// �Ƿ�A������ÿһ��Ԫ�ش��ڵ���B�ж�Ӧλ�õ�Ԫ��
//
bool CGraph::isAargGTBarg(int idxA, int idxB)
{
    for (int i=0; m_PA[idxA][i]!=-1; ++i)
    {
        if (m_PA[idxA][i] < m_PA[idxB][i])
        {
            return false;
        }
    }

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////
// CalLastPA
// ����LastPA�������б�ʶ
//
void CGraph::CalLastPA(char *filename)
{
    /*int iTokenOfStage1[MAX_P];
    for (int i=0; i!=MAX_P; ++i)
    {
        iTokenOfStage1[i] = -1;
    }

    ifstream inputFile;//�����ļ���
    char strInputBuf[1024] = {'\0'};
    int idx = 0;
    bool isPreLineToken = false;//ǰһ�������Ƿ���token����
    char strTemp[S_LEN];

    locale loc = locale::global(locale("")); //Ҫ�򿪵��ļ�·�������ģ�����ȫ��localeΪ���ػ���
    inputFile.open(filename, ios::in);//���ļ�
    locale::global(loc);//�ָ�ȫ��locale,locΪ�����locale����

    if (!inputFile)
    {
        MessageBoxA(NULL, "�����ļ���ʧ�ܣ�", "����", MB_OK);
        return;
    }

    while (inputFile.getline(strInputBuf,1024))//���ж�������
    {
        //��toks��ǻ�ǰһ������ʱtoken���ݣ���ǰ����:����������ǰ����token����
        if ((NULL!=strstr(strInputBuf,"toks")) || (isPreLineToken && NULL!=strstr(strInputBuf,":")))
        {
            isPreLineToken = true;

            int iLen = strlen(strInputBuf);
            bool flag = false;
            int iTemp = 0;
            memset(strTemp, 0, S_LEN);//��ʱ�ַ���

            for (int i=0; i!=iLen; ++i)
            {
                if (' ' == strInputBuf[i] && flag && 0!=strcmp(strTemp,"") && 0!=strcmp(strTemp,":"))
                {
                    iTokenOfStage1[idx] = atoi(strTemp);
                    idx++;
                    memset(strTemp, 0, S_LEN);
                    iTemp = 0;
                }
                else if (' ' != strInputBuf[i] && flag)
                {
                    strTemp[iTemp] = strInputBuf[i];
                    iTemp++;
                }
                else if (' ' == strInputBuf[i] && !flag)//��flagΪtrue����ʾ�Ժ������ո�������Ч������strTemp��
                {
                    flag = true;
                }
                if (0 == strcmp(strTemp,":"))//���⴦�����tokenֵ
                {
                    memset(strTemp, 0, S_LEN);
                    iTemp = 0;
                }
            }
            
            iTokenOfStage1[idx++] = atoi(strTemp);//�������һ��tokenֵ
        }
        else if (isPreLineToken)//��ǰ��û��token���ݣ�����ǰһ����token���ݣ����Ѷ�ȡstatage 1������token����
        {
            break;//�˳�whileѭ��
        }
    }

    inputFile.close();//�ر��ļ���

    for (int i=0; i!=MAX_P-1; ++i)
    {
        if((-1!=iTokenOfStage1[i])&&(0==iTokenOfStage1[i])&&(0!=iTokenOfStage1[i+1]))
        {
            m_isLastPA[i] = true;
        }
    }*/
    int iLen = strlen(filename);
    int idx = 0;
    char strTemp[S_LEN];
    memset(strTemp, 0, S_LEN);

    for (int i=0; i!=iLen; ++i)
    {
        if (' ' == filename[i])
        {
            m_isLastPA[atoi(strTemp)-1] = true;
            memset(strTemp, 0, S_LEN);
            idx = 0;
        }
        else
        {
            strTemp[idx++] = filename[i];
        }
    }

    m_isLastPA[atoi(strTemp)-1] = true;
}