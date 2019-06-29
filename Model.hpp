#pragma once

#include <ctime>
#include <cstdlib>

class Model;  // Модель клеточного автомата "2D-диффузия"

class Model
{
private:
    int numCells; // Количество клеток
    float** oldGrid; // Сетка старого поколения
    float** newGrid; // Сетка нового поколения

    float diffusion; // Коэффициент диффузии [м^2 / с]
    float dt; // Шаг по времени [с]
    float dx; // Шаг по сетке [м]
    float courant; // Коэффициент Куранта []

    // Коэффициенты для верхнего и нижнего граничных условий
    float alpha_1;
    float alpha_2;
    float beta_1;
    float beta_2;
    float mu_1;
    float mu_2;

    // Концентрации на левой и правой границе сетки
    float c_1;
    float c_2;

    // Концентрация внутри сетки
    float c_0;

    float curTime; // Текущее время [с]
    float end_time; // Конечное время [с]

public:
    Model(int numCells_, float diffusion_, float courant_, float length, float end_time_) // Конструктор
    {
        numCells = numCells_;
        diffusion = diffusion_;
        courant = courant_;
        dx = length / numCells;
        dt = courant_ * dx * dx / diffusion;

        curTime = 0.0;
        end_time = end_time_;

        // Динамическое выделение памяти для двухмерных массивов
        oldGrid = new float*[numCells + 2];
        newGrid = new float*[numCells + 2];
        for (int index = 0; index < numCells + 2; index++)
        {
            oldGrid[index] = new float[numCells + 2];
            newGrid[index] = new float[numCells + 2];
        };

        // Обнуляем массивы
        for (int x = 0; x < numCells + 2; x++)
            for (int y = 0; y < numCells + 2; y++)
            {
                oldGrid[x][y] = 0.0f;
                newGrid[x][y] = 0.0f;
            };

    }

    // Установить коэффициенты для верхнего и нижнего граничных условий
    void setUpDownCoeffs(float a1, float a2, float b1, float b2, float mu1, float mu2)
    {
        alpha_1 = a2;
        alpha_2 = a2;
        beta_1 = b1;
        beta_2 = b2;
        mu_1 = mu1;
        mu_2 = mu2;
    }

    // Установить концентрации на левой и правой границе сетки
    void setLeftRightCoeffs(float c1, float c2)
    {
        c_1 = c1;
        c_2 = c2;
    }

    // Задать концентрацию внутри сетки
    void setInnerCoeffs(float c0)
    {
        c_0 = c0;
    }

    void setUpDownBorder(float** grid)
    {
        #pragma omp parallel for
        for (int x = 2; x <= numCells - 1; x++)
        {
            grid[x][1] = (mu_1 - alpha_1 / dx * oldGrid[x][2]) / (beta_1 - alpha_1 / dx);
            grid[x][numCells] = (mu_2 + alpha_2 / dx * oldGrid[x][numCells - 1]) / (beta_2 + alpha_2 / dx);
        }
    }

    void setLeftRightBorder(float** grid)
    {
        #pragma omp parallel for
        for (int y = 1; y <= numCells; y++)
        {
            grid[1][y] = c_1;
            grid[numCells][y] = c_2;
        }
    }

    int calcEngine() // Алгоритм клеточного автомата "Нейронная сеть"
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
        float** temp = oldGrid;
        oldGrid = newGrid;
        newGrid = temp;

        // Увеличение текущего времени на 1 шаг по времени
        curTime += dt;

        return 0;
    }

    float** getGrid() // Получить указатель на массив клеток
    {
        return oldGrid;
    }

    int getNumCells() // Получить размер сетки
    {
        return numCells;
    }

    float getCurTime() // Получить текущее время
    {
        return curTime;
    }

    void randomInit() // Заполнить сетку случайным образом
    {
        srand((unsigned int)time(NULL)); // "сбрасываем рандом"

        for (int x = 1; x <= numCells; x++)
            for (int y = 1; y <= numCells; y++)
                oldGrid[x][y] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        // Задание концентрации на границах
        setUpDownBorder(oldGrid);
        setLeftRightBorder(oldGrid);
    }

    void userInit() // Задать пользовательское начальное распределение
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

    float checkGrid() // Проверить, что концентрация не выходит за границу области [0; 1]
    {
        for (int x = 1; x <= numCells; x++)
            for (int y = 1; y <= numCells; y++)
                if ((newGrid[x][y] < 0.0f) || (newGrid[x][y] > 1.0f))
                    return newGrid[x][y];

        return 0.0f;
    }
};