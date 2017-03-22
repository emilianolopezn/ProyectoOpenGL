#pragma once
#include "Modelo.h"

class Nave: public Modelo {
public:
	int vidas;
	bool herido;
	Nave();
};