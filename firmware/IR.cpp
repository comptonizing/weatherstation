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
	sendLog("Taking IR sky ... ");
	auto ret = m_mlx->readObjectTempC();
	sendLog("done");
	return ret;
}

float IR::ambientTemp() {
	sendLog("Taking IR sky ... ");
	auto ret = m_mlx->readAmbientTempC();
	sendLog("done");
	return ret;
}
