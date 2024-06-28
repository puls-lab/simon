/*
Copyright (C) 2024 Martin Saraceno

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>. 
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define NUMLEDS 4
#define MAXCOLS 128
#define DEBOUNCE_MS 44


// declartions
// classes
// LB
class LB {
private:
    
public:
    volatile uint8_t* dd;
    volatile uint8_t* port;
    volatile uint8_t* pin;
    uint8_t ddid;
    uint8_t portid;
    uint8_t pinid;

    LB();
    LB(volatile uint8_t* dd, uint8_t ddid,
       volatile uint8_t* port, uint8_t portid,
       volatile uint8_t* pin, uint8_t pinid);
    virtual void enable() = 0;
    bool get_state();
};

// LED
class LED : public LB {
private:

public:
    LED();
    LED(volatile uint8_t* dd, uint8_t ddid,
        volatile uint8_t* port, uint8_t portid,
        volatile uint8_t* pin, uint8_t pinid);
    void enable();
    void turn_on();
    void turn_off();
};

// Button
class Button : public LB {
private:
    
public:
    Button();
    Button(volatile uint8_t* dd, uint8_t ddid,
           volatile uint8_t* port, uint8_t portid,
           volatile uint8_t* pin, uint8_t pinid);
    void enable();
};

// functions
uint8_t random_number();
uint8_t wait_input(Button * buttons);
void led_score(LED * leds, uint8_t score);
void game_over(LED * leds, uint8_t score);


// definitions
//classes
// LB
LB::LB() {}

LB::LB(volatile uint8_t* dd, uint8_t ddid,
       volatile uint8_t* port, uint8_t portid,
       volatile uint8_t* pin, uint8_t pinid) : dd(dd), ddid(ddid),
                                               port(port), portid(portid),
                                               pin(pin), pinid(pinid) {}

bool LB::get_state() {
    return !((*pin) & (1<<pinid));
}

//LED
LED::LED() {}

LED::LED(volatile uint8_t* dd, uint8_t ddid,
         volatile uint8_t* port, uint8_t portid,
         volatile uint8_t* pin, uint8_t pinid) : LB(dd, ddid,
                                                    port, portid,
                                                    pin, pinid) {
    enable();
    turn_on();
}

void LED::enable () {
    *dd |= 1<<ddid;
}

void LED::turn_on() {
    *port |= 1<<portid;
}

void LED::turn_off() {
    *port &= ~(1<<portid);
}

// Button
Button::Button() {}

Button::Button (volatile uint8_t* dd, uint8_t ddid,
                volatile uint8_t* port, uint8_t portid,
                volatile uint8_t* pin, uint8_t pinid) : LB(dd, ddid,
                                                           port, portid,
                                                           pin, pinid) {
    enable();
}

void Button::enable () {
    *dd &= ~(1<<ddid);
    *port |= 1<<portid;
}

// functions
uint8_t random_number() {
    return rand()%NUMLEDS;
}

uint8_t wait_input(Button * buttons) {
    uint8_t col;
    bool exit = false;
    while (1) {
        for (uint8_t i=0; i<NUMLEDS; ++i) {
            if (buttons[i].get_state()) {
                _delay_ms(DEBOUNCE_MS);
                col = i;
                exit = true;
                break;
            }
        }
        if (exit) {
            break;
        }
    }
    return col;
}

void led_score(LED * leds, uint8_t score) {
    uint8_t s10 = score/10;
    uint8_t s1 = score - 10*s10;
    for (uint8_t n=0; n<s10; ++n) {
        leds[NUMLEDS-2].turn_on();
        _delay_ms(200);
        leds[NUMLEDS-2].turn_off();
        _delay_ms(200);
    }
    for (uint8_t n=0; n<s1; ++n) {
        leds[NUMLEDS-1].turn_on();
        _delay_ms(200);
        leds[NUMLEDS-1].turn_off();
        _delay_ms(200);
    }
}

void game_over(LED * leds, uint8_t score) {
    for (uint8_t n=0; n<10; ++n) {
        for (uint8_t i=0; i<NUMLEDS; ++i) {
            leds[i].turn_on();
        }
        _delay_ms(100);
        for (uint8_t i=0; i<NUMLEDS; ++i) {
            leds[i].turn_off();
        }
        _delay_ms(100);
    }
    _delay_ms(500);
    led_score(leds, score);
    while (1) {}
}


// main
int main(void) {
    uint16_t seed = 0;
    bool exit = false;
    uint8_t col;
    uint8_t score = 0;
    
    // set LEDs
    LED leds[NUMLEDS];
    leds[0] = LED(&DDRC, DDRC5,
                 &PORTC, PORTC5,
                 &PINC, PINC5);
    leds[1] = LED(&DDRD, DDRD0,
                  &PORTD, PORTD0,
                  &PIND, PIND0);
    leds[2] = LED(&DDRD, DDRD1,
                  &PORTD, PORTD1,
                  &PIND, PIND1);
    leds[3] = LED(&DDRD, DDRD2,
                  &PORTD, PORTD2,
                  &PIND, PIND2);

    // set buttons
    Button buttons[NUMLEDS];
    buttons[0] = Button(&DDRD, DDRD3,
                        &PORTD, PORTD3,
                        &PIND, PIND3);
    buttons[1] = Button(&DDRD, DDRD4,
                        &PORTD, PORTD4,
                        &PIND, PIND4);
    buttons[2] = Button(&DDRE, DDRE0,
                       &PORTE, PORTE0,
                       &PINE, PINE0);
    buttons[3] = Button(&DDRE, DDRE1,
                        &PORTE, PORTE1,
                        &PINE, PINE1);

    // set random seed by user
    while(1) {
        for (uint8_t i=0; i<NUMLEDS; ++i) {
            exit |= buttons[i].get_state();
        }
        if (exit) {
            break;
        }
        seed += 1;
    }
    srand(seed);

    // turn off all LEDs
    for (uint8_t i=0; i<NUMLEDS; ++i) {
        leds[i].turn_off();
    }
    _delay_ms(1000);
    
    // make initial color array
    uint8_t sol[MAXCOLS];
    for (uint8_t i=0; i<MAXCOLS; ++i) {
        sol[i] = random_number();
    }
    
    while(1) {
        // show colors
        for (uint8_t i=0; i<score+1; ++i) {
            leds[sol[i]].turn_on();
            _delay_ms(500);
            leds[sol[i]].turn_off();
            _delay_ms(100);
        }
        // check colors
        for (uint8_t i=0; i<score+1; ++i) {
            col = wait_input(buttons);
            if (col == sol[i]) {
                leds[col].turn_on();
                _delay_ms(100);
                leds[col].turn_off();
                _delay_ms(100);
            } else {
                game_over(leds, score);
            }
        }
        _delay_ms(500);
        score++;
    }
    return 0;
}
