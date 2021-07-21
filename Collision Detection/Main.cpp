#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <string>
#include "Polygon.h"


//AABB stands for Axis Align Bounding Box
bool AABB(Polygon polygon1, Polygon polygon2);

//SAT stands for Separated Axis Theorem
bool SAT(Polygon &polygon1, Polygon &polygon2);

//SAT with static resolution (pushes the shapes back if overlaped)
bool SAT_static(Polygon &polygon1, Polygon &polygon2, bool hardPush);



int main() {

	enum collisionMethod {
		aabb,
		sat,
		sat_static_steady,
		sat_static_push,
	};

	enum polygons {
		triangle,
		square,
		pentagon,
		costumShape,
	};


	sf::RenderWindow window(sf::VideoMode(700, 700), "Collision Detection");

	std::vector<Polygon> polys;

	std::vector<sf::Vector2f> trianglePoints;
	trianglePoints.push_back(sf::Vector2f(35.0f, 0.0f));
	trianglePoints.push_back(sf::Vector2f(70.0f, 70.0f));
	trianglePoints.push_back(sf::Vector2f(0.0f, 70.0f));
	polys.push_back(Polygon(3, sf::FloatRect(100.0f, 140.0f, 70.0f, 70.0f), trianglePoints, sf::Color(210, 100, 100)));

	std::vector<sf::Vector2f> squarePoints;
	squarePoints.push_back(sf::Vector2f(0.0f, 0.0f));
	squarePoints.push_back(sf::Vector2f(80.0f, 0.0f));
	squarePoints.push_back(sf::Vector2f(80.0f, 80.0f));
	squarePoints.push_back(sf::Vector2f(0.0f, 80.0f));
	polys.push_back(Polygon(4, sf::FloatRect(480.0f, 410.0f, 80.0f, 80.0f), squarePoints, sf::Color(151, 254, 146)));

	std::vector<sf::Vector2f> pentagonPoints;
	pentagonPoints.push_back(sf::Vector2f(40.0f, 0.0f));
	pentagonPoints.push_back(sf::Vector2f(80.0f, 35.0f));
	pentagonPoints.push_back(sf::Vector2f(65.0f, 80.0f));
	pentagonPoints.push_back(sf::Vector2f(15.0f, 80.0f));
	pentagonPoints.push_back(sf::Vector2f(0.0f, 35.0f));
	polys.push_back(Polygon(5, sf::FloatRect(310.0f, 320.0f, 80.0f, 80.0f), pentagonPoints, sf::Color(151, 164, 253)));

	//Can change the points of this convex polygon
	std::vector<sf::Vector2f> costumShapePoints;
	costumShapePoints.push_back(sf::Vector2f(25.0f, 5.0f));
	costumShapePoints.push_back(sf::Vector2f(40.0f, 0.0f));
	costumShapePoints.push_back(sf::Vector2f(85.0f, 40.0f));
	costumShapePoints.push_back(sf::Vector2f(70.0f, 85.0f));
	costumShapePoints.push_back(sf::Vector2f(20.0f, 70.0f));
	costumShapePoints.push_back(sf::Vector2f(0.0f, 45.0f));
	polys.push_back(Polygon(6, sf::FloatRect(260.0f, 530.0f, 90.0f, 90.0f), costumShapePoints, sf::Color(237, 237, 78)));
	

	Polygon* currentPoly = NULL;
	int collisionMethod = -1;
	bool movePolygon2 = true;

	sf::Font font;
	font.loadFromFile("RegularFont\\tecnico_regular.ttf");

	sf::Text textChangeMethod("click Left Arrow and Right Arrow to change method", font, 25);
	textChangeMethod.setFillColor(sf::Color::White);
	textChangeMethod.setPosition(sf::Vector2f(20.0f, 20.0f));
	bool methodChanged = false;

	sf::Text textMovePoly("click AWSD to move the polygon", font, 25);
	textMovePoly.setFillColor(sf::Color::White);
	textMovePoly.setPosition(sf::Vector2f(110.0f, 60.0f));
	bool polygonMoved = false;

	sf::Text textRotatePoly("click Q and E to rotate the shape", font, 25);
	textRotatePoly.setFillColor(sf::Color::White);
	textRotatePoly.setPosition(sf::Vector2f(100.0f, 100.0f));
	bool polygonRotated = false;


	while (window.isOpen()){

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2f mouse = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
					if (polys[triangle].checkMouseClick(mouse)) {
						std::cout << "Clicked triangle\n";
						currentPoly = &polys[triangle];
					}
					if (polys[square].checkMouseClick(mouse)) {
						std::cout << "Clicked square\n";
						currentPoly = &polys[square];
					}
					if (polys[pentagon].checkMouseClick(mouse)) {
						std::cout << "Clicked pentagon\n";
						currentPoly = &polys[pentagon];
					}
					if (polys[costumShape].checkMouseClick(mouse)) {
						std::cout << "Clicked costum shape\n";
						currentPoly = &polys[costumShape];
					}
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Key::A) {
					if (currentPoly != NULL) {
						polygonMoved = true;
						currentPoly->move(sf::Vector2f(-10.0f, 0.0f));
					}
				} else if (event.key.code == sf::Keyboard::Key::D) {
					if (currentPoly != NULL) {
						polygonMoved = true;
						currentPoly->move(sf::Vector2f(10.0f, 0.0f));
					}
				} else if (event.key.code == sf::Keyboard::Key::W) {
					if (currentPoly != NULL) {
						polygonMoved = true;
						currentPoly->move(sf::Vector2f(0.0f, -10.0f));
					}
				} else if (event.key.code == sf::Keyboard::Key::S) {
					if (currentPoly != NULL) {
						polygonMoved = true;
						currentPoly->move(sf::Vector2f(0.0f, 10.0f));
					}
				} else if (event.key.code == sf::Keyboard::Key::Q) {
					if (currentPoly != NULL) {
						polygonRotated = true;
						currentPoly->rotate(15);
					}
				} else if (event.key.code == sf::Keyboard::Key::E) {
					if (currentPoly != NULL) {
						polygonRotated = true;
						currentPoly->rotate(-15);
					}

				} else if (event.key.code == sf::Keyboard::Key::Left) {
					collisionMethod--;
					if (collisionMethod < 0) {
						collisionMethod = 3;
					}
					switch (collisionMethod) {
						case aabb:
							for (int i = 0; i < polys.size(); i++) {
								polys[i].enableBox();
							}
							break;
						case sat:
							for (int i = 0; i < polys.size(); i++) {
								polys[i].enableNormalLines();
							}
							break;
						case sat_static_steady:
							for (int i = 0; i < polys.size(); i++) {
								polys[i].disableDebugLines();
							}
							movePolygon2 = false;
							break;
						case sat_static_push:
							for (int i = 0; i < polys.size(); i++) {
								polys[i].disableDebugLines();
							}
							movePolygon2 = true;
							break;
						default:
							std::cout << "Erro";
							break;
					}
					methodChanged = true;

				} else if (event.key.code == sf::Keyboard::Key::Right) {
					collisionMethod++;
					if (collisionMethod > 3) {
						collisionMethod = 0;
					}
					switch (collisionMethod) {
						case aabb:
							for (int i = 0; i < polys.size(); i++) {
								polys[i].enableBox();
							}
							break;
						case sat:
							for (int i = 0; i < polys.size(); i++) {
								polys[i].enableNormalLines();
							}
							break;
						case sat_static_steady:
							for (int i = 0; i < polys.size(); i++) {
								polys[i].disableDebugLines();
							}
							movePolygon2 = false;
							break;
						case sat_static_push:
							movePolygon2 = true;
							for (int i = 0; i < polys.size(); i++) {
								polys[i].disableDebugLines();
							}
							break;
						default:
							std::cout << "Erro";
							break;
					}
					methodChanged = true;
				}
			}
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (currentPoly != NULL) {
			for (int i = 0; i < polys.size(); i++) {
				if (currentPoly != &polys[i]) {
					switch (collisionMethod) {
						case aabb:
							std::cout << "Colision result: " << AABB(*currentPoly, polys[i]) << std::endl;
							break;
						case sat:
							std::cout << "Colision result: " << SAT(*currentPoly, polys[i]) << std::endl;
							break;
						case sat_static_steady:
						case sat_static_push:
							SAT_static(*currentPoly, polys[i], movePolygon2);
							break;
						default:
							break;
					}
				}
			}
		}

		//function calls to render on screen
		window.clear();
		for (int i = 0; i < polys.size(); i++) {
			polys[i].draw(window);
		}
		//make sure current poly is the last one drawn
		if (currentPoly)
			currentPoly->draw(window);
		if(!methodChanged)
			window.draw(textChangeMethod);
		if(!polygonMoved)
			window.draw(textMovePoly);
		if(!polygonRotated)
			window.draw(textRotatePoly);
		window.display();
	}
}


