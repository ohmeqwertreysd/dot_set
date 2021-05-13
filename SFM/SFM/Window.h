// Заголовочный файл Window.h, который определяет константы, объявляет класс Window и его методы, а также содержит вспомогательные функции
#pragma once
#ifndef WINDOW__H
#define WINDOW__H
#include "hashtable_struct.h"
#include "list_struct.h"
#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <fstream>
#include <sstream>

constexpr size_t window_width = 1500U;  // Ширина окна
constexpr size_t window_height = 720U; // Высота окна

constexpr size_t field_indent = 10U; // Отступ поля от окна

constexpr size_t field_width = 1000U;  // Ширина поля для множества точек
constexpr size_t field_height = 700U; // Высота поля для множества точек

constexpr float dot_size = 5.f; // Размер точки (радиус круга)

constexpr float epsilon_float = 1E-15f; // Абсолютная и относительная погрешность вещественных чисел

typedef struct floatOwn
{
    float data;
    floatOwn(const float& d = float()) : data(d) {}
}floatw;

typedef ds::HashTable<floatw, ds::Pair<sf::Vector2f, sf::Vector2f>> hashtable;

bool operator==(const floatw& r, const floatw& l);
bool operator<(const floatw& r, const floatw& l);
float operator*(const floatw& r, const float& l);
bool operator==(const sf::Vector2f& left, const sf::Vector2f& right);
bool operator==(const sf::VertexArray& left, const sf::VertexArray& right);

bool isCollinear(const ds::Pair<sf::Vector2f, sf::Vector2f>& ab, const ds::Pair<sf::Vector2f, sf::Vector2f>& cd);
bool isOrthogonal(const ds::Pair<sf::Vector2f, sf::Vector2f>& ab, const ds::Pair<sf::Vector2f, sf::Vector2f>& cd);
bool isCorrectDot(const sf::Vector2f& a);
sf::Vector2f get_vector_between_two_point(const sf::Vector2f& a, const sf::Vector2f& b);
float distance_between_two_point(const sf::Vector2f& a, const sf::Vector2f& b);
sf::Vector2f get_position_circle(const sf::Vector2f& point);

class Window :public sf::Drawable, public sf::Transformable
{
private:
    int dots_amount;
    bool draw_flag;
    ds::List<sf::Vector2f>* dots;
    std::string dots_textbox;
    ds::List<sf::VertexArray>* squares;
protected:
    sf::Vector2f get_random_point();
    ds::List<sf::Vector2f>* generate_points(const size_t& dot_amount);
protected:
    ds::Pair<ds::List<floatw>*, hashtable*> distation_calc();
    ds::List<sf::VertexArray>* search_square(hashtable* ht, ds::List<floatw>* dists);
protected:
    void textbox_to_dots();
    std::string dots_to_textbox();
    std::string file_to_textbox();
    void dots_to_file();
    void square_to_file();
public:
    Window();
    ~Window();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void logic();
};

#endif // !WINDOW__H
