#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

using namespace std;

const int MAX_N = 100000;//最大Stage数
const int MAX_P = 1000;  //最大库所数
const int MAX_UT = 100;  //最大不可控变迁个数
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
    bool GetSConnection();                            //遍历所有顶点，计算强连通分量
	bool isConByUnconTran(int iML, int iMB);          //从iML节点是否存在通过不可控变迁到达iMB节点的路径
	bool isInUnconTranSet(char *tran);                //变迁tran是否在不可控变迁集合中
    void SaveML(char *filename);                      //获得合法状态集合ML，保存到filename文件中
    void SaveMTSI(char *filename);                    //获得状态变迁分离事例MTSI集合，保存到filename文件中
    void SaveMFBM(char *filename);                    //获得MFBM集合，保存到filename文件中
    void SaveMLStar(char *filename, int type=0);      //获得ML*，type为1以矩阵保存到filename文件中
    void SaveMFBMStar(char *filename, int type=0);    //获得MFBM*，type为1以矩阵保存到filename文件中
	void SetUnconTranSet(char *trans);                //从输入框中获得不可控变迁集合
	void SaveAsTempFile();                            //把本次的SCC去除UDM后保存为下次的输入文件
	bool GetConFlag();                                //获得是否计算的标志
	void ResetBelongArr();                            //根据最后计算出的SCC和原始输入文件的图结构重新计算m_Belong数组
	
private:
	int SplitStr(char *str, char *strTran);           //获得输入字符串空格后面的内容
    void InitTokenArg(int index, char *str);          //从输入字符串提取token值
    void InitPAFromToken(int index);                  //根据token值初始化PA数组
	void InsertEdge(int idx, int data, char *strTran);//插入邻接表节点
    void Tarjan(int i);                               //Tarjan算法获得图的强连通分量
	void GetConWithUnconTran();                       //从强连通分量中删除 “可通过一个或多个连续的不可控变迁到达强连通图之外节点” 的节点
    void SaveStage(ofstream &outFile, int n);         //输出Stage nr.及其P.nr和toks到文件
    void SaveAsMatrix(ofstream &outFile, int n);      //以矩阵格式输出PA
    bool isAargGTBarg(int idxA, int idxB);            //是否A数组中每一个元素大于等于B中对应位置的元素

private:
	EdgeNode *m_V[MAX_N];      //邻接表
	int m_DFN[MAX_N];          //节点DFN(u)搜索的次序编号
	int m_LOW[MAX_N];          //Low(u)为u或u的子树能够追溯到的最早的栈中节点的次序号
	int m_Stop;
    int m_Bcnt;
    int m_Dindex;
	int m_Instack[MAX_N];
	int m_Stap[MAX_N];
    int m_PA[MAX_N][MAX_P];     //PA库所
    int m_PToken[MAX_N][MAX_P]; //库所的token数
    bool m_isML[MAX_N];         //是否在ML中
    bool m_isMFBM[MAX_N];       //是否在MFBM
	char m_UnconTran[MAX_UT][T_LEN];//保存不可控变迁
	bool m_isConCal;              //是否停止计算的标志

public:
	int m_Belong[MAX_N];
	int m_iNodeNum;            //状态数目
};
