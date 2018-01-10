#include <iostream>
#include <fstream>
#include <cmath>
#include "AntColony.h"
#define numMax -1
using namespace std;

void init2DArray(double *array, int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            *(array + j + i * y) = numMax;
        }
    }
}

void runNotData(ifstream &in) {
    char ch_1, ch_2;
    string ch_3;
    do {
        in >> ch_1 >> ch_2;
        getline(in, ch_3);
    } while (ch_1 != '*' && ch_2 != '/');
}

void importData(ifstream &in, double *array, int x, int y) {
    runNotData(in);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            in >> *(array + j + i * y);
        }
    }
}

void import2DData(ifstream &in, double *array, int x, int y) {
    runNotData(in);
    int tmp_i, tmp_j;
    for (int i = 0; i < x; i++) {
        in >> tmp_i >> tmp_j;
        in >> *(array + tmp_j + tmp_i * y);
    }
}

double diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp.tv_sec + (double) temp.tv_nsec / 1000000000.0;;
}

int main(int argc, char *argv[]) {
    ifstream inputFile(argv[1]);
    ofstream outputFile("output.txt");

    int pCount; // 處理器數量
    int tCount; // 任務數量
    int eCount; // 有向邊數量

    runNotData(inputFile);
    inputFile >> pCount >> tCount >> eCount;

    double transDataRate[pCount][pCount]; // 處理器間的傳送單位成本
    double runCost[tCount][pCount]; // 處理器執行各任務的時間
    double transDataVol[tCount][tCount];  // 處理器間傳送的資料單位量
    int taskWaitCount[tCount];

    // 初始化資料
    init2DArray(*transDataRate, pCount, pCount);
    init2DArray(*runCost, tCount, pCount);
    init2DArray(*transDataVol, tCount, tCount);
    for (int i = 0; i < tCount; ++i) {
        taskWaitCount[i] = 0;
    }

    // 匯入資料
    importData(inputFile, *transDataRate, pCount, pCount);
    importData(inputFile, *runCost, tCount, pCount);
    import2DData(inputFile, *transDataVol, eCount, tCount);

    for (int i = 0; i < tCount; ++i) {
        for (int j = 0; j < tCount; ++j) {
            if (transDataVol[i][j] != -1) {
                taskWaitCount[i]++;
            }
        }
    }

    struct timespec start, end;
    double avgTime = 0, bestCase = 99999, worstCase = 0, avgCase = 0, cases[5], sd = 0;
    for (int i = 0; i < 5; ++i) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        AntColony antColony(tCount, pCount, tCount, *transDataVol, *transDataRate, *runCost, taskWaitCount);
        antColony.setThreadCount(5);
        antColony.run(200);
        clock_gettime(CLOCK_MONOTONIC, &end);
        avgTime += diff(start, end);
        cases[i] = antColony.getBestFinalTime();
        avgCase += cases[i];
        if (cases[i] < bestCase) {
            bestCase = cases[i];
        }
        if (cases[i] > worstCase) {
            worstCase = cases[i];
        }
    }
    avgCase /= 5;
    avgTime /= 5;
    for (int j = 0; j < 5; ++j) {
        sd += pow(cases[j]-avgCase, 2);
    }
    sd = sqrt(sd / 5);

    cout << "Best: " << bestCase << endl;
    cout << "Worst: " << worstCase << endl;
    cout << "Avg. : " << avgCase << endl;
    cout << "SD: " << sd << endl;
    cout << "Avg. time: " << avgTime << endl;
    inputFile.close();
    outputFile.close();
    return 0;
}
