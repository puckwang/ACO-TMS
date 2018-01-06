//
// Created by 王奕翔 on 2018/1/6.
//

#include <iostream>
#include "Evaluator.h"

double Evaluator::getCost() {
    std::vector<int> process[pCount];
    double finalTime = 0;

    for (int i = 0; i < tCount; i++) {
        int taskId = *(ss + i);
        double canStartTime = getCanStartTime(taskId);
        insertTask(taskId, canStartTime, &process[*(ms + taskId)]);
        finalTime = finalTime > *(ft + taskId) ? finalTime : *(ft + taskId);
    }

    return finalTime;
}

void Evaluator::setCount(int _pCount, int _tCount) {
    pCount = _pCount;
    tCount = _tCount;
    createStFt();
}

void Evaluator::setTransDataVol(double *transDataVol) {
    this->transDataVol = transDataVol;
}

void Evaluator::setRunCost(double *runCost) {
    this->runCost = runCost;
}

void Evaluator::setSsMs(int *_ss, int *_ms) {
    ss = _ss;
    ms = _ms;
}

void Evaluator::printSt() {
    printArray(st, tCount);
}

void Evaluator::printFt() {
    printArray(ft, tCount);
}

void Evaluator::createStFt() {
    st = new double[tCount];
    ft = new double[tCount];
}

void Evaluator::printArray(double *array, int r) {
    for (int j = 0; j < r; j++) {
        std::cout << *(array + j) << " ";
    }
    std::cout << std::endl;
}

// 回傳該任務可開始的時間
double Evaluator::getCanStartTime(int task) {
    double canStartTime = 0;
    for (int i = 0; i < tCount; i++) {
        if (*(transDataVol + i * tCount + task) >= 0) {
            double endTime = ft[i];

            // 不同處理器
            if (ms[task] != ms[i]) {
                endTime += *(transDataVol + i * tCount + task) * *(transDataRate + ms[task] * pCount + ms[i]);
            }

            canStartTime = canStartTime < endTime ? endTime : canStartTime;
        }
    }
    return canStartTime;
}

void Evaluator::insertTask(int taskId, double canStartTime, std::vector<int> *process) {
    double startTime = canStartTime, costTime = *(runCost + taskId * pCount + *(ms + taskId));

    if (process->size() != 0) {
        for (auto p = process->begin(); p != process->end(); p++) {
            double now_task_ft = *(ft + *p);

            if (p+1 == process->end()) {
                startTime = now_task_ft > canStartTime ? now_task_ft : canStartTime;
                process->insert(p+1, taskId);
                st[taskId] = startTime;
                ft[taskId] = startTime + costTime;
                return;
            }

            double next_task_st = *(st + *(p+1)), next_task_ft = *(ft + *(p+1));

            if ((now_task_ft >= canStartTime && next_task_st - now_task_ft >= costTime)
                || (now_task_ft < canStartTime && next_task_ft > canStartTime
                    && next_task_st - canStartTime >= costTime)) {
                startTime = now_task_ft < canStartTime ? canStartTime : now_task_ft;
                process->insert(p+1, taskId);
                st[taskId] = startTime;
                ft[taskId] = startTime + costTime;
                return;
            }
        }
    }
    process->push_back(taskId);
    st[taskId] = startTime;
    ft[taskId] = startTime + costTime;
}

void Evaluator::setTransDataRate(double *transDataRate) {
    this->transDataRate = transDataRate;
}
