%����pm=1���±꣬pm=1��L��������MFBM^*��ina���õ�ת�ø�ʽ��transposed incidence matrix(Ҫȥ����P_C);
%���N_c��u_c0���Լ�������INA�Ŀ��ƿ�����ʽ��
%[N_c]=-[L]*[N_P]; u_c0=b-[L]*u_p0 (���P_A�г�ʼʱ���пϣ���u_p0=0,����u_c0=b=E l_i*M(p_i)-1)

clear all;%�������׳���һЩ�������


PM=[1];%����lingo�����pm=1��Ӧ���±�

%LΪn_cXn,nΪP_C
L=[3 3 3 1 1]; 
MFBM=[   
   0   3   0   0   0;
   1   2   0   0   0;
   0   2   0   2   1;
   0   1   1   2   1;
   2   1   0   0   0
];

% NP_transposedΪmXn,mΪT,nΪP_C,������ina,ȥ���˷�P_C:
%INA������P/T-Invariants������I����session�ļ��п��ҵ����������ת�þ���
%��ȥ��noncritical places����Ӧ���У�����idle, resource, and noncritical activity places
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

% NPΪnXm,nΪP_A,mΪT
NP=NP_transposed'



%������ƿ�����ʼ�п�ֵ
a=length(PM);
uc0=zeros(1,a);

for i=1:a
    uc0(1,i)=L(i,:)*MFBM(PM(i),:)'-1;
end
uc0

NC=-L*NP


%���INA���õ�control places��Ϣ
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
        
        



    

