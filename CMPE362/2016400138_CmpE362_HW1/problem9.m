%Problem 9
m = csvread('exampleSignal.csv',3,0);
figure;
plot(m);
figure;
findpeaks(m);
