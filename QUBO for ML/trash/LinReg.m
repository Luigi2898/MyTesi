clear all
close all
clc

Nx = 100;
Xstart = 0;
Xstop = 1;
x = linspace(Xstart, Xstop, Nx)';
y = x + 1;
d = 1;

figure
plot(x, y, 'o')

x = [x ones(Nx,1)];
K = 3;
twos = linspace(-3, -1, K); 
P = [-2.^twos 2.^twos];
P = sort(P);
K = 2 * K;
I_d1 = eye(d + 1);
Pc = kron(I_d1, P);

A = (Pc')*(x')*x*Pc;
b = -2*(Pc')*(x')*y;

Nw = K * (d + 1);

tmp = [0:(2^Nw) - 1];

bin = dec2bin(tmp);

for i = [1:(2^Nw)]
    for j = [1:Nw]
        binN(i,j) = str2double(bin(i,j));
    end
end

for i = [1:(2^Nw)]
    
end

