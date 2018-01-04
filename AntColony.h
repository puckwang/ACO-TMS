//
// Created by 王奕翔 on 2018/1/4.
//

#ifndef ANT_TMS_ANTCOLONY_H
#define ANT_TMS_ANTCOLONY_H

#include "Ant.h"
#define initPheromones 1.0

class AntColony {
private:
    int taskCount, processCount, antCount;
    double *map;
    Ant *ant;

    void initMap();

    void init2DArray(double *array, int x, int y);

    void intAnt();

    void moveAnts();

    void updatePheromones();

    void evaporatePheromones();

    void getBestAnt();

    void moveAnt(Ant *ant);

public:
    AntColony(int taskCount, int processCount, int antCount);

    void run(int iteration);
};

#endif //ANT_TMS_ANTCOLONY_H
