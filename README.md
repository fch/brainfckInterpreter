# brainfckInterpreter
Very basic interpreter

Feel free to give ideas, tips and constructive critique for improvement.

The underlying structure functions like a turing machine in a way.
The _Tape_ consists of multiple _Cells_, and points to one of them.
Each _Cell_ holds a value and has a reference to it's left and right neighbour.

BE CAREFUL! Theres no limit to the count of cells, so a infinite loop just going
into a direction, like this little guy:
```
+[>+]
```
can blow up memory consumption! Handle with care!
