//
// Created by 王奕翔 on 2018/1/4.
//

#include <cmath>
#include <cstdlib>
#include <random>
#include <iostream>
#include "AntColony.h"
#include <thread>

AntColony::AntColony(int taskCount, int processCount, int antCount, double *transDataVol, double *transDataRate,
                     double *runCost) {
    this->taskCount = taskCount;
    this->processCount = processCount;
    this->antCount = antCount;
    this->transDataVol = transDataVol;
    this->transDataRate = transDataRate;
    this->runCost = runCost;
    evaluator.setCount(processCount, taskCount);
    evaluator.setTransDataVol(transDataVol);
    evaluator.setTransDataRate(transDataRate);
    evaluator.setRunCost(runCost);
    initMap();
    bestTaskSchedule = new int[taskCount];
    bestProcessMatch = new int[taskCount];
}

void AntColony::initMap() {
    taskMap = new double[taskCount * taskCount];
    init2DArray(taskMap, taskCount, taskCount);
}

void AntColony::init2DArray(double *array, int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            *(array + j + i * y) = initPheromones;
        }
    }
}

void AntColony::intAnt() {
    ants = new Ant[antCount];
    for (int i = 0; i < antCount; i++) {
        ants[i].init(taskCount, processCount, transDataVol);
    }
}

void AntColony::run(int iteration) {
    for (int i = 0; i < iteration; i++) {
        intAnt();
        moveAnts();
        updatePheromones();
        if (hasFoundBest) {
            initMap();
            hasFoundBest = false;
        }
    }
}

void AntColony::moveAnts() {
    std::thread *thread[threadCount];
    for (int j = 0; j < threadCount; ++j) {
        thread[j] = new std::thread(&AntColony::moveAntsThread, this, antCount / threadCount * j, antCount / threadCount * (j + 1));
    }

    for (int k = 0; k < threadCount; ++k) {
        thread[k]->join();
    }

    for (int i = 0; i < this->antCount; ++i) {
        evaluateAnt(ants[i]);
        if (bestFinalTime > ants[i].getFinalTime()) {
            saveBestData(ants[i]);
        }
    }
}

void AntColony::moveAntsThread(int start, int end) {
    for (int i = start; i < end; ++i) {
        moveAnt(ants[i]);
    }
}

void AntColony::moveAnt(Ant &ant) {
    int selectedTask;
    ant.clear();
    for (int i = 0; i < this->taskCount; ++i) {
        if (i == 0) {
            selectedTask = 0;
        } else {
            selectedTask = getRandTask(ant);
        }
        ant.selectTask(selectedTask);
    }
}

void AntColony::updatePheromones() {
    evaporatePheromones();
    for (int i = 0; i < antCount; ++i) {
        updateAntPheromones(ants[i]);
    }
}

void AntColony::updateAntPheromones(Ant &ant) {
    double deltaPheromones = getDeltaPheromones(ant);
    for (int i = 0; i < taskCount; ++i) {
        int taskID = *(ant.getTaskSchedule() + i), orderID = i;
        // 更新 task to  process 的費洛蒙
        setTaskMapPheromones(taskID, orderID, deltaPheromones);
    }
}

void AntColony::evaporatePheromones() {
    for (int i = 0; i < taskCount; ++i) {
        for (int j = 0; j < taskCount; ++j) {
            *(taskMap + i * taskCount + j) *= (1 - evaporatePheromonesCoefficient);
            checkTaskMapPheromones(i, j);
        }
    }
}

int AntColony::getRandTask(Ant &ant) {
    double taskProbability[taskCount], taskProbabilitySum = 0.0;
    for (int i = 0; i < taskCount; ++i) {
        if (!ant.canRunTask(i)) {
            taskProbability[i] = 0.0;
            continue;
        }
        taskProbability[i] = calculateProbability(i, ant.getTaskScheduleIndex() + 1, ant);
        taskProbabilitySum += taskProbability[i];
    }
    double targetPoint = getRandom(taskProbabilitySum);
    for (int i = 0; i < taskCount; ++i) {
        targetPoint -= taskProbability[i];
        if (targetPoint <= 0.0) {
            return i;
        }
    }

    std::cout << "[ERROR] getRandTask" << std::endl;
    return 0;
}

double AntColony::calculateProbability(int taskID, int orderID, Ant &ant) {
     return pow(getTaskMapPheromones(taskID, orderID), alpha);
}

double AntColony::getTaskMapPheromones(int taskID, int orderID) {
    return *(taskMap + taskID * taskCount + orderID);
}

void AntColony::setTaskMapPheromones(int taskID, int orderID, double newValue) {
    *(taskMap + taskID * taskCount + orderID) += newValue;
    checkTaskMapPheromones(taskID, orderID);
}

double AntColony::getRandom(double fmax) {
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_real_distribution<double> dis(0.0, fmax);

    return dis(gen);
}

void AntColony::evaluateAnt(Ant &ant) {
    evaluator.setSsMs(ant.getTaskSchedule(), ant.getProcessMatch());
    ant.setFinalTime(evaluator.getCost());
}

void AntColony::saveBestData(Ant &ant) {
    bestFinalTime = ant.getFinalTime();
    for (int i = 0; i < taskCount; ++i) {
        *(bestTaskSchedule + i) = ant.getTaskSchedule()[i];
        *(bestProcessMatch + i) = ant.getProcessMatch()[i];
    }
    hasFoundBest = true;
}

void AntColony::printPheromones() {
    std::cout << "Task Map: " << std::endl;
    for (int i = 0; i < taskCount; ++i) {
        for (int j = 0; j < taskCount; ++j) {
            std::cout << *(taskMap + i * taskCount + j) << " ";
        }
        std::cout << std::endl;
    }
}

double AntColony::getBestFinalTime() {
    return bestFinalTime;
}

void AntColony::checkTaskMapPheromones(int taskID, int orderID) {
    if (*(taskMap + taskID * taskID + orderID) < MinimumPheromones) {
        *(taskMap + taskID * taskID + orderID) = MinimumPheromones;
    }
    if (*(taskMap + taskID * taskID + orderID) > MaximumPheromones) {
        *(taskMap + taskID * taskID + orderID) = MaximumPheromones;
    }
}

void AntColony::printScheduleAndMatch() {
    std::cout << "TaskSchedule: " << std::endl;
    for (int j = 0; j < taskCount; ++j) {
        std::cout << *(this->bestTaskSchedule + j) << " ";
    }
    std::cout << std::endl;
    std::cout << "ProcessMatch: " << std::endl;
    for (int j = 0; j < taskCount; ++j) {
        std::cout << *(this->bestProcessMatch + j) << " ";
    }
    std::cout << std::endl;
}

void AntColony::printStartAndFinalTime() {
    evaluator.setSsMs(this->bestTaskSchedule, this->bestProcessMatch);
    evaluator.getCost();
    std::cout << "Start Time: " << std::endl;
    evaluator.printSt();
    std::cout << "Final Time: " << std::endl;
    evaluator.printFt();
}

void AntColony::printBestFinalTime() {
    std::cout << "Best Final Time: " << getBestFinalTime() << std::endl;

}

double AntColony::getDeltaPheromones(Ant &ant) {
    return handDownPheromonesCoefficient / ant.getFinalTime();
}

void AntColony::setThreadCount(int threadCount) {
    AntColony::threadCount = threadCount;
}

