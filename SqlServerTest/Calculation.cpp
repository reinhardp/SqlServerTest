#include "Calculation.h"

Calculation::Calculation(void) {
	m_a = 0.00;
	m_b = 0.00;
}

Calculation::~Calculation(void) {
	m_a = 0.00;
	m_b = 0.00;
}

void Calculation::setMember(double a, double b) {
	m_a = a;
	m_b = b;
}

double Calculation::add(double a, double b) {
	return a + b;
}

double Calculation::substract(double a, double b) {
	return a - b;
}


double Calculation::devide(double a, double b) {
	return a / b;
}

double Calculation::multiply(double a, double b) {
	return a * b;
}