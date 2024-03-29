// Стандартные библиотеки
#include <iostream>

// Пользовательские библиотеки
#include <SFML/Graphics.hpp>
#include "Model.hpp"
#include "Road1D.hpp"
#include "Road2D.hpp"

void call_Road1D_model();
void call_Road2D_model();
void view_Road1D_model(Road1D &model, int numCells, int end_time);
void view_Road2D_model(Road2D &model, int numCells, int end_time);

// Данные по отображению на экране
const int win_length = 1200; // Размер квадратного окна
const int win_height = 300;  // Размер квадратного окна
const float offset = 16.0f;  // Отступ сетки в окне
const float recSize = 20.0f; // Размер ячеек
const float sleep_t = 0.5;   // Время задержки картинки

// Данные по модели
const int road_length = 50;
const int lane_num = 5;
const int vel_max = 3;
const int cars_num = 15;
const int dist = 1;
const int end_time = 50;
const int new_cars_time = 5;
const double prob = 0.3f;
const double ch_prob = 0.5f;

int main()
{
    call_Road2D_model();

    return 0;
}

void call_Road1D_model()
{
    Road1D model(road_length, vel_max, cars_num, dist);
    model.set_new_cars_time(new_cars_time);
    model.set_prob(prob);

    int numCells = road_length;                   // Связь размера сетки модели с "вьювером"
    view_Road1D_model(model, numCells, end_time); // Вывод модели на экран

}


void call_Road2D_model()
{
    Road2D model(road_length, lane_num, vel_max, cars_num, dist);
    //model.set_new_cars_time(new_cars_time);
    //model.set_prob(prob);
    model.set_ch_prob(ch_prob);

    int numCells = road_length;                   // Связь размера сетки модели с "вьювером"
    view_Road2D_model(model, numCells, end_time); // Вывод модели на экран

}

void view_Road1D_model(Road1D &model, int numCells, int end_time)
{
    // Инициализация окна
    sf::RenderWindow window(sf::VideoMode(win_length, win_height), L"Клеточный автомат \"1D-мерный траспортный поток\"");

    // Инициализация "дороги"
    vector<cell> grid = model.get_prev_result();

    // Пока окно открыто, выводим сетку на экран
    while (window.isOpen())
    {
        // Вывод данных в консоль
        std::cout << "Current time: " << model.get_cur_time() << std::endl;

        // Инициализация события
        sf::Event event;
        while (window.pollEvent(event)) // Обработчик события "окно закрыто"
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black); // Устанавливаем окна чёрного цвета

        // Отрисовываем ненулевые ячейки сетки
        for (int x = 0; x < numCells; x++)
        {
            sf::RectangleShape rec(sf::Vector2f(recSize, recSize));
            rec.setFillColor(sf::Color::Blue);
            rec.setOrigin(sf::Vector2f(-offset, -offset));
            rec.setPosition(sf::Vector2f(x * recSize, 0 * recSize));
            window.draw(rec);

            if (grid[x].is_car)
            {
                sf::RectangleShape rec(sf::Vector2f(recSize, recSize));
                rec.setFillColor(sf::Color::Red);
                rec.setOrigin(sf::Vector2f(-offset, -offset));
                rec.setPosition(sf::Vector2f(x * recSize, 0 * recSize));
                window.draw(rec);
            }
        }

        window.display(); // Выводим сетку на экран
        sf::sleep(sf::seconds(sleep_t));

        model.make_next_step(); // Сменяем "поколение"
        grid = model.get_next_result();

        // При окончании расчёта закончить работу программы
        if (model.get_cur_time() == end_time)
        {
            std::cout << "Calculation stopped." << std::endl;
            break;
        }
    }
}

void view_Road2D_model(Road2D &model, int numCells, int end_time)
{
    // Инициализация окна
    sf::RenderWindow window(sf::VideoMode(win_length, win_height), L"Клеточный автомат \"2D-мерный траспортный поток\"");

    // Инициализация "дороги"
    vector<vector<cell>> grid = model.get_prev_result();

    // Пока окно открыто, выводим сетку на экран
    while (window.isOpen())
    {
        // Вывод данных в консоль
        std::cout << "Current time: " << model.get_cur_time() << std::endl;

        // Инициализация события
        sf::Event event;
        while (window.pollEvent(event)) // Обработчик события "окно закрыто"
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black); // Устанавливаем окна чёрного цвета

        // Отрисовываем ненулевые ячейки сетки
        for (int x = 0; x < numCells; x++)
            for (int y = 0; y < lane_num; y++)
            {
                sf::RectangleShape rec(sf::Vector2f(recSize, recSize));
                rec.setFillColor(sf::Color::Blue);
                rec.setOrigin(sf::Vector2f(-offset, -offset));
                rec.setPosition(sf::Vector2f(x * recSize, y * recSize));
                window.draw(rec);

                if (grid[y][x].is_car)
                {
                    sf::RectangleShape rec(sf::Vector2f(recSize, recSize));
                    rec.setFillColor(sf::Color::Red);
                    rec.setOrigin(sf::Vector2f(-offset, -offset));
                    rec.setPosition(sf::Vector2f(x * recSize, y * recSize));
                    window.draw(rec);
                }
            }

        window.display(); // Выводим сетку на экран
        sf::sleep(sf::seconds(sleep_t));

        model.make_next_step(); // Сменяем "поколение"
        grid = model.get_next_result();

        // При окончании расчёта закончить работу программы
        if (model.get_cur_time() == end_time)
        {
            std::cout << "Calculation stopped." << std::endl;
            break;
        }
    }
}