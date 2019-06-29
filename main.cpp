// Стандартные библиотеки
#include <iostream>

// Пользовательские библиотеки
#include <SFML/Graphics.hpp>
#include "Model.hpp"

bool DEBUG = false;                                                // Переменная-флаг для вывода отладочной информации
void input_data();                                                 // Ввод исходных данных
void debugOutput(const vector<vector<float>>& grid, int numCells); // Вывод данных по сетке в консоль

void view_model(Model &model, vector<vector<float>> &grid, int numCells);

// Данные по модели
int numCells_    = 128;   // Количество клеток
float diffusion_ = 10.0f; // Коэффициент диффузии [м^2 / с]
float courant_   = 0.25f; // Коэффициент Куранта []
float length_    = 10.0f; // Размер квадратной области [м]
float end_time_  = 2.0f;  // Конечное время [с]

// Начальная концентрация
float c_0 = 0.3f;

// Коэффициенты для верхнего и нижнего граничных условий
float alpha_1 = 0.0f;
float alpha_2 = 0.0f;
float beta_1  = 1.0f;
float beta_2  = 1.0f;
float mu_1    = 0.4f;
float mu_2    = 0.6f;

// Концентрации на левой и правой границе сетки
float c_1 = 0.3f;
float c_2 = 0.3f;

// Данные по отображению на экране
const int winSize = 800;    // Размер квадратного окна
const float offset = 16.0f; // Отступ сетки в окне
const float recSize = 5.0f; // Размер ячеек

int main()
{
//    input_data();
    Model model(numCells_, diffusion_, courant_, length_, end_time_);    // Модель
    model.setInnerCoeffs(c_0);                                           // Задание начальной концентрации
    model.setUpDownCoeffs(alpha_1, alpha_2, beta_1, beta_2, mu_1, mu_2); // Задание верхней и нижней границ
    model.setLeftRightCoeffs(c_1, c_2);                                  // Задание левой и правой границ

    model.userInit(); // Инициализация модели

    vector<vector<float>> grid = model.getGrid(); // Связь сетки модели с "вьювером"
    int numCells = model.getNumCells();           // Связь размера сетки модели с "вьювером"
    view_model(model, grid, numCells);            // Вывод модели на экран

    return 0;
}

void view_model(Model &model, vector<vector<float>>& grid, int numCells)
{
    // Инициализация окна
    sf::RenderWindow window(sf::VideoMode(winSize, winSize), L"Клеточный автомат \"2D-диффузия\"");

    // Пока окно открыто, выводим сетку на экран
    while (window.isOpen())
    {
        // Вывод данных в консоль
        std::cout << "Current time: " << model.getCurTime() << std::endl;
        if (DEBUG) debugOutput(grid, numCells);

        // Инициализация события
        sf::Event event;
        while (window.pollEvent(event)) // Обработчик события "окно закрыто"
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black); // Устанавливаем окна чёрного цвета
        grid = model.getGrid();         // Получаем обновлённые данные по сетке

        // Отрисовываем ненулевые ячейки сетки
        for (int x = 1; x <= numCells; x++)
            for (int y = 1; y <= numCells; y++)
                if (grid[x][y] != 0.0f)
                {
                    sf::RectangleShape rec(sf::Vector2f(recSize, recSize));
                    rec.setFillColor(sf::Color(0, 255, 0, (sf::Uint8) (grid[x][y] * 255)));
                    rec.setOrigin(sf::Vector2f(-offset, -offset));
                    rec.setPosition(sf::Vector2f((x - 1) * recSize, (y - 1) * recSize));
                    window.draw(rec);
                };

        window.display(); // Выводим сетку на экран
        if (DEBUG) sf::sleep(sf::seconds(0.5f)); // Задерживаем изображение

        int workFlag = model.calcEngine(); // Сменяем "поколение"

        // Если концентрация вышла за границы [0; 1] закончить работу программы
        if (model.checkGrid() != 0.0)
        {
            std::cout << "Concentration is out of [0; 1] border." << std::endl;
            std::cout << "Value: " << model.checkGrid() << std::endl;
            sf::sleep(sf::seconds(0.5f)); // Задерживаем изображение
            break;
        }

        // При окончании расчёта закончить работу программы
        if (workFlag == 1)
        {
            std::cout << "Calculation stopped." << std::endl;
            system("pause");
            break;
        }
    }
}

void input_data()
{
    std::cout << "Set cell number: ";
    std::cin >> numCells_;
    std::cout << "Set diffusion coefficient (m^2 / s): ";
    std::cin >> diffusion_;
    std::cout << "Set courant number: ";
    std::cin >> courant_;
    std::cout << "Set grid length (m): ";
    std::cin >> length_;
    std::cout << "Set time of calculation (s): ";
    std::cin >> end_time_;

    // Задание начальной концентрации
    std::cout << "Set initial concentration (c_0): ";
    std::cin >> c_0;

    // Задание верхней и нижней границ
    std::cout << "Set border coefficients (alpha_1, beta_1, mu_1): ";
    std::cin >> alpha_1 >> beta_1 >> mu_1;
    std::cout << "Set border coefficients (alpha_2, beta_2, mu_2): ";
    std::cin >> alpha_2 >> beta_2 >> mu_2;

    // Задание левой и правой границ
    std::cout << "Concentrations on left and right borders (c_1, c_2): ";
    std::cin >> c_1 >> c_2;
}

void debugOutput(const vector<vector<float>>& grid, int numCells)
{
    std::cout << "------" << std::endl;

    for (int x = 0; x < numCells + 2; x++)
    {
        for (int y = 0; y < numCells + 2; y++)
            std::cout << grid[x][y] << "\t";
        std::cout << std::endl;
    }
};