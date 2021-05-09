// main.cpp - Главный файл, который содержит главую функцию main, в которой описан основной цикл работы программы
#include "Window.h"
#include "imgui.h"
#include "imgui-sfml.h"
#include "SFML/glut.h"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Program", sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    Window wind;


    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
    

        window.clear(sf::Color::Color(225, 230, 235));
        wind.logic();
        window.draw(wind);

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
}