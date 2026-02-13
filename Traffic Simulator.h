#pragma once

#include "resource.h"

class TrafficLight {
public:
	int m_index;
	int m_id;
	int m_x1, m_y1, m_x2, m_y2, m_x3, m_y3;
public:
	TrafficLight(int index, int id, int x1, int y1, int x2, int y2, int x3, int y3) : m_index(index), m_id(id), m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_x3(x3), m_y3(y3) {}
};