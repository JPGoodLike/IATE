#include "Lists.hpp"
#include "Vectors.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

constexpr int g_pixelPerUnit = 50;

namespace mbgl {

    vec2 SpaceToScreenPoint(vec2 pointInSpace, int pixelPerUnit, sf::RenderWindow& window) {
        vec2 windowCenter = window.getSize();
        windowCenter /= 2;
        vec2 dirInPixels = pointInSpace * pixelPerUnit;
        dirInPixels.y = -dirInPixels.y;
        return windowCenter + dirInPixels;
    }

    vec2 ScreenToSpacePoint(vec2 pointOnScreen, int pixelPerUnit, sf::RenderWindow& window) {
        vec2 windowCenter = window.getSize();
        windowCenter /= 2;
        vec2 dirInUnits = (pointOnScreen - windowCenter)/pixelPerUnit;
        dirInUnits.y = -dirInUnits.y;
        return dirInUnits;
    }

    float PolygonNodeCross(CDLList<vec2>::CDLLNode* node) {
        vec2 v1 = node->Back()->Get() - node->Get();
        vec2 v2 = node->Next()->Get() - node->Get();
        return v1.x * v2.y - v2.x * v1.y;
    }

    float TreePointsCross(const vec2 p1, const vec2 p2, const vec2 p3) {
        vec2 v1 = p1 - p2;
        vec2 v2 = p3 - p2;
        return v1.x * v2.y - v2.x * v1.y;
    }

    class Triangle {
    public:
        vec2 vertices[3];
        
        Triangle(vec2 vertices[3]) {
            for (int i = 0; i < 3; i++) {
                this->vertices[i] = vertices[i];
            }            
        }        
        Triangle(vec2 p1, vec2 p2, vec2 p3) {
            vertices[0] = p1;
            vertices[1] = p2;
            vertices[2] = p3;
        }

        // float Area() {
        //     float a = (vertices[1] - vertices[0]).Magnitude();
        //     float b = (vertices[2] - vertices[1]).Magnitude();
        //     float c = (vertices[0] - vertices[2]).Magnitude();
        //     float p = (a + b + c) / 2;
        //     return sqrt(p * (p - a) * (p - b) * (p - c));
        // }

        float Area() {
            vec2 v1 = vertices[0] - vertices[1];
            vec2 v2 = vertices[2] - vertices[1];
            return (v1.x * v2.y - v2.x * v1.y) / 2; // cross product = area of a rectangle
        }

        bool IsContain(vec2 point) {
            vec2 v1 = vertices[0] - vertices[1];
            vec2 v2 = vertices[2] - vertices[1];
            vec2 vp = point - vertices[1];
            float det = v1.x * v2.y - v2.x * v1.y;
            float w1 = (v2.y * vp.x - v2.x * vp.y) / det; // found with inverse matrix
            float w2 = (-v1.y * vp.x + v1.x * vp.y) / det;
            // std::cout << w1 << " " << w2 << "\n";
            return (w1 >= 0 && w2 >=0 && w1 + w2 <= 1);
        }

        void NormalizeOrder() {
            if (Area() < 0) {
                vec2 temp = vertices[2];
                vertices[2] = vertices[0];
                vertices[0] = temp;
            }
        }

        void Draw(sf::RenderWindow& window, sf::Color color = sf::Color::White) {
            sf::ConvexShape shape(3);
            for (int i = 0; i < 3; i++) {
                shape.setPoint(i, SpaceToScreenPoint(vertices[i], g_pixelPerUnit, window));
            }
            shape.setFillColor(color);
            shape.setOutlineColor(sf::Color::Black);
            shape.setOutlineThickness(0);
            window.draw(shape);
        }

        void DrawOutline(sf::RenderWindow& window, sf::Color color = sf::Color::Black) {
            sf::Vertex line1[] = {
                sf::Vertex(SpaceToScreenPoint(vertices[0], g_pixelPerUnit, window), color),
                sf::Vertex(SpaceToScreenPoint(vertices[1], g_pixelPerUnit, window), color)
            };
            sf::Vertex line2[] = {
                sf::Vertex(SpaceToScreenPoint(vertices[1], g_pixelPerUnit, window), color),
                sf::Vertex(SpaceToScreenPoint(vertices[2], g_pixelPerUnit, window), color)
            };
            sf::Vertex line3[] = {
                sf::Vertex(SpaceToScreenPoint(vertices[2], g_pixelPerUnit, window), color),
                sf::Vertex(SpaceToScreenPoint(vertices[0], g_pixelPerUnit, window), color)
            };
            window.draw(line1, 2, sf::Lines);
            window.draw(line2, 2, sf::Lines);
            window.draw(line3, 2, sf::Lines);
        }
    };

