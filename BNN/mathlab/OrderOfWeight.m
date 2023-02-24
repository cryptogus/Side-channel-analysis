clc;
close all;
clear all;

start_tr = 2;
% File absolute path
res_path = '.';

START_POINT = 1;

END_POINT = 1120;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Right First Weight
first_weight_str = '27 2E 36 4B 55 66 8E D2';
% Right Second Weight
%second_weight_str = 'D2 14 8D 9A';

START_BYTE = 1;
%  Number of weight
RESULT_NO = 8;
%RESULT_NO = 4;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

subplot(1,1,1);

KEY = sscanf(first_weight_str, '%x', [START_BYTE (START_BYTE + RESULT_NO - 1)]);
%KEY = sscanf(second_weight_str, '%x', [START_BYTE (START_BYTE + RESULT_NO - 1)]);

for k = START_BYTE:(START_BYTE + RESULT_NO - 1)
    path=strcat(res_path, '\\mathlab\\first\\%03d(0x%02x).txt');
    %path=strcat(res_path, '\\mathlab\\second\\%03d(0x%02x).txt');
    file = sprintf(path, KEY(k), KEY(k));
    data_temp = load(file);
   
  all_data(:,k) = data_temp(:);
end

hold on;

xlim([1 END_POINT-START_POINT+1])
for k = START_BYTE:(START_BYTE + RESULT_NO - 1)
    plot(all_data(:,k),'-','LineWidth',1);
end

title('First Order CPA', 'fontsize',16,'fontweight','bold','fontname','Times New Roman');
set(gca,'fontname','Times New Roman');
xlabel('Time','fontsize',16,'fontweight','bold','fontname','Times New Roman');
ylabel('Absolute correlation coefficient','fontsize',16,'fontweight','bold','fontname','Times New Roman');