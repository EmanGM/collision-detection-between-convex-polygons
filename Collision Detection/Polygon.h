#pragma once
#include <SFML/Graphics.hpp>


class Polygon {

private:

	int lados;
	sf::ConvexShape polygon;
	sf::FloatRect rect;
	sf::RectangleShape box;
	bool isBoxVisible;
	bool isNormalsVisible;
	sf::VertexArray lines;

	void setNormalLines();
	void setBoundingBox();


public:

	Polygon(int numberSides, sf::FloatRect rect, std::vector<sf::Vector2f> points, sf::Color color = sf::Color(255, 255, 255));
	bool checkMouseClick(sf::Vector2f mousePosition);
	void move(sf::Vector2f offset);
	void rotate(float angle);
	int getNmberEdges() {
		return lados;
	}
	sf::FloatRect getRect() {
		return rect;
	}
	sf::Vector2f getPoint(int index);
	sf::Vector2f getPosition() {
		return polygon.getPosition();
	}
	void enableBox();
	void enableNormalLines();
	void disableDebugLines();
	void draw(sf::RenderWindow &window);
};

