x = M(:,2);
t = M(:,1);

Fs = 1/(t(2)-t(1));
Fn = Fs/2;
L = size(t,1);
NFFT = 2^nextpow2(L);
FTx = fft(x-mean(x),NFFT)/L;
Fv = linspace(0, 1, NFFT/2+1)*Fn;
Iv = 1:numel(Fv);
figure
y = abs(FTx(Iv))*2;
plot(Fv, y)
grid
xlabel('Frequency')
ylabel('Maagnitude')
title('Fourier Transform of Acceleration Data')
data = [Fv.',y];