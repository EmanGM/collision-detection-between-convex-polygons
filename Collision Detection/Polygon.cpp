#include "Polygon.h"


Polygon::Polygon(int numberSides, sf::FloatRect rect, std::vector<sf::Vector2f> points, sf::Color color) {

	lados = numberSides;
	polygon.setPointCount(lados);
	this->rect = rect;

	for (int i = 0; i < lados; i++) {
		polygon.setPoint(i, points[i]);
	}
	polygon.setOrigin(sf::Vector2f(this->rect.width / 2, this->rect.height / 2));
	polygon.setPosition(sf::Vector2f(this->rect.left + this->rect.width / 2, this->rect.top + this->rect.height / 2));
	polygon.setFillColor(color);

	//create square for AABB debug
	isBoxVisible = false;
	box.setOutlineThickness(-1.5f);
	box.setOutlineColor(sf::Color::White);
	box.setFillColor(sf::Color::Transparent);
	setBoundingBox();

	//create lines for SAT debug
	isNormalsVisible = false;
	lines.setPrimitiveType(sf::Lines);
	for (int i = 0; i < lados * 2; i++) {
		lines.append(sf::Vector2f(0.0f, 0.0f));
	}
	setNormalLines();
}


void Polygon::setBoundingBox() {
	rect = polygon.getGlobalBounds();
	box.setPosition(sf::Vector2f(this->rect.left, this->rect.top));
	box.setSize(sf::Vector2f(this->rect.width, this->rect.height));
}

void Polygon::setNormalLines() {

	sf::Vector2f center(rect.left + rect.width / 2, rect.top + rect.height / 2);
	for (int i = 0; i < lados; i++) {
		lines[2*i] = sf::Vertex(center);
		sf::Vector2f normalVector;
		normalVector.x = -(getPoint(i).y - getPoint(i + 1 < lados ? i + 1 : 0).y);
		normalVector.y = getPoint(i).x - getPoint(i + 1 < lados ? i + 1 : 0).x;
		float norm = sqrt(pow(normalVector.x, 2) + pow(normalVector.y, 2));
		lines[2*i+1] = sf::Vector2f(normalVector / norm * 80.0f + center);
	}
}


bool Polygon::checkMouseClick(sf::Vector2f mousePosition) {
	return this->rect.contains(int(mousePosition.x), int(mousePosition.y));
}


void Polygon::move(sf::Vector2f offset) {
	polygon.move(offset);
	box.move(offset);
	for (int i = 0; i < lines.getVertexCount(); i++) {
		lines[i].position += offset;
	}
	rect = polygon.getGlobalBounds();
}


void Polygon::rotate(float angle) {
	polygon.rotate(angle);
	setNormalLines();
	setBoundingBox();
}


sf::Vector2f Polygon::getPoint(int index) {
	sf::Vector2f ori = polygon.getOrigin();
	sf::Transform trans = polygon.getTransform();
	sf::Vector2f transP = polygon.getPoint(index);
	sf::Vector2f transformedPoint = trans.transformPoint(transP);
	return sf::Vector2f(transformedPoint.x + ori.x, transformedPoint.y + ori.y);
}

void Polygon::enableBox() {
	isBoxVisible = true;
	isNormalsVisible = false;
}

void Polygon::enableNormalLines() {
	isBoxVisible = false;
	isNormalsVisible = true;
}

void Polygon::disableDebugLines() {
	isBoxVisible = false;
	isNormalsVisible = false;
}

void Polygon::draw(sf::RenderWindow& window) {

	window.draw(polygon);
	if (isBoxVisible) 
		window.draw(box);
	if (isNormalsVisible)
		window.draw(lines);
}