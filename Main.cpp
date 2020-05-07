#include <iostream>
#include <SFML/Graphics.hpp>
#include "EarClipping.cpp"

using namespace mbgl;

enum DrawMode {
    Basic = 0,
    Triangulated = 1,
    ConvexPolygons = 2    
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Ear Clipping");
    window.setKeyRepeatEnabled(false);
    
    Polygon p;
    bool pOrder = false;

    // Triangle t(vec2(-5, -3), vec2(3, -3), vec2(0, 5));
    // sf::Color tColor = sf::Color::White;
    // std::cout << t.Area() << "\n";
    // t.NormalizeOrder();
    // std::cout << t.Area() << "\n";

    DrawMode drawMode = Basic;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    vec2 mousePos = sf::Mouse::getPosition(window);
                    if (!pOrder) {
                        p.vertices.AddFront(ScreenToSpacePoint(mousePos, g_pixelPerUnit, window));
                    } else
                        p.vertices.AddBack(ScreenToSpacePoint(mousePos, g_pixelPerUnit, window));
                    if (p.NormalizeOrder())
                        pOrder = !pOrder;
                }
                // if (event.mouseButton.button == sf::Mouse::Right) {
                //     vec2 mousePos = sf::Mouse::getPosition(window);
                //     tColor = (t.IsContain(ScreenToSpacePoint(mousePos, g_pixelPerUnit, window))) ? sf::Color::Green : sf::Color::Red;
                // }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    drawMode = Basic;
                } else if (event.key.code == sf::Keyboard::Num2) {
                    p.Triangulate(window);
                    drawMode = Triangulated;
                } else if (event.key.code == sf::Keyboard::Num3) {
                    p.DivideOnConvex(window);
                    drawMode = ConvexPolygons;
                }
            }
        }

        window.clear(sf::Color(100, 100, 115));
        p.DrawOutline(window);
        if (drawMode == ConvexPolygons) {
            for (auto subP : p.convexPolygons) {
                subP.DrawConvex(window);
                subP.DrawOutline(window);
            }
        } else {
            for (auto t : p.triangles) {
                t.Draw(window, sf::Color::Green);
                if (drawMode == Triangulated)
                    t.DrawOutline(window);
            }
            // t.Draw(window, tColor);
        }
        window.display();
    }

    return 0;
}