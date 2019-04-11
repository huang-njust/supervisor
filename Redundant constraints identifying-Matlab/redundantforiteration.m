%redundantforiteration.m
%identify the redundant constraints for iteration with beta
clear all;
%clc;%清屏
fprintf('Running...\n');


%LM=393; %记得修改！！！！！！！！！！！for chen2012fig6
%NA=16;%for chen2012fig6
LM=26; %记得修改！！！！！！！！！！！%for chen2012fig5
NA=11;%for chen2012fig5


input=load('F:/CoefficientsMatrix.txt');
A=zeros(LM,NA+1);
A=input;

%--------------------------ILP方法，且约束为A1块渐小--------------------------
count=0;
xcount=0;
myredundancy=zeros(1,LM);
tic;%开始计时
for i=1:LM
        A1=A;
        f=reshape(A1(i,:),1,NA+1);              
        A1(i,:)=[];%删除i所在行
        ithcount=0;
        for k=1:i-1  %删除本块中之前已认定为冗余的行，这样避免出现两个互为冗余的行都被删除的情况
            if myredundancy(1,k)==1
                A1(k-ithcount,:)=[];%删除后位置有变化
                ithcount=ithcount+1;                
            end
        end
        
        %A1=-A1;%因为来自>=1，而不是<=-1;加了-后，表示<=-1
        %f=-f;%因为来自>=1，而不是<=-1;加了-后，表示<=-1
        
        b=zeros(LM-ithcount-1,1);%已删冗余行和本行
        e=-ones(1,LM-ithcount-1);%-1==>less than; 0==>equals; 1==>greater than
        vlb=[];%缺省为0
        vub=[];
        xint=1:NA+1;%表示都为整数变量
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
%              if nnz(duals)>2  %nnz为非零元素个数
%                  disp(['  i=',num2str(i)]);
%                  %xcount=xcount+1;
%              end
             
            myredundancy(1,i)=1;
            count=count+1;
        end
end
%disp(['xcount=',num2str(xcount)]);
disp(['ILP方法：Time=',num2str(toc),'秒；冗余约束个数=',num2str(count)]);
dlmwrite ('F:/ILPredundancy.txt',myredundancy);


%--------------------------1+1>1方法---------------------------------
count2=0;%统计冗余个数
myredundancy2=zeros(1,LM);
tic;%开始计时
for i=1:LM
        A1=A;
        f=reshape(A1(i,:),1,NA+1);              
        A1(i,:)=[];%删除j所在行
        
        ithcount=0;
        for k=1:i-1  %删除本块中之前已认定为冗余的行，这样避免出现两个互为冗余的行都被删除的情况
            if myredundancy2(1,k)==1
                A1(k-ithcount,:)=[];%删除后位置有变化
                ithcount=ithcount+1;                
            end
        end
        
        %A1=-A1;%因为来自>=1，而不是<=-1;加了-后，表示<=-1
        %f=-f;%因为来自>=1，而不是<=-1;加了-后，表示<=-1
        
        r=0;%0表示不冗余       
        for x=1:LM-ithcount-1
            for y=x+1:LM-ithcount-1
                r=1;
                for z=2:NA+1
                    if 0.5*A1(x,z)+0.5*A1(y,z)<f(1,z)
                        r=0;
                        break;
                    end
                end
                if r==1  %表示认定冗余
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
disp(['1+1>1方法：Time=',num2str(toc),'秒；冗余约束个数=',num2str(count2)]);
dlmwrite ('F:/nonILPredundancy.txt',myredundancy2);

fprintf('Finished!');


        








