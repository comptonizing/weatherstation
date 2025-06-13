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
	Serial.print("Taking IR sky ... ");
	auto ret = m_mlx->readObjectTempC();
	Serial.println("done");
	return ret;
}

float IR::ambientTemp() {
	Serial.print("Taking IR sky ... ");
	auto ret = m_mlx->readAmbientTempC();
	Serial.println("done");
	return ret;
}
