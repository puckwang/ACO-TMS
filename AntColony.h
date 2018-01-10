//
// Created by 王奕翔 on 2018/1/4.
//

#ifndef ANT_TMS_ANTCOLONY_H
#define ANT_TMS_ANTCOLONY_H

#include "Ant.h"
#include "Evaluator.h"

#define initPheromones 0.9 // 初始費洛蒙  0~1
#define evaporatePheromonesCoefficient 0.5 // 費洛蒙蒸發係數 0~1
#define handDownPheromonesCoefficient 5 // 費洛蒙遺留係數
#define MaximumPheromones 1
#define MinimumPheromones 0.001
#define alpha 2 // 費洛蒙影響力控制系數
#define beta 4 // 可視度影響力控制系數

class AntColony {
private:
    int taskCount, processCount, antCount, *bestTaskSchedule, *bestProcessMatch;
    int threadCount;
    int *taskWaitCount;
    double bestFinalTime = 999999999;
    double *taskMap; // taskMap[taskCount][processCount]
    double *transDataVol, *transDataRate, *runCost;
    Ant *ants;
    bool hasFoundBest = false;
    std::mutex gMutex;

    void initMap();

    void init2DArray(double *array, int x, int y);

    void intAnt();

    void moveAnts();

    void updatePheromones();

    void moveAnt(Ant &ant);

    int getRandTask(Ant &ant);

    double calculateProbability(int taskID, int orderID, Ant &ant);

    double getTaskMapPheromones(int taskID, int orderID);

    void setTaskMapPheromones(int taskID, int orderID, double newValue);

    double getRandom(double max);

    void evaluateAnt(Ant &ant);

    void saveBestData(Ant &ant);

    void checkTaskMapPheromones(int taskID, int orderID);

    /**
     * 蒸發費洛蒙
     */
    void evaporatePheromones();

    /**
     * 更新單一隻螞蟻路徑的費洛蒙
     * @param ant
     */
    void updateAntPheromones(Ant &ant);

    void moveAntsThread(int start, int end);

    double getDeltaPheromones(Ant &ant);

public:
    AntColony(int taskCount, int processCount, int antCount, double *transDataVol, double *transDataRate, double *runCost, int *taskWaitCount);

    void run(int iteration);

    void printPheromones();

    void printScheduleAndMatch();

    void printStartAndFinalTime();

    void printBestFinalTime();

    double getBestFinalTime();

    void setThreadCount(int threadCount);
};

#endif //ANT_TMS_ANTCOLONY_H
