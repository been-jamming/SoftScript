# SoftScript
A lightweight, customizable, interpreted programming language

To compile:

`gcc SoftScript.c hollow_lists.c dictionary.c datavalue.c lexer.c expression.c include.c standard.c -o softscript -Ofast`

To run the example program:

`./softscript test.ss`

To run the CLI:

`./softscript`

Some example code:

`>>>print("Hello World!")`
Simple hello world

`>>>s = 2.`
`>>>q = 1`
`>>>lq = 0`
Sets s to 2.0, q to 1, and lq to 0

`>>>while(lq != q, {lq = q; q = ((s/q + q)/2)})`
A while loop which uses Newton's method to calculate the square root of s

`>>>a = (array(100, 100))`
Creates a 100 by 100 array and assigns it to the variable a

`>>>a[0][0] = 221`
Assigns the value 221 to a\[0\]\[0\]

<a href = "https://github.com/computergeek314/SoftScript/wiki">Documentation</a> is provided in this repository's wiki.
