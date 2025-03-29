#include "IR.h"

IR &IR::i() {
	static IR theInstance;
	return theInstance;
}

IR::IR() {
	m_mlx = new Adafruit_MLX90614;
	m_mlx->begin();
}

IR::~IR() {
}

float IR::skyTemp() {
	return m_mlx->readObjectTempC();
}

float IR::ambientTemp() {
	return m_mlx->readAmbientTempC();
}
