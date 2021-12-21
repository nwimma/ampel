#include "pico/stdlib.h"
#define SLEEP_TIME 4000
void setupPin(uint);

int main() {
	stdio_init_all();
	const uint ONBOARD_PIN = PICO_DEFAULT_LED_PIN;
	setupPin(ONBOARD_PIN);
	gpio_put(ONBOARD_PIN, 1);

	const uint GREEN_PIN = 9;
	const uint YELLOW_PIN = 11;
	const uint RED_PIN = 13;

	setupPin(GREEN_PIN);
	setupPin(YELLOW_PIN);
	setupPin(RED_PIN);

	while (1) {
		gpio_put(RED_PIN, 1);
		sleep_ms(SLEEP_TIME);
		gpio_put(RED_PIN, 0);

		gpio_put(YELLOW_PIN, 1);
		sleep_ms(SLEEP_TIME / 2);
		gpio_put(YELLOW_PIN, 0);

		gpio_put(GREEN_PIN, 1);
		sleep_ms(SLEEP_TIME);
		gpio_put(GREEN_PIN, 0);

		for(int i = 0; i < 4; i++) {
			sleep_ms(SLEEP_TIME / 4);
			gpio_put(GREEN_PIN, 1);
			sleep_ms(SLEEP_TIME / 4);
			gpio_put(GREEN_PIN, 0);
		}

		gpio_put(YELLOW_PIN, 1);
		sleep_ms(SLEEP_TIME / 2);
		gpio_put(YELLOW_PIN, 0);
	}
}

void setupPin(uint pin) {
	gpio_init(pin);
	gpio_set_dir(pin, GPIO_OUT);
}
