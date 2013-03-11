libsi
=====

C library for interpret scheme

Install
=====

install:
	$ make && sudo make install
it also includes a tiny demo which is under `si` directory, use
	$ make demo
to compile it, and
	$ cd si && ./si
to have a try. In demo, you could try following function
	si> (map (lambda (x) (+ x 20)) '(1 2 3))

Hope you enjoy it.

Dependency
=====

The library itself requires **libgc** and **libm**, in addition the demo requires **libreadline**
