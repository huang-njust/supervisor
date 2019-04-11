#pragma once

#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>

using namespace std;

const int MAX_N = 100000;//���Stage��
const int MAX_P = 1000;   //��������
const int T_LEN = 5;     //��Ǩ�ַ�������
const int S_LEN = 8;     //Stage����󳤶�

struct EdgeNode      //�ڽӱ�ڵ���������
{
	int k;           //�ڵ���
    char tran[T_LEN];//��Ǩ
	EdgeNode * next; //��һ���ڵ�

	EdgeNode()       //���ݳ�ʼ��
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

	bool CreateGraphFromFile(char *filename);         //���ļ��������ݣ�����ͼ���ڽӱ�
	void PrintEdgeNode();                             //��ӡ�ڽӱ�ڵ�
    void GetSConnection();                            //�������ж��㣬����ǿ��ͨ����
    void SaveML(char *filename);                      //��úϷ�״̬����ML�����浽filename�ļ���
    void SaveMTSI(char *filename);                    //���״̬��Ǩ��������MTSI���ϣ����浽filename�ļ���
    void SaveMFBM(char *filename);                    //���MFBM���ϣ����浽filename�ļ���
    void SaveMLStar(char *filename, int type=0);      //���ML*��typeΪ1�Ծ��󱣴浽filename�ļ���
    void SaveMFBMStar(char *filename, int type=0);    //���MFBM*��typeΪ1�Ծ��󱣴浽filename�ļ���
	void CalLastPA(char *filename);                   //����LastPA�������б�ʶ
private:
	int SplitStr(char *str, char *strTran);           //��������ַ����ո���������
    void InitTokenArg(int index, char *str);          //�������ַ�����ȡtokenֵ
    void InitPAFromToken(int index);                  //����tokenֵ��ʼ��PA����
	void InsertEdge(int idx, int data, char *strTran);//�����ڽӱ�ڵ�
    void Tarjan(int i);                               //Tarjan�㷨���ͼ��ǿ��ͨ����
    void SaveStage(ofstream &outFile, int n);         //���Stage nr.����P.nr��toks���ļ�
    void SaveAsMatrix(ofstream &outFile, int n);      //�Ծ����ʽ���PA
    bool isAargGTBarg(int idxA, int idxB);            //�Ƿ�A������ÿһ��Ԫ�ش��ڵ���B�ж�Ӧλ�õ�Ԫ��


private:
	int m_iNodeNum;            //״̬��Ŀ
	EdgeNode *m_V[MAX_N];      //�ڽӱ�
	int m_DFN[MAX_N];          //�ڵ�DFN(u)�����Ĵ�����
	int m_LOW[MAX_N];          //Low(u)Ϊu��u�������ܹ�׷�ݵ��������ջ�нڵ�Ĵ����
	int m_Stop;
    int m_Bcnt;
    int m_Dindex;
	int m_Instack[MAX_N];
	int m_Stap[MAX_N];
	int m_Belong[MAX_N];
    int m_PA[MAX_N][MAX_P];     //PA����
    int m_PToken[MAX_N][MAX_P]; //������token��
    bool m_isML[MAX_N];         //�Ƿ���ML��
    bool m_isMFBM[MAX_N];       //�Ƿ���MFBM
    bool m_isLastPA[MAX_P];     //�Ƿ���ĳ����Ʒ�����һ����������Ӧ��activity place
};
