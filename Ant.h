//
// Created by 王奕翔 on 2018/1/4.
//

#ifndef ANT_TMS_ANT_H
#define ANT_TMS_ANT_H

class Ant {
public:
    Ant();

    void init(int taskCount, int processCount, double *transDataVol);

    void selectTask(int taskId);

    void selectProcess(int taskId, int processId);

    void clear();

    int getCurrentProcess();

    int getCurrentTask();

    bool canRunTask(int taskID);

    void printTaskSchedule();

    void printProcessMatch();

    void printDoneTask();

private:
    int *taskSchedule, taskScheduleIndex = -1; // 排程陣列
    int *processMatch; // 匹配陣列
    int *doneTask;
    int taskCount, processCount;
    int currentTask = 0, currentProcess = 0;
    double *transDataVol;
};

#endif //ANT_TMS_ANT_H
