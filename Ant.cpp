//
// Created by 王奕翔 on 2018/1/4.
//

#include "Ant.h"

Ant::Ant() = default;

void Ant::init(int taskCount, int processCount) {
    this->taskSchedule = new int[taskCount];
    this->processMatch = new int[processCount];
    this->canRunTask = new int[taskCount];
    this->canRunTask[0] = 1; // Start: 0
    this->taskCount = taskCount;
    this->processCount = processCount;
}

void Ant::clear() {
    for (int i = 0; i < taskCount; ++i) {
        taskSchedule[i] = 0;
        canRunTask[i] = 0;
    }

    for (int i = 0; i < processCount; ++i) {
        processMatch[i] = 0;
    }
    processMatchIndex = 0;
    taskScheduleIndex = 0;
}

int* Ant::getCanRunTask() const {
    return canRunTask;
}

void Ant::selectTask(int taskId) {
    taskSchedule[taskScheduleIndex++] = taskId;
}

void Ant::selectProcess(int processId) {
    processMatch[processMatchIndex++] = processId;
}
