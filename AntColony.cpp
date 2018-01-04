//
// Created by 王奕翔 on 2018/1/4.
//

#include "AntColony.h"

AntColony::AntColony(int taskCount, int processCount, int antCount) {
    this->taskCount = taskCount;
    this->processCount = processCount;
    this->antCount = antCount;
    initMap();
}

void AntColony::initMap() {
    map = new double[taskCount * processCount];
    init2DArray(map, taskCount, processCount);
}

void AntColony::init2DArray(double *array, int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            *(array + j + i * y) = initPheromones;
        }
    }
}

void AntColony::intAnt() {
    ant = new Ant[this->antCount];
    for (int i = 0; i < this->antCount; i++) {
        ant[i].init(this->taskCount, this->processCount);
    }
}

void AntColony::run(int iteration) {
    for (int i = 0; i < iteration; i++) {
        moveAnts();
        getBestAnt();
        updatePheromones();
        evaporatePheromones();
    }
}

void AntColony::moveAnts() {
    for (int i = 0; i < this->antCount; ++i) {
        moveAnt(&ant[i]);
    }
}

void AntColony::moveAnt(Ant *ant) {
    for (int i = 0; i < this->taskCount; ++i) {

    }
}

void AntColony::getBestAnt() {

}

void AntColony::updatePheromones() {

}

void AntColony::evaporatePheromones() {

}

