%% clean workspace
clearvars;
close all;

%% import data from excel
% datasets characteristics
[datasets.params, datasets.names] = xlsread("knn_datasets.xlsx", 1, "A2:E9");
datasets.params(6, :) = [];
datasets.names(6)     = [];
datasets.names(3)     = {'Breast Cancer'};

% power values estimated by vivado
system.static  = 0.159;
system.dynamic = 2.094;

% timings spent by accelerators
accel.freq         = 100E6;
accel.cores        = xlsread("knn_datasets.xlsx", 4, "A1:G1");
accel.time         = zeros(7, length(accel.cores));
accel.time(1:5, :) = xlsread("knn_datasets.xlsx", 4, "A2:G6");
accel.time(6:7, :) = xlsread("knn_datasets.xlsx", 4, "A8:G9");
accel.time         = accel.time ./ 1E6;

% processor times
processor.time      = zeros(7,1);
processor.time(1:5) = xlsread("knn_datasets.xlsx", 5, "B2:B6");
processor.time(6:7) = xlsread("knn_datasets.xlsx", 5, "B8:B9");

% another case study
zynq.var_knn.num        = xlsread("knn_datasets.xlsx", 8, "B1:I1");
zynq.var_knn.luts       = xlsread("knn_datasets.xlsx", 8, "B5:I5");
zynq.var_knn.luts_max   = xlsread("knn_datasets.xlsx", 8, "J5");
zynq.var_knn.accel_time = xlsread("knn_datasets.xlsx", 8, "B2:I2") / 1E6;
zynq.var_knn.arm_time   = xlsread("knn_datasets.xlsx", 8, "B3:I3");

zynq.var_training.num        = xlsread("knn_datasets.xlsx", 9, "B1:N1");
zynq.var_training.accel_time = xlsread("knn_datasets.xlsx", 9, "B2:N2") / 1E6;
zynq.var_training.arm_time   = xlsread("knn_datasets.xlsx", 9, "B3:N3");

zynq.var_features.num        = xlsread("knn_datasets.xlsx", 9, "B1:K1");
zynq.var_features.accel_time = xlsread("knn_datasets.xlsx", 9, "B2:K2") / 1E6;
zynq.var_features.arm_time   = xlsread("knn_datasets.xlsx", 9, "B3:K3");

%% calculate results
% performance variation with number of cores
results.var_accel = [accel.cores; accel.time(end, :)];

% performance improvements
results.performance = processor.time ./ accel.time;

% energy improvements
accel.energy     = accel.time * (system.static + system.dynamic);
processor.energy = processor.time * system.static;
results.energy   = processor.energy ./ accel.energy;

%% plot results
% performance variation with number of cores
figure();
stem(log2(results.var_accel(1, :)), results.var_accel(2, :), "filled");
xlim([log2(results.var_accel(1, 1)), log2(results.var_accel(1, end))]);
xticks(log2(results.var_accel(1, :)));
xticklabels({"2^0", "2^1", "2^2", "2^3", "2^4", "2^4+2^3"});
ylabel("Execution time [s]");
xlabel("Number of accelerators");
grid on;
ax = gca;
ax.XAxis.MinorTickValues = log2(0:1:24);
grid minor;
hold on;
plot(log2(results.var_accel(1, :)), results.var_accel(2, 1) ./ results.var_accel(1, :), '--');
legend("Obtained timings", "Theoretical prediction");

% generate legend for number of cores
for i=1:length(accel.cores)
    if accel.cores(i) == 1
        results.legend_cores(i) = sprintf("%d core", accel.cores(i));
    else
        results.legend_cores(i) = sprintf("%d cores", accel.cores(i));
    end
end

% accelerator execution time
figure();
bar(log2(accel.time * 16384));
legend(results.legend_cores, "Location", "northwest");
xticklabels(datasets.names);
set(gca,'XTickLabelRotation',30);
xlabel("Data Set");
ylabel("Execution time [s]");
ax = gca;
ax.YAxis.MinorTickValues = 1:1:28;
grid on;
grid minor;
ylim([0, 28]);
yticks(0:4:28);
yticklabels({"2^{-14}", "2^{-10}", "2^{-6}", "2^{-2}", "2^{2}", "2^{6}", "2^{10}", "2^{14}"});
set(gca, 'YGrid', 'on', 'XGrid', 'off', 'YMinorGrid', 'on', 'XMinorGrid', 'off');

