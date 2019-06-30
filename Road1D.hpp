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

    /// Задать период времени, через который появляются новые машины
    /// @param new_cars_time_ - период времени, через который появляются новые машины
    void set_new_cars_time(int new_cars_time_);

private:
    int road_length;   // Длина дороги
    int vel_max;       // Максимальная скорость машины
    int cars_num;      // Количество машин на дороге в начальный момент времени
    int dist;          // Безопасное расстояние между машинами
    int cur_time;      // Текущее время модели
    int new_cars_time; // Период времени, через который появляются новые машины

    vector<cell> cells;     // Ячейки на предыдущем шаге по времени
    vector<cell> new_cells; // Ячейки на следующем шаге по времени
};
