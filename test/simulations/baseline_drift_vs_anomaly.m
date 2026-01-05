% baseline_drift_vs_anomaly.m
% Drift tolerance demonstration

clc; clear;

t = 0:10:200;

% Slow baseline drift
drift = 120 + 0.05 * t;

% Inject anomaly
signal = drift;
signal(t >= 120) = signal(t >= 120) + 80;

% EWMA
alpha = 0.15;
ewma = zeros(size(signal));
ewma(1) = signal(1);
for i = 2:length(signal)
    ewma(i) = alpha*signal(i) + (1-alpha)*ewma(i-1);
end

% CUSUM
k = 3;
h = 20;
cusum = zeros(size(signal));
for i = 2:length(signal)
    cusum(i) = max(0, cusum(i-1) + ewma(i) - k);
end

% Plot
figure;
subplot(2,1,1);
plot(t, signal, '-o'); hold on;
plot(t, ewma, 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('Signal');
title('Baseline drift with injected anomaly');
legend('Raw','EWMA');
grid on;

subplot(2,1,2);
plot(t, cusum, 'LineWidth', 1.5);
yline(h, '--r', 'Threshold');
xlabel('Time (s)');
ylabel('CUSUM');
title('CUSUM response');
grid on;
