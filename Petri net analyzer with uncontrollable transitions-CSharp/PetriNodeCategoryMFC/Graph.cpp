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
		m_DFN[i] = 0;
		m_LOW[i] = 0; 
		m_Instack[i] = 0;
		m_Stap[i] = 0;  
		m_Belong[i] = 0;
	}

	m_Stop = 0;
	m_Bcnt = 0;
	m_Dindex = 0;
	m_isConCal = false;

	for (int j=0; j!=MAX_UT; ++j)
	{
		memset(m_UnconTran[j],0,T_LEN);
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
    //初始化图相关的类变量，之所在此处而不是构造函数中进行初始化，是因为会多次调用此函数进行图构造
	m_iNodeNum = 0;

	for (int i=0; i!=MAX_N; ++i)
	{
		m_V[i] = NULL;
		for (int j=0; j!=MAX_P; ++j)
		{
			m_PA[i][j] = -1;
			m_PToken[i][j] = -1;
		}
	}
	//类变量初始化完成


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
        if (0 == m_PToken[0][i])//Stage 1中token数为0即为PA
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
bool CGraph::GetSConnection()
{
	//计算强连通分量
	for (int i=0; i<m_iNodeNum; i++)
	{
		if (!m_DFN[i])
		{
			Tarjan(i);
		}
	}

	for (int i=0; i<m_iNodeNum; i++)//从1开始以避免m_Belong[i]++中改变m_Belong[0]的值，该值是一个节点属于强连通图的标志
	{
		if (m_Belong[i] == m_Belong[0])//i在League Marking中
		{
			for (int j=0; j<m_iNodeNum; j++)
			{
				if (m_Belong[j] != m_Belong[0])//j在Bad Marking中
				{
					if (isConByUnconTran(i,j))//判断是否存在从i到j的只通过不可控变迁的路径
					{
						m_Belong[i]++;//从原来的强连通分量重去除该节点
						m_isConCal = true;//需要继续计算SCC
						if (i == 0)
						{
							return false;//如果M0到一个bad marking通过不可控变迁可达，则直接退出计算过程，并弹出对话框提醒
						}
						break;
					}
				}
			}
		}
	}

	return true;
}

// ////////////////////////////////////////////////////////////////////////////////
// isConByUnconTran(int iML, int iMB)
// 从iML节点是否存在通过不可控变迁到达iMB节点的路径
//
bool CGraph::isConByUnconTran(int iML, int iMB)
{
	EdgeNode *pNode = m_V[iML];
	
	while (NULL != pNode)//深度优先搜索
	{
		if (isInUnconTranSet(pNode->tran))//该变迁在不可控变迁集合中
		{
			if (pNode->k == iMB)//找到iMB节点
			{
				return true;
			}
			if (isConByUnconTran(pNode->k,iMB))//深搜
			{
				return true;
			}
		}

		pNode = pNode->next;//深搜下一个节点
	}

	return false;
}

// ////////////////////////////////////////////////////////////////////////////////
// isInUnconTranSet(char *tran)
// 变迁tran是否在不可控变迁集合中
//
bool CGraph::isInUnconTranSet(char *tran)
{
	for (int i=0; i!=MAX_UT; ++i)
	{
		if (0 == strcmp(m_UnconTran[i],tran))
		{
			return true;
		}
		if (0 == strcmp(m_UnconTran[i],""))
		{
			return false;
		}
	}

	return false;
}

// ////////////////////////////////////////////////////////////////////////////////
// GetConWithUnconTran
// 从强连通分量中删除 “可通过一个或多个连续的不可控变迁到达强连通图之外节点” 的节点
//
void CGraph::GetConWithUnconTran()
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
// SetUnconTranSet
// 从输入框中获得不可控变迁集合
//
void CGraph::SetUnconTranSet(char *strUnconTran)
{
	if (0 == strcmp(strUnconTran,""))
	{
		return;
	}

	int itx = 0;
	int idx = 0;
	char tran[T_LEN];
	memset(tran, 0, T_LEN);
	//读入不可控变迁
	for (int i=0; i!=strlen(strUnconTran); ++i)
	{
		if (strUnconTran[i] == ',')
		{
			strcpy(m_UnconTran[idx],"t");
			strcat(m_UnconTran[idx++],tran);
			memset(tran, 0, T_LEN);
			itx = 0;
		}
		else
		{
			tran[itx++]=strUnconTran[i];
		}
	}
	strcpy(m_UnconTran[idx],"t");
	strcat(m_UnconTran[idx++],tran);
}

// ////////////////////////////////////////////////////////////////////////////////
// SaveAsTempFile
// 把本次的SCC去除UDM后保存为下次的输入文件
//
void CGraph::SaveAsTempFile()
{
	ofstream outFile;//输入文件流

	locale loc = locale::global(locale("")); //要打开的文件路径含中文，设置全局locale为本地环境
	outFile.open("tempinput.txt", ios::out);//打开文件
	locale::global(loc);//恢复全局locale,loc为定义的locale变量

	for (int i=0; i!=m_iNodeNum; ++i)
	{
		if (m_Belong[i] == m_Belong[0])
		{
			SaveStage(outFile, i);

			EdgeNode *pNode = m_V[i];
			while(NULL != pNode)
			{
				//当前节点是合法节点，与它之间有边的节点是坏节点
				if (m_Belong[pNode->k]==m_Belong[0])
				{
					outFile<<"=="<<pNode->tran<<"=> s"<<pNode->k+1<<endl;
				}

				pNode = pNode->next;
			}
		}
	}

	outFile.close();//关闭文件流
}

// ////////////////////////////////////////////////////////////////////////////////
// GetConFlag
// 获得是否计算的标志
//
bool CGraph::GetConFlag()
{
	return m_isConCal;
}

// ////////////////////////////////////////////////////////////////////////////////
// ResetBelongArr
// 根据最后计算出的SCC和原始输入文件的图结构重新计算m_Belong数组
//
void CGraph::ResetBelongArr()
{
	for (int i=0; i!=m_iNodeNum; ++i)
	{
		if (m_Belong[i] != m_Belong[0])
		{
			;
		}
	}
}