% ewma_cusum_basic.m
% Base case: noise suppression and change detection

clc; clear;

t = 0:10:180;

% Synthetic MQ-like signal
raw = [120 121 119 122 121 120 150 190 240 300 320 ...
       315 300 260 220 180 150 130 125];

% EWMA
alpha = 0.2;
ewma = zeros(size(raw));
ewma(1) = raw(1);
for i = 2:length(raw)
    ewma(i) = alpha*raw(i) + (1-alpha)*ewma(i-1);
end

% CUSUM (one-sided)
k = 5;
h = 25;
cusum = zeros(size(raw));
for i = 2:length(raw)
    cusum(i) = max(0, cusum(i-1) + ewma(i) - k);
end

% Plot
figure;
yyaxis left
plot(t, raw, '-o', 'DisplayName', 'Raw signal'); hold on;
plot(t, ewma, '-s', 'LineWidth', 1.5, 'DisplayName', 'EWMA');

yyaxis right
plot(t, cusum, '--', 'LineWidth', 1.5, 'DisplayName', 'CUSUM');

xlabel('Time (s)');
ylabel('Signal / Statistic');
title('EWMA smoothing and CUSUM change detection');
legend;
grid on;
