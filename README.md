# rotary_encoder_led
Arduino code for controlling LED brightness and blink rate using a single rotary encoder and an ESP32.

This repository contains Arduino code used for controlling the intensity of a 12 V LED strip using a potentiometer with a ESP32 `potentiometer_control_led.ino` and for controlling LED brightness and blink rate using a single rotary encoder `rotary_control_led.ino`. A video showing the project can be found on [Youtube](https://youtu.be/zEA_s_bB1kk)

The 12 V LED power supply is used to power the ESP32 via a LM7805 12 V to 5 V  converter.  The 3.3 V PWM from ESP32 is used to control the 12 V  LED intensity using a IRF640N MOSFET to convert the 3.3 V digital signal to a 12 V signal. The duty cycle of PWM (and therefore LED brightness) is calculated by scaling a 0 to 3.3 V input voltage, set by a potentiometer connected to an analog input pin. The blink interval is fixed and has to be changed by changing the Arduino code.

The second part of the video shows how the potentiometer is replaced by a rotary encoder. The button on the encoder toggles between changing the light intensity/PWM duty cycle or the blink interval. A OLED display was also added to show the % of maximum LED intensity and the interval between blinks in ms. The rotary encoder was connected to a different ESP32 for only practical reasons (it was easier to wire up a new PCB). The PCB used for the potentiometer-based set up was used because the LM7805 and IRF640N were already attached. 

The video does not clearly show how the intensity of the LED changes, so a light sensor was used and the data plotted in real time using the Arduino. 

All code is available on my GitHub page.

## Wiring diagram
This diagram shows the wiring of the MOSFET, the LM7805 and and LED and potentiometer. For the rotary encoder, the potentimeter was replaced with a encoder and an OLED display added (using a separate PCB/ESP32). Wiring not shown. 
![led_potentiator](https://github.com/damianjwilliams/rotary_encoder_led/assets/18335331/944aa6c2-b80e-4f73-ac20-4f8af39d71f0)

