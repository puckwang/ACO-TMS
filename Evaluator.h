//
// Created by 王奕翔 on 2018/1/6.
//

#ifndef ANT_TMS_EVALUATOR_H
#define ANT_TMS_EVALUATOR_H

#include <vector>

class Evaluator {
public:
    double getCost();

    void setCount(int _pCount, int _tCount);

    void setTransDataVol(double *transDataVol);

    void setRunCost(double *runCost);

    void setSsMs(int *_ss, int *_ms);

    void printSt();

    void printFt();

    void setTransDataRate(double *transDataRate);

private:
    int pCount, tCount, *ss, *ms;
    double *st, *ft, *transDataVol, *transDataRate, *runCost;

    void createStFt();

    void printArray(double *array, int r);

    // 回傳該任務可開始的時間
    double getCanStartTime(int task, int process);

    std::vector<int>::iterator  insertTask(int taskId, double canStartTime, std::vector<int> *process);
};

#endif //ANT_TMS_EVALUATOR_H
