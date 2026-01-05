% multi_sensor_correlation.m
% Cross-sensor event correlation

clc; clear;

t = 0:10:140;

voc = [120 121 123 130 150 180 220 260 300 310 320 330 335 340 345];
temp = [30 31 31 32 33 35 38 42 46 50 55 60 63 65 66];

figure;
yyaxis left
plot(t, voc, '-o', 'LineWidth', 1.5);
ylabel('VOC signal (relative)');

yyaxis right
plot(t, temp, '--s', 'LineWidth', 1.5);
ylabel('Temperature (°C)');

xlabel('Time (s)');
title('Correlated VOC and temperature rise');
legend('VOC','Temperature');
grid on;
