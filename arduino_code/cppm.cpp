/*
 * Combined-PPM signal generator
 *
 * Based on the code from:
 * https://quadmeup.com/generate-ppm-signal-with-arduino/
 * https://github.com/DzikuVx/ppm_encoder/blob/master/ppm_encoder_source.ino
 *
 * Copyright 2016 by Thomas Buck <xythobuz@xythobuz.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 */

#include <Arduino.h>
#include "cppm.h"

// #define DEBUG_OUTPUT Serial

CPPM* CPPM::inst = NULL;

void CPPM::init(void) {
#ifdef DEBUG_OUTPUT
    DEBUG_OUTPUT.println("Initializing Timer...");
#endif

    state = 1;
    currentChannel = channels;
    calcRest = 0;
    for (uint8_t i = 0; i < channels; i++) {
        data[i] = CHANNEL_DEFAULT_VALUE;
    }

    pinMode(output, OUTPUT);
    digitalWrite(output, CPPM::inst->onState ? LOW : HIGH);

    cli();
    TCCR1A = 0; // set entire TCCR1 register to 0
    TCCR1B = 0;
    OCR1A = 100; // compare match register
    TCCR1B |= (1 << WGM12); // turn on CTC mode
    TCCR1B |= (1 << CS11); // 8 prescaler: 0,5 microseconds at 16mhz
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
    sei();
}

void CPPM::setOutput(uint8_t i) {
    digitalWrite(output, LOW);
    pinMode(output, INPUT);
    output = i;
    pinMode(output, OUTPUT);
    digitalWrite(output, CPPM::inst->onState ? LOW : HIGH);
}

void CPPM::copy(uint16_t* d) {
#ifdef DEBUG_OUTPUT
    DEBUG_OUTPUT.println("New CPPM data!");
#endif

    cli();
    for (int i = 0; i < channels; i++) {
        data[i] = d[i];
    }
    sei();
}

ISR(TIMER1_COMPA_vect) {
    if (!CPPM::inst) {
        return;
    }

		// BB: if we're in ctc mode why do we clear this? isn't this done when the interrupt is fired?
    TCNT1 = 0;
    if (CPPM::inst->state) {
        // start pulse
        digitalWrite(CPPM::inst->output, CPPM::inst->onState ? HIGH : LOW);
        OCR1A = CPPM::inst->pulseLength << 1;
        CPPM::inst->state = 0;
    } else {
        // end pulse and calculate when to start the next pulse
        digitalWrite(CPPM::inst->output, CPPM::inst->onState ? LOW : HIGH);
        CPPM::inst->state = 1;
        if (CPPM::inst->currentChannel >= CPPM::inst->channels) {
            CPPM::inst->currentChannel = 0;
            CPPM::inst->calcRest += CPPM::inst->pulseLength;
            OCR1A = (CPPM::inst->frameLength - CPPM::inst->calcRest) << 1;
            CPPM::inst->calcRest = 0;
        } else {
					  // data[i] dictates pulse position
            OCR1A = (CPPM::inst->data[CPPM::inst->currentChannel] - CPPM::inst->pulseLength) << 1; // relative to current
            CPPM::inst->calcRest += CPPM::inst->data[CPPM::inst->currentChannel]; // relative to frame start
            CPPM::inst->currentChannel++;
        }
    }
}

