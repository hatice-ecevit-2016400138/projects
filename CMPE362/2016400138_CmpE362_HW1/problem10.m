%Problem 10
m = imread('lena.png');
g = rgb2gray(m);
disp(strcat('Mean of the picture:' , num2str(mean(g,'all'))));
disp(strcat('Variance of the picture:', num2str(std2(g))));
maxelement = max(g,[],'all');
[maxX,maxY] = find(g == maxelement);
disp(strcat('Max element of the picture:',num2str(maxelement)));
disp(strcat('Location of the max element(row, column):', num2str(maxX) , ',' , num2str(maxY)));
minelement = min(g,[],'all');
[minX, minY] = find(g == minelement);
disp(strcat('Min element of the picture:',num2str(minelement)));
disp(strcat('Location of the min element(row, column): ' , num2str(minX) , ',' , num2str(minY)));
