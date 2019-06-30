//
// Created by aleksandr on 30.06.19.
//

// Стандартные библиотеки
#include <iostream>

using std::cout;
using std::endl;

// Пользовательские библиотеки
#include "Road1D.hpp"
#include "Road2D.hpp"

Road2D::Road2D(int road_length_, int lane_num_, int vel_max_, int cars_num_, int dist_) :
    road_length(road_length_),
    lane_num(lane_num_),
    vel_max(vel_max_),
    cars_num(cars_num_),
    dist(dist_),
    cur_time(1),
    new_cars_time(-1),
    prob(-1.0),
    ch_prob(-1.0),
    cells(lane_num, vector<cell>(road_length_)),
    new_cells(lane_num, vector<cell>(road_length_)),
    gen(rd()),
    coord_dist(0, road_length - 1),
    lane_dist(0, lane_num - 1),
    vel_dist(1, vel_max - 1),
    prob_dist(0.0, 1.0)
{
    // Задать начальные данные по машинам
    for (int car_ind = 0; car_ind < cars_num; ++car_ind)
    {
        const int coord = coord_dist(gen);
        const int lane = lane_dist(gen);
        cells[lane][coord].is_car = true;
        cells[lane][coord].velocity = vel_dist(gen);
    }
}

void Road2D::make_next_step()
{
    if (cur_time != 1)
        cells = new_cells;

    const int rand_lane = lane_dist(gen);
    if ((new_cars_time != -1) && !cells[rand_lane][0].is_car && ((cur_time % new_cars_time) == 0))
    {
        cells[rand_lane][0].is_car = true;
        cells[rand_lane][0].velocity = 1;
    }

    for (auto& lane : new_cells)
        for (auto& elem : lane)
        {
            elem.is_car = false;
            elem.velocity = 0;
        }

    for (int lane = 0; lane < lane_num; ++lane)
    {
        for (int coord = 0; coord < road_length; ++coord)
            if (cells[lane][coord].is_car)
            {
                int next_velocity = 0; // Вычисляемая скорость на следующий шаг по времени

                // 1. Ускорение
                next_velocity = std::min(cells[lane][coord].velocity + 1, vel_max);

                // 2. Торможение
                bool next_found = false;
                int between_dist = 0;

                for (int i = coord + 1; i < road_length; ++i)
                    if (cells[lane][i].is_car)
                    {
                        next_found = true;
                        between_dist = i - coord;
                        break;
                    }
                if (next_found)
                    next_velocity = std::min(next_velocity, between_dist - dist);

                // 3. Случайные возмущения
                if (prob > 0.0 && prob_dist(gen) > prob)
                    next_velocity = std::max(next_velocity - 1, 0);

                // 4. Движение
                bool is_changed_lane = false;
                if (ch_prob > 0.0 && prob_dist(gen) > ch_prob)
                {
                    const int new_lane = (cur_time % 2 == 0) ? lane - 1 : lane + 1;
                    if (new_lane >= 0 && new_lane < lane_num && !cells[new_lane][coord].is_car)
                    {
                        is_changed_lane = true;
                        new_cells[new_lane][coord].is_car = true;
                        new_cells[new_lane][coord].velocity = next_velocity;
                    }
                }

                if (!is_changed_lane)
                {
                    int next_coord = coord + next_velocity;
                    if (next_coord >= road_length)
                    {
                        new_cells[lane][coord].is_car = false;
                        new_cells[lane][coord].velocity = 0;
                    } else
                    {
                        new_cells[lane][next_coord].is_car = true;
                        new_cells[lane][next_coord].velocity = next_velocity;
                    }
                }
            }
    }

    // Переход на следующий шаг по времени
    ++cur_time;
}

vector<vector<cell>> Road2D::get_prev_result()
{
    return cells;
}

vector<vector<cell>> Road2D::get_next_result()
{
    return new_cells;
}

void Road2D::output_data()
{
    cout << "--------------" << endl;
    for (int lane = 0; lane < lane_num; ++lane)
        for (auto& elem : new_cells[lane])
            cout << lane << " " << elem.is_car << " " << elem.velocity << endl;
    cout << "--------------" << endl;
}

int Road2D::get_cur_time()
{
    return cur_time;
}

void Road2D::set_new_cars_time(int new_cars_time_)
{
    new_cars_time = new_cars_time_;
}

void Road2D::set_prob(float prob_)
{
    prob = prob_;
}

void Road2D::set_ch_prob(float ch_prob_)
{
    ch_prob = ch_prob_;
}