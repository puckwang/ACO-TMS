//
// Created by 王奕翔 on 2018/1/4.
//

#include <iostream>
#include "Ant.h"
#define numMax -1

Ant::Ant() = default;

void Ant::init(int taskCount, int processCount, double *transDataVol) {
    this->taskSchedule = new int[taskCount];
    this->processMatch = new int[taskCount];
    this->doneTask = new int[taskCount];
    this->taskCount = taskCount;
    this->processCount = processCount;
    this->transDataVol = transDataVol;
}

void Ant::clear() {
    for (int i = 0; i < taskCount; ++i) {
        taskSchedule[i] = 0;
        doneTask[i] = 0;
        processMatch[i] = 0;
    }

    taskScheduleIndex = -1;
}

void Ant::selectTask(int taskId) {
    taskSchedule[++taskScheduleIndex] = taskId;
    currentTask = taskId;
    doneTask[taskId] = 1;
}

void Ant::selectProcess(int taskId, int processId) {
    processMatch[taskId] = processId;
    currentProcess = processId;
}

int Ant::getCurrentTask() {
    return currentTask;
}

int Ant::getCurrentProcess() {
    return currentProcess;
}

bool Ant::canRunTask(int taskID) {
    if (doneTask[taskID]) {
        return false;
    }

    for (int i = 0; i < taskCount; ++i) {
        if (*(transDataVol + i * taskCount + taskID) != numMax && !doneTask[i]) {
            return false;
        }
    }

    return true;
}

void Ant::printTaskSchedule() {
    std::cout << "task schedule: ";
    for (int i = 0; i < taskCount; ++i) {
        std::cout << taskSchedule[i] << " ";
    }
    std::cout << std::endl;
}

void Ant::printProcessMatch() {
    std::cout << "process match: ";
    for (int i = 0; i < taskCount; ++i) {
        std::cout << processMatch[i] << " ";
    }
    std::cout << std::endl;
}

void Ant::printDoneTask() {
    std::cout << "Done task: ";
    for (int i = 0; i < taskCount; ++i) {
        std::cout << doneTask[i] << " ";
    }
    std::cout << std::endl;
}

int *Ant::getTaskSchedule() {
    return taskSchedule;
}

int *Ant::getProcessMatch() {
    return processMatch;
}

double Ant::getFinalTime() {
    return finalTime;
}

void Ant::setFinalTime(double finalTime) {
    this->finalTime = finalTime;
}