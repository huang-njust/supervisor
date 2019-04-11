%redundant.m
%identify the redundant constraints
clear all;
%clc;%清屏
fprintf('Running...\n');

FBM=13; %记得修改！！！！！！！！！！！
LM=112; %记得修改！！！！！！！！！！！
NA=16;

input=load('F:/CoefficientsMatrix.txt');
A=zeros(FBM,LM,NA);
for i=1:FBM
    A(i,:,:)=input((i-1)*LM+1:i*LM,:);
end

%--------------------------ILP方法，且约束为A1块渐小--------------------------
count=0;
myredundancy=zeros(1,FBM*LM);
tic;%开始计时
for i=1:FBM
    for j=1:LM
        A1=reshape(A(i,:,:),LM,NA);
        f=reshape(A1(j,:),1,NA);              
        A1(j,:)=[];%删除j所在行
        ithcount=0;
        for k=1:j-1  %删除本块中之前已认定为冗余的行，这样避免出现两个互为冗余的行都被删除的情况
            if myredundancy(1,(i-1)*LM+k)==1
                A1(k-ithcount,:)=[];%删除后位置有变化
                ithcount=ithcount+1;                
            end
        end
        
        A1=-A1;%因为来自>=1，而不是<=-1;加了-后，表示<=-1
        f=-f;%因为来自>=1，而不是<=-1;加了-后，表示<=-1
        
        b=-ones(LM-ithcount-1,1);%已删冗余行和本行
        e=-ones(1,LM-ithcount-1);%-1==>less than; 0==>equals; 1==>greater than
        vlb=[];%缺省为0
        vub=[];
        xint=1:NA;%表示都为整数变量
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
disp(['ILP方法(分块)：Time=',num2str(toc),'秒；冗余约束个数=',num2str(count)]);
dlmwrite ('F:/ILPredundancy.txt',myredundancy);


%--------------------------1+1>1方法---------------------------------
count2=0;%统计冗余个数
myredundancy2=zeros(1,FBM*LM);
tic;%开始计时
for i=1:FBM
    for j=1:LM
        A1=reshape(A(i,:,:),LM,NA);
        f=reshape(A1(j,:),1,NA);              
        A1(j,:)=[];%删除j所在行
        
        ithcount=0;
        for k=1:j-1  %删除本块中之前已认定为冗余的行，这样避免出现两个互为冗余的行都被删除的情况
            if myredundancy2(1,(i-1)*LM+k)==1
                A1(k-ithcount,:)=[];%删除后位置有变化
                ithcount=ithcount+1;                
            end
        end
        
        A1=-A1;%因为来自>=1，而不是<=-1;加了-后，表示<=-1
        f=-f;%因为来自>=1，而不是<=-1;加了-后，表示<=-1
        
        r=0;%0表示不冗余
        for x=1:LM-ithcount-1
            for y=x+1:LM-ithcount-1
                r=1;
                for z=1:NA
                    if A1(x,z)+A1(y,z)<f(1,z)
                        r=0;
                        break;
                    end
                end
                if r==1  %表示认定冗余
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
disp(['1+1>1方法：Time=',num2str(toc),'秒；冗余约束个数=',num2str(count2)]);
dlmwrite ('F:/nonILPredundancy.txt',myredundancy2);

fprintf('Finished!');

        








