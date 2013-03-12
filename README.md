libsi
=====

C library for interpret scheme


##install library

	$ make && sudo make install

it also includes a tiny demo which is under `si` directory
##compile demo

	$ make demo

##try demo
because demo could not use libreadline itself(libreadline and libgc both use heap),
so recommend you use following command:

	$ cd si && rlwrap ./si

In demo, you could try following function

	si> (map (lambda (x) (+ x 20)) '(1 2 3))

Hope you enjoy it.

Dependency
=====

The library itself requires `libgc` and `libm`