bool AABB(Polygon polygon1, Polygon polygon2) {
	sf::FloatRect rect1 = polygon1.getRect();
	sf::FloatRect rect2 = polygon2.getRect();

	//for the X-axis
	if (rect1.left < (rect2.left + rect2.width) && (rect1.left + rect1.width) > rect2.left) {
		//for the Y-axis
		if (rect1.top < (rect2.top + rect2.height) && (rect1.top + rect1.height) > rect2.top) {
			return true;
		}
	}

	return false;
}


bool SAT(Polygon &polygon1, Polygon &polygon2) {
	
	int p1Lados = polygon1.getNmberEdges();
	int p2Lados = polygon2.getNmberEdges();
	sf::Vector2f *p1Points = new sf::Vector2f[p1Lados];
	sf::Vector2f *p2Points = new sf::Vector2f[p2Lados];
	for (int i = 0; i < p1Lados; i++) {
		p1Points[i] = polygon1.getPoint(i);
	}
	for (int i = 0; i < p2Lados; i++) {
		p2Points[i] = polygon2.getPoint(i);
	}

	//for first polygon
	for (int firstPoint = 0; firstPoint < p1Lados; firstPoint++) {
		int secondPoint = (firstPoint + 1) % p1Lados;

		//Consider a perpendicular axis
		sf::Vector2f projAxis( -(p1Points[firstPoint].y - p1Points[secondPoint].y), p1Points[firstPoint].x - p1Points[secondPoint].x);

		//Find the min and max of the projected "shadow"
		float min1 = INFINITY;
		float max1 = -INFINITY;
		for (int pointNumber = 0; pointNumber < p1Lados; pointNumber++) {

			float projectedPoint = (p1Points[pointNumber].x * projAxis.x) + (p1Points[pointNumber].y * projAxis.y);
			min1 = std::min(min1, projectedPoint);
			max1 = std::max(max1, projectedPoint);
		}

		//Find the min and max of the projected "shadow"
		float min2 = INFINITY;
		float max2 = -INFINITY;
		for (int pointNumber = 0; pointNumber < p2Lados; pointNumber++) {

			float projectedPoint = (p2Points[pointNumber].x * projAxis.x) + (p2Points[pointNumber].y * projAxis.y);
			min2 = std::min(min2, projectedPoint);
			max2 = std::max(max2, projectedPoint);
		}

		if (min1 > max2 || max1 < min2) {
			return false;
		}
	}
	
	//for second polygon 
	for (unsigned int firstPoint = 0; firstPoint < p2Lados; firstPoint++) {
		int secondPoint = (firstPoint + 1) % p2Lados;

		//Consider a perpendicular axis
		sf::Vector2f projAxis(-(p2Points[firstPoint].y - p2Points[secondPoint].y), p2Points[firstPoint].x - p2Points[secondPoint].x);

		//Find the min and max of the projected "shadow"
		float min1 = INFINITY;
		float max1 = -INFINITY;
		for (int pointNumber = 0; pointNumber < p1Lados; pointNumber++) {

			float projectedPoint = (p1Points[pointNumber].x * projAxis.x) + (p1Points[pointNumber].y * projAxis.y);
			min1 = std::min(min1, projectedPoint);
			max1 = std::max(max1, projectedPoint);
		}

		//Find the min and max of the projected "shadow"
		float min2 = INFINITY;
		float max2 = -INFINITY;
		for (int pointNumber = 0; pointNumber < p2Lados; pointNumber++) {

			float projectedPoint = (p2Points[pointNumber].x * projAxis.x) + (p2Points[pointNumber].y * projAxis.y);
			min2 = std::min(min2, projectedPoint);
			max2 = std::max(max2, projectedPoint);
		}

		if (min1 > max2 || max1 < min2) {
			return false;
		}
	}

	return true;
}


