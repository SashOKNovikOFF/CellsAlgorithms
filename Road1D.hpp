//
// Created by aleksandr on 30.06.19.
//

#pragma once

// Стандартные библиотеки
#include <vector>

using std::vector;

struct cell
{
    bool is_car;
    int velocity;
};

class Road1D
{
public:
    /// Конструктор
    /// @param road_length_ - длина дороги
    /// @param vel_max - максимальная скорость машины
    /// @param cars_num_ - количество машин на дороге в начальный момент времени
    /// @param dist_ - безопасное расстояние между машинами
    Road1D(int road_length_, int vel_max_, int cars_num_, int dist_);

    /// Выполнить следующий шаг по времени
    void make_next_step();

    /// Получить результат на предыдущем шаге по времени
    vector<cell> get_prev_result();

    /// Получить результат на следующем шаге по времени
    vector<cell> get_next_result();

    /// Вывести результат на экран
    void output_data();

    /// Выдать текущее время модели
    int get_cur_time();

private:
    int road_length;
    int vel_max;
    int cars_num;
    int dist;
    int cur_time;

    vector<cell> cells;
    vector<cell> new_cells;
};
