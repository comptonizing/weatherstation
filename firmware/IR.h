#include <Adafruit_MLX90614.h>

class IR {
	public:
		static IR &i();
		float skyTemp();
		float ambientTemp();
	private:
		IR();
		~IR();
		IR(const IR&);
		IR& operator=(const IR&);

		Adafruit_MLX90614 *m_mlx;
};
