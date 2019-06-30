//
// Created by aleksandr on 30.06.19.
//

#pragma once

// Стандартные библиотеки
#include <random>
#include <vector>

using std::vector;

// Пользовательские классы
class cell;

class Road2D
{
public:
    /// Конструктор
    /// @param road_length_ - длина дороги
    /// @param lane_num_ - число полос
    /// @param vel_max - максимальная скорость машины
    /// @param cars_num_ - количество машин на дороге в начальный момент времени
    /// @param dist_ - безопасное расстояние между машинами
    Road2D(int road_length_, int lane_num_, int vel_max_, int cars_num_, int dist_);

    /// Выполнить следующий шаг по времени
    void make_next_step();

    /// Получить результат на предыдущем шаге по времени
    vector<vector<cell>> get_prev_result();

    /// Получить результат на следующем шаге по времени
    vector<vector<cell>> get_next_result();

    /// Вывести результат на экран
    void output_data();

    /// Выдать текущее время модели
    int get_cur_time();

    /// Задать период времени, через который появляются новые машины
    /// @param new_cars_time_ - период времени, через который появляются новые машины
    void set_new_cars_time(int new_cars_time_);

    /// Задать вероятность возмущения скоростей машин
    /// @param prob_ - вероятность возмущения скоростей машин
    void set_prob(float prob_);

    /// Задать вероятность смены полосы машиной
    /// @param ch_prob_ - вероятность смены полосы машиной
    void set_ch_prob(float ch_prob_);

private:
    int road_length;   // Длина дороги
    int lane_num;      // Число полос
    int vel_max;       // Максимальная скорость машины
    int cars_num;      // Количество машин на дороге в начальный момент времени
    int dist;          // Безопасное расстояние между машинами
    int cur_time;      // Текущее время модели
    int new_cars_time; // Период времени, через который появляются новые машины
    double prob;       // Вероятность возмущения скоростей машин
    double ch_prob;    // Вероятность смены полосы машиной

    vector<vector<cell>> cells;     // Ячейки на предыдущем шаге по времени
    vector<vector<cell>> new_cells; // Ячейки на следующем шаге по времени

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> coord_dist;
    std::uniform_int_distribution<> lane_dist;
    std::uniform_int_distribution<> vel_dist;
    std::uniform_real_distribution<> prob_dist;
    std::uniform_real_distribution<> ch_prob_dist;
};
