#include <LiquidCrystal.h>

#define LENGTH(X)               (sizeof X / sizeof X[0])
#define LCD_PIN_CONTRAST	6
#define LCD_PIN_BACKLIGHT 	9

#define CONTRAST_DEF		110
#define BACKLIGHT_DEF		90
#define BLINK_DEF		0
#define AUTOSCROLL_DEF		0
#define SCROLL_DELAY		250

#define	VALUE_MAX		255
#define	VALUE_INC		5
#define PS1			"> "
#define PS2			": "

enum { ModeInsert, ModeCommand }; /* modes */

static int contrast = CONTRAST_DEF;
static int backlight = BACKLIGHT_DEF;
static int blink;
static int autoscroll;
static int lcdstate = 1; /* on, off */
static char buf[1024]; /* command buffer */
static char serialC;

static unsigned int Mode = ModeInsert;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void
handleserial(void) {
	serialC = Serial.read();
	if(Mode == ModeInsert) {
		switch(serialC) {
			case 127: // Backspace
				buf[ strlen(buf) - 1 ] = '\0';
				Serial.write("\b \b");
				break;

			case 13: // Return
				Mode = ModeCommand;
				if(*buf) {
					lcd.clear();
					if(autoscroll) {
						lcd.setCursor(16, 0);
						for(int i = 0; i < LENGTH(buf) && buf[i]; ++i) {
							lcd.print(buf[i]);
							delay(SCROLL_DELAY);
						}
					}
					else {
						lcd.print(buf);
					}
					*buf = '\0';
				}
				Serial.println(); Serial.print(PS1); /* reset */
				break;

			case 27: // ESC
				Mode = ModeCommand;
				Serial.println(); Serial.print(PS1); /* reset */
				break;

			default:
				Serial.print(serialC);
				snprintf(buf, sizeof buf, "%s%c", buf, serialC);
				break;
		}

		return;
	}

	switch(serialC) {
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

		case 'l':
			if(blink) {
				lcd.noBlink();
				Serial.println("Blink is OFF");
			}
			else {
				lcd.blink();
				Serial.println("Blink is ON");
			}
			blink = !blink;
			break;

		case 'a':
			autoscroll = !autoscroll;
			if(autoscroll) {
				lcd.autoscroll();
				Serial.println("Autoscroll is ON");
			}
			else {
				lcd.noAutoscroll();
				Serial.println("Autoscroll is OFF");
			}
			break;

		case 'i':
			Mode = ModeInsert;
			Serial.write("\b\b");
			Serial.print(PS2);
			break;

		case 'd':
			lcdstate = !lcdstate;
			if(lcdstate) {
				lcd.display();
				Serial.println("Display is ON");
			}
			else {
				lcd.noDisplay();
				Serial.println("Display is OFF");
			}
			break;

		default:
			Serial.println((serialC != 13 ? "Unknown command" : ""));
			break;
	}

	if(Mode == ModeCommand)
		Serial.print(PS1);
}

void
setup(void) {
	Serial.begin(9600);
	lcd.begin(16, 2); /* 16x2 display */

	/* defaults */
	analogWrite(LCD_PIN_CONTRAST, contrast);
	analogWrite(LCD_PIN_BACKLIGHT, backlight);
	if(BLINK_DEF)
		lcd.blink();
	else
		lcd.noBlink();
	
	if(AUTOSCROLL_DEF)
		lcd.autoscroll();
	else
		lcd.noAutoscroll();
	
	if(lcdstate)
		lcd.display();
	else
		lcd.noDisplay();

	lcd.write("Hello!");
	Serial.print(PS1);
}

void
loop(void) {
	if(Serial.available() > 0) {
		handleserial();
	}
}
