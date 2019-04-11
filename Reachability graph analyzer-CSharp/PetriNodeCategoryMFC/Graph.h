#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

using namespace std;

const int MAX_N = 100000;//最大Stage数
const int MAX_P = 1000;   //最大库所数
const int T_LEN = 5;     //变迁字符串长度
const int S_LEN = 8;     //Stage号最大长度

struct EdgeNode      //邻接表节点数据类型
{
	int k;           //节点编号
    char tran[T_LEN];//变迁
	EdgeNode * next; //下一个节点

	EdgeNode()       //数据初始化
	{
		k = -1;
        memset(tran, 0, T_LEN);
		next = NULL;
	}
};

class CGraph
{
public:
	CGraph(void);
	~CGraph(void);

	bool CreateGraphFromFile(char *filename);         //从文件读入数据，生成图的邻接表
	void PrintEdgeNode();                             //打印邻接表节点
    void GetSConnection();                            //遍历所有顶点，计算强连通分量
    void SaveML(char *filename);                      //获得合法状态集合ML，保存到filename文件中
    void SaveMTSI(char *filename);                    //获得状态变迁分离事例MTSI集合，保存到filename文件中
    void SaveMFBM(char *filename);                    //获得MFBM集合，保存到filename文件中
    void SaveMLStar(char *filename, int type=0);      //获得ML*，type为1以矩阵保存到filename文件中
    void SaveMFBMStar(char *filename, int type=0);    //获得MFBM*，type为1以矩阵保存到filename文件中
	void CalLastPA(char *filename);                   //计算LastPA，并进行标识
private:
	int SplitStr(char *str, char *strTran);           //获得输入字符串空格后面的内容
    void InitTokenArg(int index, char *str);          //从输入字符串提取token值
    void InitPAFromToken(int index);                  //根据token值初始化PA数组
	void InsertEdge(int idx, int data, char *strTran);//插入邻接表节点
    void Tarjan(int i);                               //Tarjan算法获得图的强连通分量
    void SaveStage(ofstream &outFile, int n);         //输出Stage nr.及其P.nr和toks到文件
    void SaveAsMatrix(ofstream &outFile, int n);      //以矩阵格式输出PA
    bool isAargGTBarg(int idxA, int idxB);            //是否A数组中每一个元素大于等于B中对应位置的元素


private:
	int m_iNodeNum;            //状态数目
	EdgeNode *m_V[MAX_N];      //邻接表
	int m_DFN[MAX_N];          //节点DFN(u)搜索的次序编号
	int m_LOW[MAX_N];          //Low(u)为u或u的子树能够追溯到的最早的栈中节点的次序号
	int m_Stop;
    int m_Bcnt;
    int m_Dindex;
	int m_Instack[MAX_N];
	int m_Stap[MAX_N];
	int m_Belong[MAX_N];
    int m_PA[MAX_N][MAX_P];     //PA库所
    int m_PToken[MAX_N][MAX_P]; //库所的token数
    bool m_isML[MAX_N];         //是否在ML中
    bool m_isMFBM[MAX_N];       //是否在MFBM
    bool m_isLastPA[MAX_P];     //是否是某个产品的最后一道工序所对应的activity place
};
