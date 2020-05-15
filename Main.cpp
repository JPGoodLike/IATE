#include <iostream>
#include <SFML/Graphics.hpp>
#include "EarClipping.cpp"

using namespace mbgl;

enum DrawMode {
    Outline = 0,
    BasicShape = 1,
    Triangulated = 2,
    ConvexPolygons = 3    
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

    DrawMode drawMode = Outline;

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
                    drawMode = Outline;
                } else if (event.key.code == sf::Keyboard::Num2) {
                    p.Triangulate(window);
                    drawMode = BasicShape;
                } else if (event.key.code == sf::Keyboard::Num3) {
                    p.Triangulate(window);
                    drawMode = Triangulated;
                } else if (event.key.code == sf::Keyboard::Num4) {
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
        } else if (drawMode != Outline) {
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