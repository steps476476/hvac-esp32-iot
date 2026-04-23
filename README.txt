# Industrial IoT Smart HVAC Controller (ESP32)

## Overview
This project implements a Smart HVAC Controller using ESP32 and Wokwi simulation. It demonstrates embedded systems design, RTOS multi-tasking, ADC integration, interrupt handling, and industrial safety logic.

## Features
- DHT22 temperature monitoring
- ADC input (potentiometer)
- Emergency interrupt (button)
- Overheat shutdown
- Buzzer alarm system
- FreeRTOS task-based architecture

## Hardware (Wokwi Components)
- ESP32 DevKit v1
- DHT22 Sensor
- Push Button
- LED (Relay simulation)
- Buzzer
- Potentiometer

## Pin Configuration
 Component  GPIO 
----------------
 DHT22     4    
 Button    18   
 LED       2    
 Buzzer    15   
 ADC       34   

## How to Run
1. Open Wokwi
2. Create ESP32 project
3. Replace files with diagram.json and sketch.ino
4. Start simulation

## Author
Stephen Oyetude