#include <SFML/Graphics.hpp>
#include "Model.hpp"
#include <iostream>

bool DEBUG = false;                           // Переменная-флаг для вывода отладочной информации
void input_data();                            // Ввод исходных данных
void debugOutput(float** grid, int numCells); // Вывод данных по сетке в консоль

// Данные по модели
int numCells_; // Количество клеток
float diffusion_; // Коэффициент диффузии [м^2 / с]
float courant_; // Коэффициент Куранта []
float length_; // Размер квадратной области [м]
float end_time_; // Конечное время [с]

// Начальная концентрация
float c_0;

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

int main()
{
//    input_data();
//    Model model(numCells_, diffusion_, courant_, length_, end_time_);
//    model.setInnerCoeffs(c_0);
//    model.setUpDownCoeffs(alpha_1, alpha_2, beta_1, beta_2, mu_1, mu_2);
//    model.setLeftRightCoeffs(c_1, c_2);

    Model model(256, 10.0f, 0.25f, 10.0f, 2.0f); // Модель
    model.setInnerCoeffs(0.3f); // Задание начальной концентрации
    model.setUpDownCoeffs(0.0f, 0.0f, 1.0f, 1.0f, 0.4f, 0.6f); // Задание верхней и нижней границ
    model.setLeftRightCoeffs(0.3f, 0.3f); // Задание левой и правой границ

    model.userInit(); // Инициализация модели

    float** grid = model.getGrid(); // Связь сетки модели с "вьювером"
    int numCells = model.getNumCells(); // Связь размера сетки модели с "вьювером"

    const int winSize = 800;    // Размер квадратного окна
    const float offset = 16.0f; // Отступ сетки в окне
    const float recSize = 3.0f; // Размер ячеек

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
                    rec.setFillColor(sf::Color(0, 255, 0, (sf::Uint8)(grid[x][y] * 255)));
                    rec.setOrigin(sf::Vector2f(-offset, -offset));
                    rec.setPosition(sf::Vector2f((x - 1) * recSize, (y - 1) * recSize));
                    window.draw(rec);
                };

        window.display(); // Выводим сетку на экран
        if (DEBUG) sf::sleep(sf::seconds(0.5f)); // Задерживаем изображение
        system("cls"); // Очищаем экран консоли

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

    return 0;
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

void debugOutput(float** grid, int numCells)
{
    std::cout << "------" << std::endl;

    for (int x = 0; x < numCells + 2; x++)
    {
        for (int y = 0; y < numCells + 2; y++)
            std::cout << grid[x][y] << "\t";
        std::cout << std::endl;
    }
};