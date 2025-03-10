It would be well to include in some algebras library a class to check if a matrix represents a base of a vectorial space.
With that, it would be easy to create a representation system class, allowing to configurate the impression buffer to use the International System,
the British one or any other. 

At this moment, only the International System is allowed.

The names of classes (value, value::magnitude and value::constant) should be improved too:
    - The value is anything that can be used in physics (in most of cases, physical variables)
    - The magnitude is really a measuring unit, but it is not visible outside the class.
    - The constant is just a wrapper for a "const double".
    - The basic measuring units are defined as values (and not constants, this should be fixed).

In addition, the use of the angle as a basic magnitude could be improved in the following ways:
    - The constant 'pi' (3.14159...) should be include (either in positive or negative) the radian as a unit.
    - With that, it should be prevented for the rest of constants to include (or not) those radians.
