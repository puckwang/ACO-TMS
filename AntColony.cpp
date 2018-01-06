//
// Created by 王奕翔 on 2018/1/4.
//

#include <cmath>
#include <cstdlib>
#include <random>
#include <iostream>
#include "AntColony.h"

AntColony::AntColony(int taskCount, int processCount, int antCount, double *transDataVol, double *transDataRate, double *runCost) {
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
    taskMap = new double[taskCount * processCount];
    processMap = new double[processCount * taskCount];
    init2DArray(taskMap, taskCount, processCount);
    init2DArray(processMap, processCount, taskCount);
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
        std::cout << "# " <<  i << std::endl;
        intAnt();
        moveAnts();
        updatePheromones();
    }
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

void AntColony::moveAnts() {
    bool bestChanged = false;
    for (int i = 0; i < this->antCount; ++i) {
        moveAnt(ants[i]);
        evaluateAnt(ants[i]);
        if (bestFinalTime > ants[i].getFinalTime()) {
            saveBestData(ants[i]);
            bestChanged = true;
        }
    }
    std::cout << "Best Final Time: " << bestFinalTime << std::endl;
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
        ant.selectProcess(selectedTask, getRandProcess(ant));
    }
}

void AntColony::updatePheromones() {
    evaporatePheromones();
    for (int i = 0; i < antCount; ++i) {
        updateAntPheromones(ants[i]);
    }
}

void AntColony::updateAntPheromones(Ant &ant) {
    double deltaPheromones = handDownPheromonesCoefficient / ant.getFinalTime();
    for (int i = 0; i < taskCount; ++i) {
        int taskID = *(ant.getTaskSchedule() + i), processID = *(ant.getProcessMatch() + i);
        if (i != 0) {
            // 更新 process to task 的費洛蒙
            setProcessMapPheromones(processID, taskID, deltaPheromones);
        }
        // 更新 task to  process 的費洛蒙
        setTaskMapPheromones(taskID, processID, deltaPheromones);
    }
}

void AntColony::evaporatePheromones() {
    for (int i = 0; i < taskCount; ++i) {
        for (int j = 0; j < processCount; ++j) {
            *(taskMap + i * processCount + j) *= (1 - evaporatePheromonesCoefficient);
            *(processMap + j * taskCount + i) *= (1 - evaporatePheromonesCoefficient);
            checkProcessMapPheromones(j, i);
            checkTaskMapPheromones(i, j);
        }
    }
}

int AntColony::getRandProcess(Ant &ant) {
    double processProbability[processCount], processProbabilitySum = 0.0;

    for (int i = 0; i < processCount; ++i) {
        processProbability[i] = calculateProbability(getProcessMapPheromones(i, ant.getCurrentTask()));
        processProbabilitySum += processProbability[i];
    }

    double targetPoint = getRandom(processProbabilitySum);

    for (int i = 0; i < taskCount; ++i) {
        targetPoint -= processProbability[i];
        if (targetPoint <= 0.0) {
            return i;
        }
    }

    std::cout << "[ERROR] getRandProcess" << std::endl;
    return 0;
}

int AntColony::getRandTask(Ant &ant) {
    double taskProbability[taskCount], taskProbabilitySum = 0.0;
    for (int i = 0; i < taskCount; ++i) {
        if (!ant.canRunTask(i)) {
            taskProbability[i] = 0.0;
            continue;
        }
        taskProbability[i] = calculateProbability(getTaskMapPheromones(i, ant.getCurrentProcess()));
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

double AntColony::calculateProbability(double pheromones) {
    return pow(pheromones, alpha);
}

double AntColony::getTaskMapPheromones(int taskID, int processID) {
    return *(taskMap + taskID * processCount + processID);
}

double AntColony::getProcessMapPheromones(int processID, int taskID) {
    return *(processMap + processID * taskCount + taskID);
}

void AntColony::setTaskMapPheromones(int taskID, int processID, double newValue) {
    *(taskMap + taskID * processCount + processID) += newValue;
    checkTaskMapPheromones(taskID, processID);
}

void AntColony::setProcessMapPheromones(int processID, int taskID, double newValue) {
    *(processMap + processID * taskCount + taskID) += newValue;
    checkProcessMapPheromones(processID, taskID);
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
}

void AntColony::printPheromones() {
    std::cout << "Task Map: " << std::endl;
    for (int i = 0; i < taskCount; ++i) {
        for (int j = 0; j < processCount; ++j) {
            std::cout << *(taskMap + i * processCount + j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Process Map: " << std::endl;
    for (int i = 0; i < processCount; ++i) {
        for (int j = 0; j < taskCount; ++j) {
            std::cout << *(processMap + i * taskCount + j) << " ";
        }
        std::cout << std::endl;
    }
}

double AntColony::getBestFinalTime() {
    return bestFinalTime;
}

void AntColony::checkTaskMapPheromones(int taskID, int processID) {
    if (*(taskMap + taskID * processCount + processID) < MinimumPheromones) {
        *(taskMap + taskID * processCount + processID) = MinimumPheromones;
    }
    if (*(taskMap + taskID * processCount + processID) > MaximumPheromones) {
        *(taskMap + taskID * processCount + processID) = MaximumPheromones;
    }
}

void AntColony::checkProcessMapPheromones(int processID, int taskID) {
    if (*(processMap + processID * taskCount + taskID) < MinimumPheromones) {
        *(processMap + processID * taskCount + taskID) = MinimumPheromones;
    }
    if (*(processMap + processID * taskCount + taskID) > MaximumPheromones) {
        *(processMap + processID * taskCount + taskID) = MaximumPheromones;
    }
}

