je m'apercois que je n'ai pas de schéma pour ma réalisation . il sera ajouter quand je l'aurais fait :-))

Pour info, le résultat du calcul fait par le code PID modifie la largeur de l'impulsion PWM ( c'est le principe !)
cette impulsion commande un mosfet qui coupe ou non la résistance du fer .
La valeur de cette résistance chauffante varie légérement avec la température et sert ainsi de capteur.

La puissance vient d'un transfo et d'un redressemt double alternance. La tension de puissance n'est pas
filtrée, seule l'alimentation  du arduino micro est filtrée puis régulée par un lm7805 
Deux boutons permettent de fixer la température à 100°c et à 200°c pour faire une fonction veille moins chaude.
le code  donne les connections pour le cablage, très simple.

Ce lien est très utile pour comprendre la régulation .


***************************************************************
* Arduino PID Library - Version 1.2.0
* by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
*
* This Library is licensed under the MIT License
***************************************************************

 - For an ultra-detailed explanation of why the code is the way it is, please visit: 
   http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

 - For function documentation see:  http://playground.arduino.cc/Code/PIDLibrary

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Chrono library for Arduino or Wiring
by Sofian Audry and Thomas Ouellet Fredericks

Chronometer/stopwatch library that counts the time passed since started.

## Repository

* Browse the source code here: http://github.com/SofaPirate/Chrono
* Download the latest version here : https://github.com/SofaPirate/Chrono/archive/master.zip

## Documentation

See the included "docs" folder or visit http://sofapirate.github.io/Chrono/

[![arduino-library-badge](http://www.ardu-badge.com/badge/Chrono.svg)](http://www.ardu-badge.com/Chrono)

XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
Arduino OneButton Library
===

This Arduino library is improving the usage of a singe button for input.
It shows how to use an digital input pin with a single pushbutton attached
for detecting some of the typical button press events like single clicks, double clicks and long-time pressing.
This enables you to reuse the same button for multiple functions and lowers the hardware investments.

This is also a sample for implementing simple finite-state machines by using the simple pattern above. 

You can find more details on this library at
http://www.mathertel.de/Arduino/OneButtonLibrary.aspx


## Getting Started

Clone this repository into `Arduino/Libraries` or use the built-in Arduino IDE Library manager to install
a copy of this library. You can find more detail about installing libraries 
[here, on Arduino's website](https://www.arduino.cc/en/guide/libraries).

```CPP
#include <Arduino.h>
#include <OneButton.h>
```

Each physical button requires its own `OneButton` instance. You can initialize them like this:


### Initialize a Button to GND

```CPP
#define BUTTON_PIN 4

/**
 * Initialize a new OneButton instance for a button
 * connected to digital pin 4 and GND, which is active low
 * and uses the internal pull-up resistor.
 */

OneButton btn = OneButton(
  BUTTON_PIN, // Input pin for the button
  LOW,       // Button is active LOW
  true       // Enable internal pull-up resistor
);
```


### Initialize a Button to VCC

```CPP
#define BUTTON_PIN 4

/**
 * Initialize a new OneButton instance for a button
 * connected to digital pin 4, which is active high.
 * As this does not use any internal resistor
 * an external resistor (4.7k) may be required to create a LOW signal when the button is not pressed.
 */

OneButton btn = OneButton(
  BUTTON_PIN, // Input pin for the button
  HIGH,       // Button is active high
  false       // Disable internal pull-up resistor
);
```


### Attach State Events

Once you have your button initialized, you can handle events by attaching them to the button
instance. Events can either be static functions or lambdas (without captured variables).

```CPP
// Handler function for a single click:
static void handleClick() {
  Serial.println("Clicked!");
}

// Single Click event attachment
btn.attachClick(handleClick);

// Double Click event attachment with lambda
btn.attachDoubleClick([]() {
  Serial.println("Double Press!");
});
```


### Don't forget to `tick()`!

In order for `OneButton` to work correctly, you must call `tick()` on __each button instance__
within your main `loop()`. If you're not getting any button events, this is probably why.

```CPP
void loop() {
  btn.tick();

  // Do other things...
}
```


## State Events

Here's a full list of events handled by this library:

| Attach Function         | Description                                            |
| ----------------------- | ------------------------------------------------------ |
| `attachClick`           | Fires as soon as a single click is detected.           |
| `attachDoubleClick`     | Fires as soon as a double click is detected.           |
| `attachPressStart`      | Fires as soon as the button is pressed down.           |
| `attachLongPressStart`  | Fires as soon as the button is held down for 1 second. |
| `attachDuringLongPress` | Fires periodically as long as the button is held down. |
| `attachLongPressStop`   | Fires when the button is released after a long hold.   |


### Event Timing

Valid events occur when `tick()` is called after a specified number of milliseconds. You can use
the following functions to change the timing.

**Note:** Attaching a double click will increase the delay for detecting a single click. If a double
click event is not attached, the library will assume a valid single click after one click duration,
otherwise it must wait for the double click timeout to pass.

| Function                | Default (ms) | Description                                                   |
| ----------------------- | ------------ | ------------------------------------------------------------- |
| `setDebounceTicks(int)` | `50`         | Period of time in which to ignore additional level changes.   |
| `setClickTicks(int)`    | `600`        | Timeout used to distinguish single clicks from double clicks. |
| `setPressTicks(int)`    | `1000`       | Duration to hold a button to trigger a long press.            |


### Additional Functions

`OneButton` also provides a couple additional functions to use for querying button status:

| Function                | Description                                                                    |
| ----------------------- | ------------------------------------------------------------------------------ |
| `bool isLongPressed()`  | Detect whether or not the button is currently inside a long press.             |
| `int getPressedTicks()` | Get the current number of milliseconds that the button has been held down for. |


### `tick()` and `reset()`

You can specify a logic level when calling `tick(bool)`, which will skip reading the pin and use
that level instead. If you wish to reset the internal state of your buttons, call `reset()`.


## Troubleshooting

If your buttons aren't acting they way they should, check these items:

1. Check your wiring and pin numbers.
2. Did you call `tick()` on each button instance in your loop?
3. Did you alter your clock timers in any way without adjusting ticks?
   
