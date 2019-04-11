%redundant.m
%identify the redundant constraints
clear all;
%clc;%����
fprintf('Running...\n');

FBM=13; %�ǵ��޸ģ���������������������
LM=112; %�ǵ��޸ģ���������������������
NA=16;

input=load('F:/CoefficientsMatrix.txt');
A=zeros(FBM,LM,NA);
for i=1:FBM
    A(i,:,:)=input((i-1)*LM+1:i*LM,:);
end

%--------------------------ILP��������Լ��ΪA1�齥С--------------------------
count=0;
myredundancy=zeros(1,FBM*LM);
tic;%��ʼ��ʱ
for i=1:FBM
    for j=1:LM
        A1=reshape(A(i,:,:),LM,NA);
        f=reshape(A1(j,:),1,NA);              
        A1(j,:)=[];%ɾ��j������
        ithcount=0;
        for k=1:j-1  %ɾ��������֮ǰ���϶�Ϊ������У������������������Ϊ������ж���ɾ�������
            if myredundancy(1,(i-1)*LM+k)==1
                A1(k-ithcount,:)=[];%ɾ����λ���б仯
                ithcount=ithcount+1;                
            end
        end
        
        A1=-A1;%��Ϊ����>=1��������<=-1;����-�󣬱�ʾ<=-1
        f=-f;%��Ϊ����>=1��������<=-1;����-�󣬱�ʾ<=-1
        
        b=-ones(LM-ithcount-1,1);%��ɾ�����кͱ���
        e=-ones(1,LM-ithcount-1);%-1==>less than; 0==>equals; 1==>greater than
        vlb=[];%ȱʡΪ0
        vub=[];
        xint=1:NA;%��ʾ��Ϊ��������
        %              max v = f'*x
        %                  a*x <> b
        %                  vlb <= x <= vub
        %                  x(int) are integer
        %[obj,x,duals,stat]= lp_solve(f,A1,b,e,vlb,vub,xint)
        %[obj,x] = lp_solve(f,A1,b,e,vlb,vub,xint)
        [obj,x,duals,stat]= lp_solve(f,A1,b,e,vlb,vub,xint);


        if obj<=-1
            %fprintf('Redundancy found!');
%             if abs(sum(duals)-1)>0.1
%                 disp([num2str((i-1)*LM+j),'  sum(duals)==',num2str(sum(duals))]);
%             end
            myredundancy(1,(i-1)*LM+j)=1;
            count=count+1;
        end
    end
end
disp(['ILP����(�ֿ�)��Time=',num2str(toc),'�룻����Լ������=',num2str(count)]);
dlmwrite ('F:/ILPredundancy.txt',myredundancy);


%--------------------------1+1>1����---------------------------------
count2=0;%ͳ���������
myredundancy2=zeros(1,FBM*LM);
tic;%��ʼ��ʱ
for i=1:FBM
    for j=1:LM
        A1=reshape(A(i,:,:),LM,NA);
        f=reshape(A1(j,:),1,NA);              
        A1(j,:)=[];%ɾ��j������
        
        ithcount=0;
        for k=1:j-1  %ɾ��������֮ǰ���϶�Ϊ������У������������������Ϊ������ж���ɾ�������
            if myredundancy2(1,(i-1)*LM+k)==1
                A1(k-ithcount,:)=[];%ɾ����λ���б仯
                ithcount=ithcount+1;                
            end
        end
        
        A1=-A1;%��Ϊ����>=1��������<=-1;����-�󣬱�ʾ<=-1
        f=-f;%��Ϊ����>=1��������<=-1;����-�󣬱�ʾ<=-1
        
        r=0;%0��ʾ������
        for x=1:LM-ithcount-1
            for y=x+1:LM-ithcount-1
                r=1;
                for z=1:NA
                    if A1(x,z)+A1(y,z)<f(1,z)
                        r=0;
                        break;
                    end
                end
                if r==1  %��ʾ�϶�����
                    myredundancy2((i-1)*LM+j)=1;
                    count2=count2+1;
                    break;
                end
            end
            if r==1
                    break;
            end
        end     
    end
end       
disp(['1+1>1������Time=',num2str(toc),'�룻����Լ������=',num2str(count2)]);
dlmwrite ('F:/nonILPredundancy.txt',myredundancy2);

fprintf('Finished!');

        