    class Polygon {
    public:
        CDLList<vec2> vertices;
        std::vector<Triangle> triangles;
        std::vector<Polygon> convexPolygons;

        void Triangulate(sf::RenderWindow& window) {
            if (vertices.Size() < 3)
                return;

            // NormalizeOrder();
            
            triangles.clear();

            // struct Tuple {
            //     vec2 point;
            //     CDLList<vec2>::CDLLNode* nodeInGroup;
            // };
            CDLList<vec2> cVerts = vertices;
            CDLList<CDLList<vec2>::CDLLNode*> convex;
            CDLList<CDLList<vec2>::CDLLNode*> concave;

            auto cNode = cVerts.Root();
            for (int i = 0; i < cVerts.Size(); i++) {
                float cross = PolygonNodeCross(cNode);
                if (cross >= 0) {
                    convex.AddFront(cNode);
                } else {
                    concave.AddFront(cNode);
                }
                cNode = cNode->Next();
            }

            while (convex.Size() + concave.Size() > 3) {
                auto cNodeConvex = convex.Root();
                for (int i = 0; i < convex.Size(); i++, cNodeConvex = cNodeConvex->Next()) {
                    auto nodeOnPolygon = cNodeConvex->Get();
                    Triangle ear(nodeOnPolygon->Back()->Get(), nodeOnPolygon->Get(), nodeOnPolygon->Next()->Get());
                    ear.Draw(window, sf::Color::Red);
                    window.display();

                    bool b = true;
                    auto cNodeConcave = concave.Root();
                    for (int j = 0; j < concave.Size(); j++, cNodeConcave = cNodeConcave->Next()) {
                        if (cNodeConcave->Get() == nodeOnPolygon->Back() || cNodeConcave->Get() == nodeOnPolygon->Next())
                            continue;
                        
                        if (ear.IsContain(cNodeConcave->Get()->Get())) {
                            b = false;
                            break;
                        }
                    }
                    
                    if (b) { //clip the ear
                        triangles.push_back(ear);

                        auto ajNode1 = nodeOnPolygon->Back();
                        auto ajNode2 = nodeOnPolygon->Next();

                        bool isConvexPre1 = PolygonNodeCross(ajNode1) >= 0;
                        bool isConvexPre2 = PolygonNodeCross(ajNode2) >= 0;
                        cVerts.Remove(nodeOnPolygon);
                        bool isConvexAfter1 = PolygonNodeCross(ajNode1) >= 0;
                        bool isConvexAfter2 = PolygonNodeCross(ajNode2) >= 0;
                        if (isConvexAfter1 != isConvexPre1) {
                            if (isConvexAfter1) {
                                concave.Remove(ajNode1);
                                convex.AddFront(ajNode1);
                            } else {
                                convex.Remove(ajNode1);
                                concave.AddFront(ajNode1);
                            }
                        }
                        if (isConvexAfter2 != isConvexPre2) {
                            if (isConvexAfter2) {
                                concave.Remove(ajNode2);
                                convex.AddFront(ajNode2);
                            } else {
                                convex.Remove(ajNode2);
                                concave.AddFront(ajNode2);
                            }
                        }
                        
                        convex.Remove(cNodeConvex);
                        break;
                    }
                }
            }

            triangles.push_back(Triangle(cVerts.Get(0)->Get(), cVerts.Get(1)->Get(), cVerts.Get(2)->Get()));
            // std::cout << triangles.size() << "\n";
        }

