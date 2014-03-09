/*
 * You know what it does?
 *
 * Requirements:
 * -------------
 *
 * - An Arduino board
 * - A dangling wire plugged on pin 9
*/

void
setup(void) {
	Serial.begin(9600);
	pinMode(9, INPUT);
}

void
loop(void) {
	int state = analogRead(9);
	if(state > 500) {
		Serial.print("[");
		Serial.print(micros());
		Serial.print("] ");
		Serial.println("Someone is moving around.");
	}
}
