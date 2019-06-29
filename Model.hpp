#pragma once

#include <vector>
#include <algorithm>

using std::vector;

class Model; // Модель клеточного автомата "2D-диффузия"

class Model
{
private:
    int numCells;                  // Количество клеток
    vector<vector<float>> oldGrid; // Сетка старого поколения
    vector<vector<float>> newGrid; // Сетка нового поколения

    float diffusion; // Коэффициент диффузии [м^2 / с]
    float dt;        // Шаг по времени [с]
    float dx;        // Шаг по сетке [м]
    float courant;   // Коэффициент Куранта []

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
    // Конструктор
    Model(int numCells_, float diffusion_, float courant_, float length, float end_time_);

    // Установить коэффициенты для верхнего и нижнего граничных условий
    void setUpDownCoeffs(float a1, float a2, float b1, float b2, float mu1, float mu2);

    // Установить концентрации на левой и правой границе сетки
    void setLeftRightCoeffs(float c1, float c2);

    // Задать концентрацию внутри сетки
    void setInnerCoeffs(float c0);

    void setUpDownBorder(vector<vector<float>>& grid);

    void setLeftRightBorder(vector<vector<float>>& grid);

    // Алгоритм клеточного автомата "Нейронная сеть"
    int calcEngine();

    // Получить указатель на массив клеток
    vector<vector<float>> getGrid();

    // Получить размер сетки
    int getNumCells();

    // Получить текущее время
    float getCurTime();

    // Заполнить сетку случайным образом
    void randomInit();

    // Задать пользовательское начальное распределение
    void userInit();

    // Проверить, что концентрация не выходит за границу области [0; 1]
    float checkGrid();
};