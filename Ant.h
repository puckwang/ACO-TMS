//
// Created by 王奕翔 on 2018/1/4.
//

#ifndef ANT_TMS_ANT_H
#define ANT_TMS_ANT_H

class Ant {
public:
    Ant();

    void init(int taskCount, int processCount);

private:
    int *taskSchedule, taskScheduleIndex = 0; // 排程陣列
    int *processMatch, processMatchIndex = 0; // 匹配陣列
    int *canRunTask; // 當前可執行的task
    int taskCount, processCount;

public:
    int *getCanRunTask() const;

    void selectTask(int taskId);

    void selectProcess(int processId);

    void clear();
};

#endif //ANT_TMS_ANT_H
