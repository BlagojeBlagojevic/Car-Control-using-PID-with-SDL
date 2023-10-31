# Car-Control-using-PID-with-SDL
This is a C code repository that demonstrates a simple car control system using the Proportional-Integral-Derivative (PID) control algorithm. The code also includes graphical representation using the Simple DirectMedia Layer (SDL) library

## Table of Contents
* Overview
* Prerequisites
* Code Structure
* How to Use
* License
## Overview
This code simulates a car's movement to reach a desired destination using PID control. 

The car's position is displayed graphically using SDL. 

The PID controller helps the car adjust its speed and direction to reach the desired destination accurately.

## Prerequisites

To run this code, you need to have the SDL library installed on your system. 

You can install it on Linux using the following command:

> sudo apt-get install libsdl2-dev

## Code Structure
The main code is written in C and is divided into several functions and structures:

* 'CAR' structure: Represents the car's properties and includes the desired destination, position, speed, error, and PID control parameters.

* 'GRAPH' structure: Stores data for plotting the car's speed, position, and error over time.

* 'Init_Car': Initializes the car's properties.

* 'Check_Boundary': Ensures the car stays within the screen boundaries.

* 'MainRenderrer': Renders the car and its desired destination using SDL.

* 'Limit_Speed_Acceleration': Limits the car's speed and acceleration.

* 'RandError': Introduces random error in the measurement process.

* 'UpdateCar': Updates the car's position and speed using PID control.

* 'RandDestination': Generates a random desired destination for the car.

* 'Input': Allows the user to enter PID control parameters.

* 'main': Initializes the SDL window and renderer, and runs the main simulation loop

## How to Use
1. Ensure you have the SDL library installed (as mentioned in the Prerequisites section).

2. Clone this repository to your local machine.

3. Compile the code using your preferred C compiler.

## License

MIT

