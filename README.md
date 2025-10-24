# A BASIC interpreter

This is an experiment in writing an interpreter for the BASIC programming language, 
without much preparation or knowledge about interpreters.

And just to make everything harder, I've decided to write it all in C - however I have opted for using C23 and many of its modern features.

I plan to learn as I go along, and be prepared to make large refactorings, or possibly
rewrites, as I learn better ways of doing things. In reality I do know about parsers, 
lexers, compilers, regular expressions, BNF grammars and a lot more, but I do not want
to pretend to be an expert, so I develop everything iteratively, as needed.

Build the project with

```bash
make basic
```

Run the interpreter with

```bash
./basic
```

## BASIC Commands

So far, these commands are implemented:

|Command | Example      | Description |
|--------|--------------|-------------|
|LET     | LET a = 5    | declares a variable and initializes with a value     |
|PRINT   | PRINT a      | prints a string, a number or the value of a variable |
|INPUT   | INPUT ".." a | reads a value from the user and stores it in a variable |
|LIST    | LIST         | lists the current program in memory |

Values can be strings, integers and mathematical operators.

Variables can be reassigned by writing 'variablename' = 'new value' where the new value can be another variable, or any combination of mathematical operators.

Writing a number followed by any command(s) stores those commands as a line in the program in memory. New lines are inserted according to the number, which
becomes the line-number.

Writing a number without anything following, deletes the line with that number from the program in memory.

## Mathematical operators

The following operators are implemented:

|operator|meaning|
|--------|-------|
| +      | add |
| -      | subtract |
| *      | multiply |
| /      | divide |

Note that operator precedence is not followed! 2 + 3 * 4 does not give the same result as 4 * 3 + 2.


The full "documentation" of my exploits is available online: [peterlinddk.notion.site/BASIC-interpreter](https://peterlinddk.notion.site/BASIC-interpreter-22d36f4e0a82809e8907c1f0f2076668)