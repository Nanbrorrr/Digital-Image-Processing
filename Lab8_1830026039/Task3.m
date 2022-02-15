A = imread('connected.pgm');

A = im2bw(A);
figure(1);
subplot(1,2,1);
imshow(A);
title('Binarized Image');
A_ct = ~A;
[m,n] = size(A);
X = zeros(m,n);
[k,z] = find(A==1); 
X(k(1),z(1)) = 1;
se = [0,1,0;1,1,1;0,1,0];
over = 0; 
i = 0;
while(~over)
    i = i + 1;
    Xp = X;
    X = imdilate(X,se) & A_ct;
    if X == Xp 
        over = 1;         
    end
end
A = ~X | A;
subplot(1,2,2);
imshow(A);
title('Padding Result');

A0 = A;
X1 = zeros(m,n);
B = zeros(m,n);
count = 1;
figure(2);
i = 1; 
j = 1;
while(length(find(A==1)))
    [k,z] = find(A==1);
    X1(k(1),z(1)) = 1;
    se = ones(3);

    while(1)
        Xp1 = X1;
        X1 = imdilate(X1,se) & A;
        if X1 == Xp1
            break;        
        end
    end
    B = B + X1; 
    num(count) = length(find(X1==1));
    count = count + 1;
    A = A - B;
    imshow(B);
    title('The Extraction Process of Each Connected Component');
    hold on;
end
hold off;
fid = fopen('output.txt','wt');
fprintf('Connected Component     pixels\n');
fprintf(fid,'%s\n','Connected Component     pixels');
for i = 1:count-1
    fprintf('        %2d                            %d\n',i,num(i));
    fprintf(fid,'        %2d                            %d\n',i,num(i));
end
figure(3);
subplot(1,2,1);
imshow(A0);
title('Source Image');
subplot(1,2,2);
imshow(B);
title('Extract All the Connected Components');
fclose(fid);