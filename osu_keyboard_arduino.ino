
/*
	Micro, Leonardo, other 32u4-based
	Digital Pins Usable For Interrupts: 0, 1, 2, 3, 7
	Digital Pins Usable For PWM: 3, 5, 6, 9, 10, 11
*/

#include "Keyboard.h"
#define KEY_X (2)
#define KEY_Z (3)
#define KEY_RETRY (7)

#define LED_X (9)
#define LED_Z (10)
#define LED_STATU (16)

enum {PRESSED, RELEASED} z_statu = RELEASED, x_statu = RELEASED, retry_statu = RELEASED;

void setup()
{
	Serial.begin(115200);
	Serial.print("Yes,my lord.\n");
	pinMode(LED_Z, OUTPUT);
	pinMode(LED_X, OUTPUT);
	pinMode(LED_STATU, OUTPUT);
	pinMode(A3, OUTPUT);
	digitalWrite(A3,1);
	digitalWrite(LED_STATU,1);
	analogWrite(LED_Z, 50);
	analogWrite(LED_X, 50);
	pinMode(KEY_Z, INPUT_PULLUP);
	pinMode(KEY_X, INPUT_PULLUP);
	pinMode(KEY_RETRY, INPUT_PULLUP);
}

void enableKeyboard(void)
{
	Serial.print("Keyboard ready.\n");
	attachInterrupt(digitalPinToInterrupt(KEY_Z), z_press, FALLING);
	attachInterrupt(digitalPinToInterrupt(KEY_X), x_press, FALLING);
	Keyboard.begin();
	interrupts();
	digitalWrite(LED_STATU,0);
}

void wait_2key_press(void)
{
	unsigned char _=0;
	while (digitalRead(KEY_Z) || digitalRead(KEY_X)){
		digitalWrite(LED_STATU,_);_=!_;
		delay(334);
	}
	Serial.print("2key_press\n");
}

void wait_2key_release(void)
{
	unsigned char _=0;
	while (!digitalRead(KEY_Z) || !digitalRead(KEY_X)){
		digitalWrite(LED_STATU,_);_=!_;
		delay(155);
	}
	Serial.print("2key_release\n");
}

void loop()
{
	wait_2key_press();
	wait_2key_release();
	enableKeyboard();
	while (1) {
		if (z_statu == PRESSED) {
			if(digitalRead(KEY_Z)){
				z_statu = RELEASED;
				Keyboard.release('z');
			}
		}
		if (x_statu == PRESSED) {
			if(digitalRead(KEY_X)){
				x_statu = RELEASED;
				Keyboard.release('x');
			}
		}
		if(retry_statu == RELEASED){
			if(!digitalRead(KEY_RETRY)){
				Keyboard.press('`');
			}
		} else {
			if(digitalRead(KEY_RETRY)){
				Keyboard.release('`');
			}
		}
		delay(1);
	}
}

void z_press(void)
{
	Keyboard.press('z');
	z_statu = PRESSED;
}

void x_press(void)
{
	Keyboard.press('x');
	x_statu = PRESSED;
}

