# Software Development Protocol

## General principles
* Modularization
* Reusability

## Library for sensors
* Sensors must be represented by a struct in witch it's desirable data can be acessed directly.
* Sensors must be handled as a pointer to the structure.
* Function's return must be used for error handling.
* Functions must have the pointer(s) to the sensor(s) they are manipulating as the first argument(s)
