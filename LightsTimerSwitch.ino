
// Ligths duration in ms.
const unsigned long normal_lights_timeout = 90000;
const unsigned long thiago_lights_timeout = 300000;
const unsigned long permanent_lights_timeout = 3600000;


// Button must me kept pressed until time specified is reached
const short normal_button_time = 500;
const short thiago_button_time = 3000;

unsigned long number_of_milliseconds_button_pressed = 0;

const byte button_pin = 12;
const byte lights_pin = 9;


void setup() {

	pinMode(button_pin, INPUT);
	pinMode(lights_pin, OUTPUT);

	switch_lights_off();

	digitalWrite(lights_pin, LOW);

	Serial.begin(115200);
	Serial.println("Starting serial port");
	Serial.flush();

	delay(100);
}

void start_lights_timer(unsigned long timeout) {
	unsigned long milliseconds_lights = 0;
	unsigned long initial_time = millis();

	do {
		milliseconds_lights = millis() - initial_time;

		if (is_button_pressed() || milliseconds_lights > timeout) {
			switch_lights_off();
			timeout = 0;
		}
	} while (are_lights_on());
}

void switch_lights_off() {
	digitalWrite(lights_pin, LOW);
	// after switching off the lights, ignore all events. Just wait for 1 second.
	delay(1000);
}

void switch_lights_on(unsigned long timeout) {
	digitalWrite(lights_pin, HIGH);
	start_lights_timer(timeout);
}

bool are_lights_on() {
	return digitalRead(lights_pin);
}

bool is_button_pressed() {
	bool pressed = digitalRead(button_pin);
	//This delay avoids bounces
	delay(50);
	return pressed;
}

unsigned long count_number_of_milliseconds_that_button_is_pressed() {
	unsigned long milliseconds_pressed = 0;
	unsigned long initial_time = millis();

	while (is_button_pressed()) {
		milliseconds_pressed = millis() - initial_time;
	}

	return milliseconds_pressed;
}

/* main loop is always polling for button pressed. When a button pressed is detected, then lights are switched on.
 * After that, depending on how many milliseconds the button is kept pressed, a different duration for lights is set.
 * Notice that permanent lights timeout is set to 1h, which is in my case kind of 'permanent'. Modify the time values according to your needs.
 */
void loop() {

	if (is_button_pressed()) {
		// First, switch on lights, then decide
		digitalWrite(lights_pin, HIGH);

		number_of_milliseconds_button_pressed = count_number_of_milliseconds_that_button_is_pressed();

		if (number_of_milliseconds_button_pressed <= normal_button_time) {
			switch_lights_on(normal_lights_timeout);
		} else if (number_of_milliseconds_button_pressed > normal_button_time && number_of_milliseconds_button_pressed < thiago_button_time) {
			switch_lights_on(thiago_lights_timeout);
		} else if (thiago_button_time >= thiago_button_time) {
			switch_lights_on(permanent_lights_timeout);
		}

	}
}

