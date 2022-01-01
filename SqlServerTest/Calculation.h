#pragma once
class Calculation
{
private:
	double m_a;
	double m_b;
public:
	Calculation(void);
	virtual ~Calculation(void);
	void setMember(double a, double b);
	double add(double a, double b);
	double substract(double a, double b);
	double devide(double a, double b);
	double multiply(double a, double b);
};

