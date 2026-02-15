#pragma once

#include "resource.h"
#include <vector>

class TrafficLight {
public:
	int m_index;
	int m_id;
	int m_x1, m_y1, m_x2, m_y2, m_x3, m_y3;
public:
	TrafficLight(int index, int id, int x1, int y1, int x2, int y2, int x3, int y3) : m_index(index), m_id(id), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_x3(x3), m_y3(y3) {}
};

enum Direction { EAST = 0, SOUTH = 1};

struct Car {
	int x, y;
	Direction dir;
	COLORREF color;

	Car(int startX, int startY, Direction d) : x(startX), y(startY), dir(d) {

		color = RGB(rand() % 200, rand() % 200, rand() % 200);
	}
};