//
// Created by 王奕翔 on 2018/1/6.
//

#include <iostream>
#include "Evaluator.h"

double Evaluator::getCost() {
    std::vector<int> process[pCount];
    double allFinalTime = 0;

    for (int i = 0; i < tCount; i++) {
        int taskId = *(ss + i);
        double optimalFinalTime = 2147483646, optimalStartTime;
        int optimalProcess;
        std::vector<int>::iterator optimalInsertPosition;
        for (int j = 0; j < pCount; j++) {
            double canStartTime = getCanStartTime(taskId, j);
            std::vector<int>::iterator currentInsertPosition = insertTask(taskId, canStartTime, &process[j]);
            if (ft[taskId] < optimalFinalTime) {
                optimalProcess = j;
                optimalFinalTime = ft[taskId];
                optimalStartTime = st[taskId];
                optimalInsertPosition = currentInsertPosition;
            }
        }
        ft[taskId] = optimalFinalTime;
        st[taskId] = optimalStartTime;
        ms[taskId] = optimalProcess;
        if (process[optimalProcess].size() == 0) {
            process[optimalProcess].push_back(taskId);
        } else {
            process[optimalProcess].insert(optimalInsertPosition, taskId);
        }

        allFinalTime = allFinalTime > *(ft + taskId) ? allFinalTime : *(ft + taskId);
    }

    return allFinalTime;
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
    ms = new int[tCount];
}

void Evaluator::printArray(double *array, int r) {
    for (int j = 0; j < r; j++) {
        std::cout << *(array + j) << " ";
    }
    std::cout << std::endl;
}

// 回傳該任務可開始的時間
double Evaluator::getCanStartTime(int task, int process) {
    double canStartTime = 0;
    for (int i = 0; i < tCount; i++) {
        if (*(transDataVol + i * tCount + task) >= 0) {
            double endTime = ft[i];

            // 不同處理器
            if (process != ms[i]) {
                endTime += *(transDataVol + i * tCount + task) * *(transDataRate + process * pCount + ms[i]);
            }

            canStartTime = canStartTime < endTime ? endTime : canStartTime;
        }
    }
    return canStartTime;
}

std::vector<int>::iterator Evaluator::insertTask(int task, double canStartTime, std::vector<int> *process) {
    double startTime = canStartTime, costTime = *(runCost + task * pCount + *(ms + task));

    if (process->size() != 0) {
        for (std::vector<int>::iterator p = process->begin(); p != process->end(); p++) {
            double now_task_ft = *(ft + *p);
            if (p+1 == process->end()) {
                startTime = now_task_ft > canStartTime ? now_task_ft : canStartTime;
                st[task] = startTime;
                ft[task] = startTime + costTime;
                return p+1;
            }

            double next_task_st = *(st + *(p+1)), next_task_ft = *(ft + *(p+1));
            if ((now_task_ft >= canStartTime && next_task_st - now_task_ft >= costTime)
                || (now_task_ft < canStartTime && next_task_ft > canStartTime
                    && next_task_st - canStartTime >= costTime)) {
                startTime = now_task_ft < canStartTime ? canStartTime : now_task_ft;
                st[task] = startTime;
                ft[task] = startTime + costTime;
                return p+1;
            }
        }
    }
    st[task] = startTime;
    ft[task] = startTime + costTime;
    return process->end();
}

void Evaluator::setTransDataRate(double *transDataRate) {
    this->transDataRate = transDataRate;
}
