#include <LiquidCrystal.h>

#define LCD_PIN_CONTRAST	6
#define LCD_PIN_BACKLIGHT 	9

#define CONTRAST_MAX		255
#define CONTRAST_INC		5
#define BACKLIGHT_MAX		28836
#define BACKLIGHT_INC		25

#define PROMPT			"> "

static int contrast = 0;
static int backlight = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void
handleserial(void) {
	switch(Serial.read()) {
		case 'C':
			if((contrast += CONTRAST_INC) > CONTRAST_MAX)
				contrast = CONTRAST_MAX;
			analogWrite(LCD_PIN_CONTRAST, contrast);
			Serial.print("Contrast: ");
			Serial.println(contrast);
			break;
		case 'c':
			if((contrast -= CONTRAST_INC) < 0)
				contrast = 0;
			analogWrite(LCD_PIN_CONTRAST, contrast);
			Serial.print("Contrast: ");
			Serial.println(contrast);
			break;
		case 'B':
			if((backlight += BACKLIGHT_INC) > BACKLIGHT_MAX)
				backlight = BACKLIGHT_MAX;
			analogWrite(LCD_PIN_BACKLIGHT, contrast);
			Serial.print("Backlight: ");
			Serial.println(backlight);
			break;
		case 'b':
			if((backlight -= BACKLIGHT_INC) < 0)
				backlight = 0;
			analogWrite(LCD_PIN_BACKLIGHT, contrast);
			Serial.print("Backlight: ");
			Serial.println(backlight);
			break;
		case 'R':
			Serial.println("Writing \"HELLO WORLD!\" to the LCD display.");
			lcd.print("HELLO WORLD!");
			break;
		case 'r':
			Serial.println("Writing \"hello world!\" to the LCD display.");
			lcd.print("hello world!");
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
	lcd.blink();
	Serial.print(PROMPT);
}

void
loop(void) {
	if(Serial.available() > 0)
		handleserial();
	lcd.clear();
	lcd.print("Hello World!");
}
