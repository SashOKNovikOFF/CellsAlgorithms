//
// Created by aleksandr on 29.06.19.
//

// Стандартные библиотеки
#include <ctime>
#include <cstdlib>

// Пользовательские библиотеки
#include "Model.hpp"

// Конструктор
Model::Model(int numCells_, float diffusion_, float courant_, float length, float end_time_) :
        numCells(numCells_),
        diffusion(diffusion_),
        courant(courant_),
        dx(length / numCells),
        dt(courant_ * length * length / numCells / numCells / diffusion),
        curTime(0.0),
        end_time(end_time_),
        oldGrid(numCells + 2, vector<float>(numCells + 2, 0.0f)),
        newGrid(numCells + 2, vector<float>(numCells + 2, 0.0f))
{

}

void Model::setUpDownCoeffs(float a1, float a2, float b1, float b2, float mu1, float mu2)
{
    alpha_1 = a2;
    alpha_2 = a2;
    beta_1 = b1;
    beta_2 = b2;
    mu_1 = mu1;
    mu_2 = mu2;
}

void Model::setLeftRightCoeffs(float c1, float c2)
{
    c_1 = c1;
    c_2 = c2;
}

void Model::setInnerCoeffs(float c0)
{
    c_0 = c0;
}

void Model::setUpDownBorder(vector<vector<float>>& grid)
{
#pragma omp parallel for
    for (int x = 2; x <= numCells - 1; x++)
    {
        grid[x][1] = (mu_1 - alpha_1 / dx * oldGrid[x][2]) / (beta_1 - alpha_1 / dx);
        grid[x][numCells] = (mu_2 + alpha_2 / dx * oldGrid[x][numCells - 1]) / (beta_2 + alpha_2 / dx);
    }
}

void Model::setLeftRightBorder(vector<vector<float>>& grid)
{
#pragma omp parallel for
    for (int y = 1; y <= numCells; y++)
    {
        grid[1][y] = c_1;
        grid[numCells][y] = c_2;
    }
}

int Model::calcEngine() // Алгоритм клеточного автомата "Нейронная сеть"
{
    // Остановить расчёт
    if (curTime >= end_time)
        return 1;

    // Уменьшить шаг по времени, если он перепрыгивает конечное время расчёта
    if ((dt + curTime) > end_time)
        dt = end_time - curTime;

    // Обход по всем клеткам старого поколения
#pragma omp parallel for
    for (int x = 1; x <= numCells; x++)
        for (int y = 1; y <= numCells; y++)
        {
            float diff_2_dx = oldGrid[x - 1][y] - 2.0f * oldGrid[x][y] + oldGrid[x + 1][y];
            float diff_2_dy = oldGrid[x][y - 1] - 2.0f * oldGrid[x][y] + oldGrid[x][y + 1];
            newGrid[x][y] = oldGrid[x][y] + courant * (diff_2_dx + diff_2_dy);

            // Если концентрация превышает значения в пределе [0; 1], ограничиваем её
//                newGrid[x][y] = (newGrid[x][y] > 1.0f) ? 1.0f : newGrid[x][y];
//                newGrid[x][y] = (newGrid[x][y] < 0.0f) ? 0.0f : newGrid[x][y];
        };

    // Задание концентрации на границах
    setUpDownBorder(newGrid);
    setLeftRightBorder(newGrid);

    // Смена старого и нового поколений
    std::swap(oldGrid, newGrid);

    // Увеличение текущего времени на 1 шаг по времени
    curTime += dt;

    return 0;
}

vector<vector<float>> Model::getGrid()
{
    return oldGrid;
}

int Model::getNumCells()
{
    return numCells;
}

float Model::getCurTime()
{
    return curTime;
}

void Model::randomInit()
{
    srand((unsigned int)time(NULL)); // "сбрасываем рандом"

    for (int x = 1; x <= numCells; x++)
        for (int y = 1; y <= numCells; y++)
            oldGrid[x][y] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    // Задание концентрации на границах
    setUpDownBorder(oldGrid);
    setLeftRightBorder(oldGrid);
}

void Model::userInit()
{
    // Задание единой концетрации внутри сетки
#pragma omp parallel for
    for (int x = 1; x <= numCells; x++)
        for (int y = 1; y <= numCells; y++)
            oldGrid[x][y] = c_0;

    // Задание концентрации на границах
    setUpDownBorder(oldGrid);
    setLeftRightBorder(oldGrid);
}

float Model::checkGrid()
{
    for (int x = 1; x <= numCells; x++)
        for (int y = 1; y <= numCells; y++)
            if ((newGrid[x][y] < 0.0f) || (newGrid[x][y] > 1.0f))
                return newGrid[x][y];

    return 0.0f;
}
