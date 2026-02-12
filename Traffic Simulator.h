#pragma once

#include "resource.h"

class TrafficLight {
public:
	COLORREF trafficLightColors[3] = { RGB(0,255,0), RGB(255,255,0), RGB(255,0,0) };
	int m_index;
	int m_x1, m_y1, m_x2, m_y2, m_x3, m_y3;

public:
	TrafficLight(int index, int x1, int y1, int x2, int y2, int x3, int y3) : m_index(index), m_x1(x1), m_y1(y1), m_x2(x2), m_x3(x3), m_y3(y3) {}
};