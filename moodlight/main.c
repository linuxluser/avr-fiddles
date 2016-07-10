

#define F_CPU 20000000   // 20 Mhz


#include <avr/io.h>
#include <util/delay.h>



// Define pins
#define RED_PIN    PB2
#define GRN_PIN    PB1
#define BLU_PIN    PB0
#define SW_PIN     PB3


// Define the delay time in ms
#define FLASH_DELAY_MS   70
#define FADE_DELAY_MS   100

// How dramatic should it jump from one color to the next
#define COLOR_FADE_SPEED 100

// How long to stay on one color before going to the next
#define COLOR_STAY_SPEED 10
#define DIM_TIME_US 100


// Useful constants
#define LOW 0x0
#define HIGH 0x1
#define INPUT 0x0
#define OUTPUT 0x1



void write(int pin, int value) {
    if (value == HIGH) {
        (PORTB |= (1 << pin));
    } else {
        (PORTB &= ~(1 << pin));
    }
}


void delay_microseconds(int us) {
    while (0 < us) {  
        _delay_us(1);
        --us;
    }
}


void lights_out(int microseconds) {
   int red_pin_state = PORTB & (1 << RED_PIN);
   int grn_pin_state = PORTB & (1 << GRN_PIN);
   int blu_pin_state = PORTB & (1 << BLU_PIN);
   write(RED_PIN, LOW);
   write(GRN_PIN, LOW);
   write(BLU_PIN, LOW);
   delay_microseconds(microseconds);
   write(RED_PIN, red_pin_state);
   write(GRN_PIN, grn_pin_state);
   write(BLU_PIN, blu_pin_state);
}


void fade_up(int pin) {
  int a,on,off,lights_out_time; 
  for (on = 1; on < COLOR_FADE_SPEED;  on++) {
    off = COLOR_FADE_SPEED - on;
    lights_out_time = DIM_TIME_US / COLOR_STAY_SPEED;
    for (a = 0; a < COLOR_STAY_SPEED; a++) {
      write(pin, HIGH);
      delay_microseconds(on);
      write(pin, LOW);
      delay_microseconds(off);
      //delay_microseconds(off - lights_out_time);
      //lights_out(lights_out_time);
    }
  }
  write(pin, HIGH);
}


void fade_down(int pin) {
  int n,on,off,lights_out_time; 
  for (off = 1; off < COLOR_FADE_SPEED; off++) {
    on = COLOR_FADE_SPEED - off;
    lights_out_time = DIM_TIME_US / COLOR_STAY_SPEED;
    for (n = 0; n < COLOR_STAY_SPEED; n++) {
      write(pin, HIGH);
      delay_microseconds(on);
      write(pin, LOW);
      delay_microseconds(off);
      //delay_microseconds(off - lights_out_time);
      //lights_out(lights_out_time);
    }
  }
  write(pin, LOW);
}


void red_to_yellow(void) {
  write(RED_PIN, HIGH);
  write(BLU_PIN, LOW);
  fade_up(GRN_PIN);
}


void yellow_to_green(void) {
  write(GRN_PIN, HIGH);
  write(BLU_PIN, LOW);
  fade_down(RED_PIN);
}


void greentocyan(void) {
  write(GRN_PIN, HIGH);
  write(RED_PIN, LOW);
  fade_up(BLU_PIN);
}


void cyan_to_blue(void) {
  write(BLU_PIN, HIGH);
  write(RED_PIN, LOW);
  fade_down(GRN_PIN);
}


void blue_to_magenta(void) {
  write(BLU_PIN, HIGH);
  write(GRN_PIN, LOW);
  fade_up(RED_PIN);
}


void magenta_to_red(void) {
  write(RED_PIN, HIGH);
  write(GRN_PIN, LOW);
  fade_down(BLU_PIN);
}



void setup(void) {
    // Set Data Direction Register (DDR) for port B
    DDRB = (1 << RED_PIN) | (1 << GRN_PIN) | (1 << BLU_PIN);
}


int main(void) {

    setup();

    for (;;) {
        red_to_yellow();
        yellow_to_green();
        greentocyan();
        cyan_to_blue();
        blue_to_magenta();
        magenta_to_red();
	delay_microseconds(COLOR_FADE_SPEED * COLOR_STAY_SPEED);
    }

    return 0;
}

