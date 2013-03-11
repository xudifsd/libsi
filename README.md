libsi
=====

C library for interpret scheme

Have a try
=====

##install library

	$ make && sudo make install

it also includes a tiny demo which is under `si` directory
##compile demo

	$ make demo

##try demo

	$ cd si && ./si

In demo, you could try following function

	si> (map (lambda (x) (+ x 20)) '(1 2 3))

Hope you enjoy it.

Dependency
=====

The library itself requires `libgc` and `libm`, in addition the demo requires `libreadline`
