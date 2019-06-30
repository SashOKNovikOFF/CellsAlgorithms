//
// Created by aleksandr on 30.06.19.
//

// Стандартные библиотеки
#include <iostream>
#include <random>

using std::cout;
using std::endl;

// Пользовательские библиотеки
#include "Road1D.hpp"

Road1D::Road1D(int road_length_, int vel_max_, int cars_num_, int dist_) :
    road_length(road_length_),
    vel_max(vel_max_),
    cars_num(cars_num_),
    dist(dist_),
    cells(road_length_),
    new_cells(road_length_),
    cur_time(1)
{
    // Задать начальные данные по машинам
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> coord_dist(0, road_length);
    std::uniform_int_distribution<> vel_dist(1, vel_max);

    for (int car_ind = 0; car_ind < cars_num; ++car_ind)
    {
        const int coord = coord_dist(gen);
        cells[coord].is_car = true;
        cells[coord].velocity = vel_dist(gen);
    }
}

void Road1D::make_next_step()
{
    if (cur_time != 1)
        cells = new_cells;

    // Задать вектор ячеек на следующем шаге по времени
    for (auto& elem : new_cells)
    {
        elem.is_car = false;
        elem.velocity = 0;
    }

    for (int coord = 0; coord < road_length; ++coord)
        if (cells[coord].is_car)
        {
            int next_velocity = 0; // Вычисляемая скорость на следующий шаг по времени

            // 1. Ускорение
            next_velocity = std::min(cells[coord].velocity + 1, vel_max);

            // 2. Торможение
            bool next_found = false;
            int beetween_dist = 0;

            for (int i = coord + 1; i < road_length; ++i)
                if (cells[i].is_car)
                {
                    next_found = true;
                    beetween_dist = i - coord;
                    break;
                }
            if (next_found)
                next_velocity = std::min(next_velocity, beetween_dist - dist);

            // 3. Случайные возмущения

            // 4. Движение
            int next_coord = coord + next_velocity;
            if (next_coord >= road_length)
            {
                new_cells[coord].is_car = false;
                new_cells[coord].velocity = 0;
            } else
            {
                new_cells[next_coord].is_car = true;
                new_cells[next_coord].velocity = next_velocity;
            }
        }

    // Переход на следующий шаг по времени
    ++cur_time;
}

vector<cell> Road1D::get_prev_result()
{
    return cells;
}

vector<cell> Road1D::get_next_result()
{
    return new_cells;
}

void Road1D::output_data()
{
    cout << "--------------" << endl;
    for (auto& elem : new_cells)
        cout << elem.is_car << " " << elem.velocity << endl;
    cout << "--------------" << endl;
}

int Road1D::get_cur_time()
{
    return cur_time;
}