#include "stdafx.h"
#include "Graph.h"

// ////////////////////////////////////////////////////////////////////////////////
// CGraph
// 构造函数
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
// 析构函数
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
// 从文件读入数据，生成图的邻接表
//
bool CGraph::CreateGraphFromFile(char *filename)
{
    ifstream inputFile;//输入文件流
    char strInputBuf[1024] = {'\0'};
    int idx = 0;
    char strTran[T_LEN] = {0};
    bool isPreLineToken = false;//前一行数据是否是token数据

    locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
    inputFile.open(filename, ios::in);//打开文件
    locale::global(loc);//恢复全局locale,loc为定义的locale变量

    if (!inputFile)
    {
        MessageBoxA(NULL, "输入文件打开失败！", "错误", MB_OK);
        return false;
    }

    while (inputFile.getline(strInputBuf,1024))//按行读入内容
    {
        //有toks标记或前一行数据时token数据，当前行有:，都表明当前行是token数据
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

            if (-1 != iTemp)//根据返回值判断是否存在序号
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

    inputFile.close();//关闭文件流

    return true;
}

// ////////////////////////////////////////////////////////////////////////////////
// SplitStr
// 获得输入字符串中的序号，返回整型序号
// strTran保存变迁号，如果为空，则说明返回的是状态序号，否则返回的是经过strTran转换到的序号
//
int CGraph::SplitStr(char *str, char *strTran)
{
    bool flag = false;
    char *strTemp = NULL;//保存状态编号
    int idx = 0;
    int iLen = strlen(str);
    int i;
    int iStageNum = -1;

    if (NULL != strstr(str, "State nr."))//当前状态的序号
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

        iStageNum = atoi(strTemp)-1;//返回序号
        ++m_iNodeNum;//状态个数加1

        delete strTemp;
        strTemp = NULL;
    }
    else if (NULL != strstr(str, "=="))//转换到的状态序号
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
        for (i=0; i<iLen-1; ++i)//获得变迁
        {
            strTemp[i] = strTemp[i+1];
        }
        for (; i<iLen; ++i)
        {
            strTemp[i] = '\0';
        }

        iStageNum = atoi(strTemp)-1;//返回序号

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
// 从输入字符串提取token值
//
void CGraph::InitTokenArg(int index, char *str)
{
    int idx = 0;
    int iTemp = 0;
    int iLen = strlen(str);
    bool flag = false;
    char strTemp[S_LEN] = {0};//临时字符串

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
        else if (' ' == str[i] && !flag)//置flag为true，表示以后遇到空格则有有效数据在strTemp中
        {
            flag = true;
        }
        if (0 == strcmp(strTemp,":"))//特殊处理多行token值
        {
            memset(strTemp, 0, S_LEN);
            iTemp = 0;
        }
    }

     m_PToken[index][idx] = atoi(strTemp);//保存最后一个token值

     InitPAFromToken(index);//初始化PA数组
}

// ////////////////////////////////////////////////////////////////////////////////
// InitPAFromToken
// 根据token值初始化PA数组
//
void CGraph::InitPAFromToken(int index) 
{
    int idx = 0;

    for (int i=0; m_PToken[0][i]!=-1; ++i)
    {
        if ((0==m_PToken[0][i])&&(!m_isLastPA[i]))//Stage 1中token数为0即为PA
        {
            m_PA[index][idx] = m_PToken[index][i];
            idx++;
        }
    }
}

// ////////////////////////////////////////////////////////////////////////////////
// InsertEdgeNode
// 插入邻接表节点
//
void CGraph::InsertEdge(int idx, int data, char *strTran)
{
    EdgeNode *pNode = m_V[idx];
    EdgeNode *pEdge = new EdgeNode;

    pEdge->k = data;
    strcpy_s(pEdge->tran, T_LEN, strTran);

    if (NULL == pNode)//插入链表节点
    {	
        m_V[idx] = pEdge;
        return;
    }

    while(NULL != pNode->next)//在链表最后插入节点
    {
        pNode = pNode->next;
    }

    pNode->next = pEdge;
}

