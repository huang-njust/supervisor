%redundantforiteration.m
%identify the redundant constraints for iteration with beta
clear all;
%clc;%����
fprintf('Running...\n');


%LM=393; %�ǵ��޸ģ���������������������for chen2012fig6
%NA=16;%for chen2012fig6
LM=26; %�ǵ��޸ģ���������������������%for chen2012fig5
NA=11;%for chen2012fig5


input=load('F:/CoefficientsMatrix.txt');
A=zeros(LM,NA+1);
A=input;

%--------------------------ILP��������Լ��ΪA1�齥С--------------------------
count=0;
xcount=0;
myredundancy=zeros(1,LM);
tic;%��ʼ��ʱ
for i=1:LM
        A1=A;
        f=reshape(A1(i,:),1,NA+1);              
        A1(i,:)=[];%ɾ��i������
        ithcount=0;
        for k=1:i-1  %ɾ��������֮ǰ���϶�Ϊ������У������������������Ϊ������ж���ɾ�������
            if myredundancy(1,k)==1
                A1(k-ithcount,:)=[];%ɾ����λ���б仯
                ithcount=ithcount+1;                
            end
        end
        
        %A1=-A1;%��Ϊ����>=1��������<=-1;����-�󣬱�ʾ<=-1
        %f=-f;%��Ϊ����>=1��������<=-1;����-�󣬱�ʾ<=-1
        
        b=zeros(LM-ithcount-1,1);%��ɾ�����кͱ���
        e=-ones(1,LM-ithcount-1);%-1==>less than; 0==>equals; 1==>greater than
        vlb=[];%ȱʡΪ0
        vub=[];
        xint=1:NA+1;%��ʾ��Ϊ��������
        %              max v = f'*x
        %                  a*x <> b
        %                  vlb <= x <= vub
        %                  x(int) are integer
        %[obj,x,duals,stat]= lp_solve(f,A1,b,e,vlb,vub,xint)
        %[obj,x] = lp_solve(f,A1,b,e,vlb,vub,xint)
        [obj,x,duals,stat]= lp_solve(f,A1,b,e,vlb,vub,xint);


        if obj<=0
            %fprintf('Redundancy found!');            
%              if abs(sum(duals)-1)<0.1             
%                  disp([num2str(i),'  sum(duals)==',num2str(sum(duals))]);
%                  return;
%              end    
%              if nnz(duals)>2  %nnzΪ����Ԫ�ظ���
%                  disp(['  i=',num2str(i)]);
%                  %xcount=xcount+1;
%              end
             
            myredundancy(1,i)=1;
            count=count+1;
        end
end
%disp(['xcount=',num2str(xcount)]);
disp(['ILP������Time=',num2str(toc),'�룻����Լ������=',num2str(count)]);
dlmwrite ('F:/ILPredundancy.txt',myredundancy);


%--------------------------1+1>1����---------------------------------
count2=0;%ͳ���������
myredundancy2=zeros(1,LM);
tic;%��ʼ��ʱ
for i=1:LM
        A1=A;
        f=reshape(A1(i,:),1,NA+1);              
        A1(i,:)=[];%ɾ��j������
        
        ithcount=0;
        for k=1:i-1  %ɾ��������֮ǰ���϶�Ϊ������У������������������Ϊ������ж���ɾ�������
            if myredundancy2(1,k)==1
                A1(k-ithcount,:)=[];%ɾ����λ���б仯
                ithcount=ithcount+1;                
            end
        end
        
        %A1=-A1;%��Ϊ����>=1��������<=-1;����-�󣬱�ʾ<=-1
        %f=-f;%��Ϊ����>=1��������<=-1;����-�󣬱�ʾ<=-1
        
        r=0;%0��ʾ������       
        for x=1:LM-ithcount-1
            for y=x+1:LM-ithcount-1
                r=1;
                for z=2:NA+1
                    if 0.5*A1(x,z)+0.5*A1(y,z)<f(1,z)
                        r=0;
                        break;
                    end
                end
                if r==1  %��ʾ�϶�����
                    myredundancy2(1,i)=1;
                    count2=count2+1;
%                     if(i==273)
%                         disp([num2str(i)]);
%                         f(1,:)
%                         A1(x,:)
%                         A1(y,:)
%                     end
                    break;
                end
            end
            if r==1
                    break;
            end
        end     
end       
disp(['1+1>1������Time=',num2str(toc),'�룻����Լ������=',num2str(count2)]);
dlmwrite ('F:/nonILPredundancy.txt',myredundancy2);

fprintf('Finished!');


        








