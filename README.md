# ECPE 172 - Microcontrollers - Final Project

The microcontroller will be in control of an unlock signal (active low) which can be wired to an external mechanism, such as an
electronic deadbolt. For our project, we will be representing this signal with just an LED. The microcontroller will unlock the signal
depending on whether or not the user has pressed the correct 4-digit passcode on a keypad attached to the microcontroller. It will
relock the signal 5 seconds after unlocking it. This time delay will be controlled by one of the TM4C's onboard timers.

The 4-digts will be displayed on two 2-digit 7-segment displays, allowing it to indicate the 4 digits as they are typed in. The
microcontroller will check the passcode once all 4 digits have been entered. If the passcode is wrong, the user may try again after
a short delay.

If the user is holding down the "0" key by the time the microcontroller has finished waiting 5 seconds to lock back up, the
password will be cleared and the microcontroller will wait for a new 4-digit passcode to be specified.
