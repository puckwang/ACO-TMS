//
// Created by 王奕翔 on 2018/1/4.
//

#ifndef ANT_TMS_ANTCOLONY_H
#define ANT_TMS_ANTCOLONY_H

#include "Ant.h"
#include "Evaluator.h"

#define initPheromones 0.9 // 初始費洛蒙
#define alpha 2 // 費洛蒙影響力控制系數
#define beta 2 // 初始費洛蒙影響力控制系數

class AntColony {
private:
    int taskCount, processCount, antCount;
    double *map; // map[taskCount][processCount]
    double *transDataVol, *transDataRate, *runCost;
    Evaluator evaluator;
    Ant *ants;

    void initMap();

    void init2DArray(double *array, int x, int y);

    void intAnt();

    void moveAnts();

    void updatePheromones();

    void evaporatePheromones();

    void getBestAnt();

    void moveAnt(Ant &ant);

    int getRandProcess(Ant &ant);

    int getRandTask(Ant &ant);

    double calculateProbability(double pheromones);

    double getPheromones(int taskID, int processID);

    double getRandom(double max);

    void evaluateAnt(Ant &ant);

public:
    AntColony(int taskCount, int processCount, int antCount, double *transDataVol, double *transDataRate, double *runCost);

    void run(int iteration);
};

#endif //ANT_TMS_ANTCOLONY_H
