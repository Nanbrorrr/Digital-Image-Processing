clc;clear;

f=imread('bubbles_on_black_background.pgm');
[a,b]=size(f);

%Binarized image
f=im2bw(f);

%Opening
se=strel('disk',3');%Disc structure element
f=imopen(f,se);%Opening

subplot(141);imshow(f);
title('Binarized image','fontname','Microsoft YaHei UI Light','FontSize',16);

% L is the labeling matrix, n is the total number of connected components found
[L,n]=bwlabel(f);


%Only particles that overlap the boundary
f1=f;
for k=1:a
    if(L(k,1)~=0)
        I= L==L(k,1);
        f1(I)=0;
        I=0;
    end
end
for k=1:a
    if(L(k,b)~=0)
        I= L==L(k,b);
        f1(I)=0;
        I=0;
    end
end
for k=1:b
    if(L(1,k)~=0)
        [I]=find(L==L(1,k));
        f1(I)=0;
        I=0;
    end
end
for k=1:b
    if(L(a,k)~=0)
        [I]=find(L==L(a,k));
        f1(I)=0;
        I=0;
    end
end
f1=f-f1;
subplot(142);imshow(f1);

title('Only overlap with the boundary','fontname','Microsoft YaHei UI Light','FontSize',16);

%Only particles that overlap each other, with an area between 300 and 450
f2=f;
for k=1:n
    [r,c]=find(L==k);
    if( size(r,1)>300 && size(r,1)<450 )
        for i=1:size(r,1)
            f2(r(i),c(i))=0;
        end
    end 
end
f2=f-f2;

subplot(143);imshow(f2);
title('No overlap','fontname','Microsoft YaHei UI Light','FontSize',16);

%No overlap
f3=f;
for k=1:n
    [r,c]=find(L==k);
    if( size(r,1)<300 )
        for i=1:size(r,1)
            f3(r(i),c(i))=0;
        end
    end 
end
f3=f-f2;
f3=f3-f1;% Exclude particles that overlap with the boundary
subplot(144);imshow(f3);
title('Only overlap with each other','fontname','Microsoft YaHei UI Light','FontSize',16);