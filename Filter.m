%M = csvread("Wave Data.csv",",");
t = M(:,1);
x = M(:,2);
windowSize = 5; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;
y = filter(b,a,x);
plot(t,x)
hold on
plot(t,y)
legend('Input Data','Filtered Data')