        void DivideOnConvex(sf::RenderWindow& window) {
            convexPolygons.clear();

            NormalizeOrder();
            Triangulate(window);

            DLList<Triangle> cTriangles;
            for (Triangle t : triangles)
                cTriangles.AddFront(t);
            
            while (cTriangles.Size() > 0) {
                Triangle t = cTriangles.Root()->Get();                
                Polygon p;
                for (int i = 0; i < 3; i++) {
                    p.vertices.AddFront(t.vertices[i]);
                }
                cTriangles.Remove(cTriangles.Root());

                auto cNodeT = cTriangles.Root();
                for (int i = 0; i < cTriangles.Size(); i++, cNodeT = cNodeT->Next()) {
                    std::vector<CDLList<vec2>::CDLLNode*> commonPoints;
                    commonPoints.reserve(2);
                    vec2 remainingPoint;
                    
                    auto cNodeP = p.vertices.Root();
                    for (int j = 0; j < p.vertices.Size(); j++, cNodeP = cNodeP->Next()) {
                        for (int k = 0; k < 3; k++) {
                            if (cNodeT->Get().vertices[k] == cNodeP->Get()) {
                                commonPoints.push_back(cNodeP);
                            }
                        }
                        if (commonPoints.size() == 2)
                            break;
                    }
                    if (commonPoints.size() < 2)
                        continue;
                    
                    for (int k = 0; k < 3; k++) {
                        if (cNodeT->Get().vertices[k] != commonPoints[0]->Get() && cNodeT->Get().vertices[k] != commonPoints[1]->Get()) {
                            remainingPoint = cNodeT->Get().vertices[k];
                            break;
                        }
                    }

                    if (commonPoints[0]->Back() == commonPoints[1]) {
                        auto temp = commonPoints[0];
                        commonPoints[0] = commonPoints[1];
                        commonPoints[1] = temp;
                    }

                    bool isConvex1 = TreePointsCross(commonPoints[0]->Back()->Get(), commonPoints[0]->Get(), remainingPoint) >= 0;
                    bool isConvex2 = TreePointsCross(remainingPoint, commonPoints[1]->Get(), commonPoints[1]->Next()->Get()) >= 0;
                    if (isConvex1 && isConvex2) {
                        p.vertices.InsertAfter(remainingPoint, commonPoints[0]);
                        cTriangles.Remove(cNodeT);
                    }
                }
                convexPolygons.push_back(p);
            }
        }

        // bool NormalizeOrder() {
        //     if (vertices.Size() < 3)
        //         return false;
            
        //     float sum = 0;
        //     auto cNode = vertices.Root();
        //     for (int i = 0; i < vertices.Size(); i++) {
        //         vec2 v1 = cNode->Back()->Get() - cNode->Get();
        //         vec2 v2 = cNode->Next()->Get() - cNode->Get();
        //         sum += v1.x * v2.y - v2.x * v1.y;
        //     }
        //     // std::cout << sum << "\n";
        //     if (sum < 0) {
        //         vertices = vertices.Reversed();
        //         return true;
        //     }
        //     return false;
        // }

        bool NormalizeOrder() {
            if (vertices.Size() < 3)
                return false;
            
            float sum = 0;
            auto cNode = vertices.Root();
            for (int i = 0; i < vertices.Size(); i++) {
                vec2 p1 = cNode->Get();
                vec2 p2 = cNode->Next()->Get();
                sum += (p2.x - p1.x) * (p2.y + p1.y);
            }
            std::cout << sum << "\n";
            if (sum < 0) {
                vertices = vertices.Reversed();
                return true;
            }
            return false;
        }

        void DrawConvex(sf::RenderWindow& window, sf::Color color = sf::Color::White) {
            sf::ConvexShape shape(vertices.Size());

            auto cNode = vertices.Root();
            for (int i = 0; i < vertices.Size(); i++, cNode = cNode->Next()) {
                shape.setPoint(i, SpaceToScreenPoint(cNode->Get(), g_pixelPerUnit, window));
            }
            shape.setFillColor(color);
            window.draw(shape);
        }

        void DrawOutline(sf::RenderWindow& window, sf::Color color = sf::Color::Black) {
            auto cNode = vertices.Root();
            for (int i = 0; i < vertices.Size(); i++) {
                sf::Vertex line[] = {
                    sf::Vertex(SpaceToScreenPoint(cNode->Get(), g_pixelPerUnit, window), color),
                    sf::Vertex(SpaceToScreenPoint(cNode->Next()->Get(), g_pixelPerUnit, window), color)
                };

                window.draw(line, 2, sf::Lines);                                        
                cNode = cNode->Next();
            }
        }
    };
}