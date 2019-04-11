%输入pm=1的下标，pm=1的L，完整的MFBM^*，ina所得的转置格式的transposed incidence matrix(要去掉非P_C);
%输出N_c和u_c0，以及可用于INA的控制库所格式。
%[N_c]=-[L]*[N_P]; u_c0=b-[L]*u_p0 (如果P_A中初始时无托肯，则u_p0=0,所以u_c0=b=E l_i*M(p_i)-1)

clear all;%否则容易出现一些奇怪问题


PM=[1];%给出lingo结果中pm=1对应的下标

%L为n_cXn,n为P_C
L=[3 3 3 1 1]; 
MFBM=[   
   0   3   0   0   0;
   1   2   0   0   0;
   0   2   0   2   1;
   0   1   1   2   1;
   2   1   0   0   0
];

% NP_transposed为mXn,m为T,n为P_C,来自于ina,去掉了非P_C:
%INA运行求P/T-Invariants的命令I后，在session文件中可找到关联矩阵的转置矩阵，
%并去掉noncritical places所对应的列，包括idle, resource, and noncritical activity places
NP_transposed=[
 0   0   0    0   0;                   
 0   0   0    0   0;                   
 0   1   0    0   0;                   
 1   0   0    0   0;                   
-1   1   0    0   0;                   
 0  -1   1    0   0;                   
 0   0  -1    0   0;                   
 0   0   0    0   0;                   
 0   0  -1    0   0;                   
 0   0   0    0   0;                   
 0   0   0    1   0;                   
 0   0   0   -1   1;                   
 0   0   0    0  -1;                   
 0   0   0    0   0;                   
 0   0   0    0   0
    ];

% NP为nXm,n为P_A,m为T
NP=NP_transposed'



%计算控制库所初始托肯值
a=length(PM);
uc0=zeros(1,a);

for i=1:a
    uc0(1,i)=L(i,:)*MFBM(PM(i),:)'-1;
end
uc0

NC=-L*NP


%输出INA可用的control places信息
[x,y]=size(NC);
for i=1:x
    fprintf('%i ',uc0(1,i));
    for j=1:y
        if NC(i,j)>0
            if NC(i,j)>1
                fprintf('%i:%i ',j,NC(i,j));
            end
            if NC(i,j)==1
                fprintf('%i ',j);
            end
        end
    end
    fprintf(',');
    for j=1:y
        if NC(i,j)<0
            if NC(i,j)<-1
                fprintf('%i:%i ',j,-NC(i,j));
            end
            if NC(i,j)==-1
                fprintf('%i ',j);
            end
        end
    end
    fprintf('\n');
end
        
        



    