// ////////////////////////////////////////////////////////////////////////////////
// PrintEdgeNode
// 打印邻接表节点
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
// Tarjan算法获得图的强连通分量
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
// 遍历所有顶点，计算强连通分量
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
// 获得合法状态集合ML，保存到ML.txt文件中
//
void CGraph::SaveML(char *filename)
{
    ofstream outFile;//输入文件流

    locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
    outFile.open(filename, ios::out);//打开文件
    locale::global(loc);//恢复全局locale,loc为定义的locale变量

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (m_Belong[i] == m_Belong[0])
        {
            SaveStage(outFile, i);
        }
    }

    outFile.close();//关闭文件流
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveMTSI
// 获得状态变迁分离事例MTSI集合，保存到filename文件中
//
void CGraph::SaveMTSI(char *filename)
{
    ofstream outFile;//输入文件流

    locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
    outFile.open(filename, ios::out);//打开文件
    locale::global(loc);//恢复全局locale,loc为定义的locale变量

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (m_Belong[i] == m_Belong[0])
        {
            EdgeNode *pNode = m_V[i];

            while(NULL != pNode)
            {
                //当前节点是合法节点，与它之间有边的节点是坏节点
                if (m_Belong[pNode->k]!=m_Belong[0])
                {
                    SaveStage(outFile, i);
                    outFile<<"=="<<pNode->tran<<"=> s"<<pNode->k+1<<endl;
                }

                pNode = pNode->next;
            }
        }
    }

    outFile.close();//关闭文件流
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveMFBM
// 获得MFBM集合，保存到filename文件中
//
void CGraph::SaveMFBM(char *filename)
{
    bool *isFBMInclude = new bool[m_iNodeNum];
    ofstream outFile;//输入文件流

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        isFBMInclude[i] = false;
    }

    locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
    outFile.open(filename, ios::out);//打开文件
    locale::global(loc);//恢复全局locale,loc为定义的locale变量

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (m_Belong[i] == m_Belong[0])
        {
            EdgeNode *pNode = m_V[i];

            while(NULL != pNode)
            {
                //当前节点是合法节点，与它之间有边的节点是坏节点
                if (m_Belong[pNode->k]!=m_Belong[0])
                {
                   if (!isFBMInclude[pNode->k])//该状态没有被输出过
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
        if (isFBMInclude[i])//标记为true的都是FBM
        {
            SaveStage(outFile, i);
        }
    }

    outFile.close();//关闭文件流

    delete [] isFBMInclude;
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveMLStar
// 获得ML*，type为1以矩阵保存到filename文件中
//
void CGraph::SaveMLStar(char *filename, int type)
{
    bool *isMLStart = new bool[m_iNodeNum];
    ofstream outFile;//输入文件流

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        isMLStart[i] = true;
    }

    for (int i=0; i!=m_iNodeNum; ++i)//将DZ中的状态标记为false
    {
        if (m_Belong[i] != m_Belong[0])
        {
            isMLStart[i] = false;
        }
    }

    locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
    outFile.open(filename, ios::out);//打开文件
    locale::global(loc);//恢复全局locale,loc为定义的locale变量

    for (int i=0; i!=m_iNodeNum; ++i)
    {   
        if (isMLStart[i])//m_PA[i]没有被A-Cover
        {
            for (int j=0; j!=m_iNodeNum; ++j)
            {
                if (isMLStart[j] && i!=j)//m_PA[j]没有被A-Cover
                {
                    if (isAargGTBarg(i,j))
                    {
                        isMLStart[j] = false;//m_PA[j]被m_PA[i] Cover
                    } 
                }
            }
        }
    }

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (isMLStart[i])//标记为true的都是ML*元素
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

    outFile.close();//关闭文件流

    delete [] isMLStart;
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveMFBMStar
// 获得MFBM*，type为1以矩阵保存到filename文件中
//
void CGraph::SaveMFBMStar(char *filename, int type)
{
    bool *isMFBMStart = new bool[m_iNodeNum];
    bool *isFBMInclude = new bool[m_iNodeNum];
    ofstream outFile;//输入文件流

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
                //当前节点是合法节点，与它之间有边的节点是坏节点
                if (m_Belong[pNode->k]!=m_Belong[0])
                {
                    if (!isFBMInclude[pNode->k])//该状态没有被输出过
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
        if (isFBMInclude[i])//标记为true的都是FBM
        {
            isMFBMStart[i] = true;
        }
    }

    locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
    outFile.open(filename, ios::out);//打开文件
    locale::global(loc);//恢复全局locale,loc为定义的locale变量

    for (int i=0; i!=m_iNodeNum; ++i)
    {   
        if (isMFBMStart[i])//m_PA[i]没有被A-Cover
        {
            for (int j=0; j!=m_iNodeNum; ++j)
            {
                if (isMFBMStart[j] && i!=j)//m_PA[j]没有被A-Cover
                {
                    if (isAargGTBarg(j,i))
                    {
                        isMFBMStart[j] = false;//m_PA[j]被m_PA[i] Cover
                    } 
                }
            }
        }
    }

    for (int i=0; i!=m_iNodeNum; ++i)
    {
        if (isMFBMStart[i])//标记为true的都是ML*元素
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

    outFile.close();//关闭文件流

    delete [] isMFBMStart;
    delete [] isFBMInclude;
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveStage
// 输出Stage nr.及其P.nr和toks到文件
//
void CGraph::SaveStage(ofstream &outFile, int i)
{
    outFile<<"State nr."<<setw(8)<<i+1<<endl;//输出工序号

    outFile<<"P.nr:";//输出机器号
    for (int j=0; m_PToken[i][j]!=-1; ++j)
    {
        outFile<<setw(4)<<j+1;
    }

    outFile<<endl<<"toks:";//输出token数
    for (int j=0; m_PToken[i][j]!=-1; ++j)
    {
        outFile<<setw(4)<<m_PToken[i][j];
    }
    outFile<<endl;
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveAsMatrix
// 以矩阵格式输出PA
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
// 是否A数组中每一个元素大于等于B中对应位置的元素
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
// 计算LastPA，并进行标识
//
void CGraph::CalLastPA(char *filename)
{
    /*int iTokenOfStage1[MAX_P];
    for (int i=0; i!=MAX_P; ++i)
    {
        iTokenOfStage1[i] = -1;
    }

    ifstream inputFile;//输入文件流
    char strInputBuf[1024] = {'\0'};
    int idx = 0;
    bool isPreLineToken = false;//前一行数据是否是token数据
    char strTemp[S_LEN];

    locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
    inputFile.open(filename, ios::in);//打开文件
    locale::global(loc);//恢复全局locale,loc为定义的locale变量

    if (!inputFile)
    {
        MessageBoxA(NULL, "输入文件打开失败！", "错误", MB_OK);
        return;
    }

    while (inputFile.getline(strInputBuf,1024))//按行读入内容
    {
        //有toks标记或前一行数据时token数据，当前行有:，都表明当前行是token数据
        if ((NULL!=strstr(strInputBuf,"toks")) || (isPreLineToken && NULL!=strstr(strInputBuf,":")))
        {
            isPreLineToken = true;

            int iLen = strlen(strInputBuf);
            bool flag = false;
            int iTemp = 0;
            memset(strTemp, 0, S_LEN);//临时字符串

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
                else if (' ' == strInputBuf[i] && !flag)//置flag为true，表示以后遇到空格则有有效数据在strTemp中
                {
                    flag = true;
                }
                if (0 == strcmp(strTemp,":"))//特殊处理多行token值
                {
                    memset(strTemp, 0, S_LEN);
                    iTemp = 0;
                }
            }
            
            iTokenOfStage1[idx++] = atoi(strTemp);//保存最后一个token值
        }
        else if (isPreLineToken)//当前行没有token数据，但是前一行有token数据，即已读取statage 1的所有token数据
        {
            break;//退出while循环
        }
    }

    inputFile.close();//关闭文件流

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