% performance improvements
figure();
bar(results.performance);
legend(results.legend_cores, "Location", "northwest");
xticklabels(datasets.names);
set(gca,'XTickLabelRotation',30);
xlabel("Data Set");
ylabel("Speedup");
grid on;
set(gca, 'YGrid', 'on', 'XGrid', 'off', 'YMinorGrid', 'on', 'XMinorGrid', 'off');

% accelerator energy consumption
figure();
bar(log2(accel.energy(:, end) * 8192));
xticklabels(datasets.names);
set(gca,'XTickLabelRotation',30);
xlabel("Data Set");
ylabel("Total energy spent [J]");
ax = gca;
ax.YAxis.MinorTickValues = 1:1:28;
grid on;
grid minor;
ylim([0, 24]);
yticks(0:4:24);
yticklabels({"2^{-13}", "2^{-7}", "2^{-5}", "2^{-1}", "2^{3}", "2^{7}", "2^{11}"});
set(gca, 'YGrid', 'on', 'XGrid', 'off', 'YMinorGrid', 'on', 'XMinorGrid', 'off');

% energy improvements
figure();
bar(results.energy(:, end));
xticklabels(datasets.names);
set(gca,'XTickLabelRotation',30);
xlabel("Data Set");
ylabel("Energy effiency improvement");
set(gca, 'YGrid', 'on', 'XGrid', 'off', 'YMinorGrid', 'on', 'XMinorGrid', 'off');

% speedup versus k
figure();
yyaxis left;
plot(log2(zynq.var_knn.num), zynq.var_knn.arm_time ./ zynq.var_knn.accel_time, 'o-');
ylabel("Speedup");
ylim([0, 7]);
yyaxis right;
plot(log2(zynq.var_knn.num), zynq.var_knn.accel_time * 1E3, 'x-');
ylabel("Accelerator execution time [ms]");
ylim([0, 7]);
xticks(log2(zynq.var_knn.num) + 1);
xticklabels({"2^0", "2^1", "2^2", "2^3", "2^4", "2^5", "2^6", "2^7"});
grid on;
grid minor;
xlabel("K-Nearest Neighbors");

% luts versus k
figure();
bar(zynq.var_knn.luts);
text(1:length(zynq.var_knn.luts),zynq.var_knn.luts,num2str(zynq.var_knn.luts'),'vert','bottom','horiz','center');
box off;
xticklabels({"2^0", "2^1", "2^2", "2^3", "2^4", "2^5", "2^6", "2^7"});
xlabel("K-Nearest Neighbors");
ylabel("LUTs");

% speedup versus training
figure();
yyaxis left;
plot(log2(zynq.var_training.num), zynq.var_training.arm_time ./ zynq.var_training.accel_time, 'o-');
ylabel("Speedup");
ylim([1.1, 2.1]);
yticks(linspace(1.1, 2.1, 11));
yyaxis right;
plot(log2(zynq.var_training.num), zynq.var_training.accel_time * 1E3, 'x-');
ylabel("Accelerator execution time [ms]");
ylim([0, 12]);
yticks(linspace(0, 12, 11));
xticks(log2(zynq.var_training.num));
xticklabels({"2^2", "2^3", "2^4", "2^5", "2^6", "2^7", "2^8", "2^9", "2^{10}", "2^{11}", "2^{12}", "2^{13}", "2^{14}"});
grid on;
grid minor;
xlabel("Number of training samples");

% speedup versus features
figure();
yyaxis left;
plot(log2(zynq.var_features.num), zynq.var_features.arm_time ./ zynq.var_features.accel_time, 'o-');
ylabel("Speedup");
ylim([1, 2.4]);
yticks(linspace(1, 2.4, 11));
yyaxis right;
plot(log2(zynq.var_features.num), zynq.var_features.accel_time * 1E3, 'x-');
ylabel("Accelerator execution time [ms]");
ylim([0, 14E-1]);
yticks(linspace(0, 14E-1, 11));
xticks(log2(zynq.var_features.num));
xticklabels({"2^0", "2^1", "2^2", "2^3", "2^4", "2^5", "2^6", "2^7", "2^8", "2^9"});
grid on;
grid minor;
xlabel("Number of features");