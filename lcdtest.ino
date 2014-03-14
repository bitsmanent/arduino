#include <LiquidCrystal.h>

#define LCD_PIN_CONTRAST	6
#define LCD_PIN_BACKLIGHT 	9

#define	VALUE_MAX		255
#define	VALUE_INC		5

#define PROMPT			"> "

static int contrast = 110;
static int backlight = 90;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void
handleserial(void) {
	switch(Serial.read()) {
		case 'C':
			if((contrast += VALUE_INC) > VALUE_MAX)
				contrast = VALUE_MAX;
			analogWrite(LCD_PIN_CONTRAST, contrast);
			Serial.print("Contrast: ");
			Serial.println(contrast);
			break;
		case 'c':
			if((contrast -= VALUE_INC) < 0)
				contrast = 0;
			analogWrite(LCD_PIN_CONTRAST, contrast);
			Serial.print("Contrast: ");
			Serial.println(contrast);
			break;
		case 'B':
			if((backlight += VALUE_INC) > VALUE_MAX)
				backlight = VALUE_MAX;
			analogWrite(LCD_PIN_BACKLIGHT, backlight);
			Serial.print("Backlight: ");
			Serial.println(backlight);
			break;
		case 'b':
			if((backlight -= VALUE_INC) < 0)
				backlight = 0;
			analogWrite(LCD_PIN_BACKLIGHT, backlight);
			Serial.print("Backlight: ");
			Serial.println(backlight);
			break;
		case 'R':
			Serial.println("Writing \"HELLO WORLD!\" to the LCD display.");
			lcd.clear();
			lcd.print("HELLO WORLD!");
			break;
		case 'r':
			Serial.println("Writing \"hello world!\" to the LCD display.");
			lcd.clear();
			lcd.print("hello world!");
			break;
		case 'L':
			lcd.blink();
			Serial.println("Blink ON");
			break;
		case 'l':
			lcd.noBlink();
			Serial.println("Blink OFF");
			break;

		default:
			Serial.println("Unknown command");
			break;
	}

	Serial.print(PROMPT);
}

void
setup(void) {
	Serial.begin(9600);
	lcd.begin(16, 2); /* 16x2 display */
	lcd.write("Hello LCD! ;-)");
	analogWrite(LCD_PIN_CONTRAST, contrast);
	analogWrite(LCD_PIN_BACKLIGHT, backlight);
	Serial.print(PROMPT);
}

void
loop(void) {
	if(Serial.available() > 0)
		handleserial();
}
