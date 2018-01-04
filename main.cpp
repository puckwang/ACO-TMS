#include <iostream>
#include <fstream>
#include "AntColony.h"
#define numMax 99999
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

int main(int argc, char *argv[]) {
    ifstream inputFile(argv[1]);
    ofstream outputFile("output.txt");

    int pCount; // 處理器數量
    int tCount; // 任務數量
    int eCount; // 有向邊數量

    runNotData(inputFile);
    inputFile >> pCount >> tCount >> eCount;

    double commRate[pCount][pCount]; // 處理器間的傳送單位成本
    double compCost[tCount][pCount]; // 處理器執行各任務的時間
    double transDataVol[tCount][tCount];  // 處理器間傳送的資料單位量

    // 初始化資料
    init2DArray(*commRate, pCount, pCount);
    init2DArray(*compCost, tCount, pCount);
    init2DArray(*transDataVol, tCount, tCount);

    // 匯入資料
    importData(inputFile, *commRate, pCount, pCount);
    importData(inputFile, *compCost, tCount, pCount);
    import2DData(inputFile, *transDataVol, eCount, tCount);

    inputFile.close();
    outputFile.close();
    return 0;
}
