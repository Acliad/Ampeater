A = [0 0 1;
     0 1 0;
     0 1 1;
     1 0 0;
     1 0 1;
     1 1 0;
     1 1 1]

%% Series
H = (1:7)'/8;
b = H./(1-H)

R = A\b

y = A*R

figure(1);
clf
Hactual = y./(y+1);
plot(Hactual, 'LineWidth', 2)
hold on
plot(H, 'LineWidth', 2)

ax = gca;
xlabels = ax.XTickLabel;
xnums = cellfun(@str2num, (xlabels(:)));
xlabels_bin = cellstr(num2str(dec2bin(xnums)));
ax.XTickLabel = xlabels_bin;

set(ax, 'FontSize', 20);

xlabel('Button Combination', 'FontSize', 24)
ylabel('Voltage Divider Ratio', 'FontSize', 24)
grid on

%% Parallel
Ra = 1.5;
Vcc = 3.3;
Vmax = 1;
H = linspace(((Vmax)/Vcc)/8, (Vmax)/Vcc, 7)'
Req = Ra * (1./H -1)
Reqinv = 1./Req;

Rinv = A\Reqinv;

R = 1./Rinv

Hactual = Ra./(Ra + 1./(A*Rinv))

figure(2);
clf
plot(Hactual, 'LineWidth', 2)
hold on
plot(H, 'LineWidth', 2)

ax = gca;
xlabels = ax.XTickLabel;
xnums = cellfun(@str2num, (xlabels(:)));
xlabels_bin = cellstr(num2str(dec2bin(xnums)));
ax.XTickLabel = xlabels_bin;

set(ax, 'FontSize', 20);

xlabel('Button Combination', 'FontSize', 24)
ylabel('Voltage Divider Ratio', 'FontSize', 24)
grid on