float minDist(float min1, float min2, float max1, float max2) {
	return std::min(max1, max2) - std::max(min1, min2);
}

bool SAT_static(Polygon &polygon1, Polygon &polygon2, bool hardPush) {

	//for static resolution with need to know the 
	//axis with the smallest penetration
	float smallestPen = std::numeric_limits<float>::max();
	sf::Vector2f smallestAxis;


	//Similar to SAT
	int p1Lados = polygon1.getNmberEdges();
	int p2Lados = polygon2.getNmberEdges();
	sf::Vector2f* p1Points = new sf::Vector2f[p1Lados];
	sf::Vector2f* p2Points = new sf::Vector2f[p2Lados];
	for (int i = 0; i < p1Lados; i++) {
		p1Points[i] = polygon1.getPoint(i);
	}
	for (int i = 0; i < p2Lados; i++) {
		p2Points[i] = polygon2.getPoint(i);
	}


	//for fisrt polygon
	for (unsigned int firstPoint = 0; firstPoint < p1Lados; firstPoint++) {
		unsigned int secondPoint = (firstPoint + 1) % p1Lados;

		//Consider a perpendicular axis
		sf::Vector2f projAxis(-(p1Points[firstPoint].y - p1Points[secondPoint].y), p1Points[firstPoint].x - p1Points[secondPoint].x);

		//Find the min and max of the projected "shadow"
		float min1 = INFINITY;
		float max1 = -INFINITY;
		for (int pointNumber = 0; pointNumber < p1Lados; pointNumber++) {

			float projectedPoint = (p1Points[pointNumber].x * projAxis.x) + (p1Points[pointNumber].y * projAxis.y);
			min1 = std::min(min1, projectedPoint);
			max1 = std::max(max1, projectedPoint);
		}

		//Find the min and max of the projected "shadow"
		float min2 = INFINITY;
		float max2 = -INFINITY;
		for (int pointNumber = 0; pointNumber < p2Lados; pointNumber++) {

			float projectedPoint = (p2Points[pointNumber].x * projAxis.x) + (p2Points[pointNumber].y * projAxis.y);
			min2 = std::min(min2, projectedPoint);
			max2 = std::max(max2, projectedPoint);
		}

		//if overlap occur
		if (min1 <= max2 && max1 >= min2) {

			if (minDist(min1, min2, max1, max2) < smallestPen) {
				smallestPen = minDist(min1, min2, max1, max2);
				smallestAxis = projAxis;
			}
		} else {
			return false;
		}
	}
	
	//for second polygon
	for (unsigned int firstPoint = 0; firstPoint < p2Lados; firstPoint++) {
		int secondPoint = (firstPoint + 1) % p2Lados;

		//Consider a perpendicular axis
		sf::Vector2f projAxis(-(p2Points[firstPoint].y - p2Points[secondPoint].y), p2Points[firstPoint].x - p2Points[secondPoint].x);

		//Find the min and max of the projected "shadow"
		float min1 = INFINITY;
		float max1 = -INFINITY;
		for (int pointNumber = 0; pointNumber < p1Lados; pointNumber++) {

			float projectedPoint = (p1Points[pointNumber].x * projAxis.x) + (p1Points[pointNumber].y * projAxis.y);
			min1 = std::min(min1, projectedPoint);
			max1 = std::max(max1, projectedPoint);
		}

		//Find the min and max of the projected "shadow"
		float min2 = INFINITY;
		float max2 = -INFINITY;
		for (int pointNumber = 0; pointNumber < p2Lados; pointNumber++) {

			float projectedPoint = (p2Points[pointNumber].x * projAxis.x) + (p2Points[pointNumber].y * projAxis.y);
			min2 = std::min(min2, projectedPoint);
			max2 = std::max(max2, projectedPoint);
		}

		//if overlap occur
		if (min1 <= max2 && max1 >= min2) {
			
			if (minDist(min1, min2, max1, max2) < smallestPen) {
				smallestPen = minDist(min1, min2, max1, max2);
				smallestAxis = projAxis;
			}
		} else {
			return false;
		}
	}

	/* If we get here, that means that we can't separate the polygons on any axis, therefore, they are intersecting

		Now we just need to calculate the MTV minimun translation vector, using the smallest axis as MTV' direction and
		smallest penetration as MTV' magnitude
	*/
	float vectorLenght = std::sqrt(smallestAxis.x * smallestAxis.x + smallestAxis.y * smallestAxis.y);
	sf::Vector2f unitVector(smallestAxis.x / vectorLenght, smallestAxis.y / vectorLenght);
	sf::Vector2f pushResolution = unitVector * (smallestPen / 100.0f);

	if (hardPush) {

		sf::Vector2f direction(polygon1.getPosition().x - polygon2.getPosition().x, polygon1.getPosition().y - polygon2.getPosition().y);

		if (pushResolution.x * direction.x + pushResolution.y * direction.y < 0) {
			pushResolution.x = -pushResolution.x;
			pushResolution.y = -pushResolution.y;
		}

		polygon1.move(pushResolution);
	} else {
		sf::Vector2f direction(polygon2.getPosition().x - polygon1.getPosition().x, polygon2.getPosition().y - polygon1.getPosition().y);

		if (pushResolution.x * direction.x + pushResolution.y * direction.y < 0) {
			pushResolution.x = -pushResolution.x;
			pushResolution.y = -pushResolution.y;
		}

		polygon2.move(pushResolution);
	}
	return true;
}