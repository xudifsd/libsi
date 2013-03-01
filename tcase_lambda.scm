((lambda (x) (* x x)) 1)	;should be 1
((lambda (x y) x y) 3 2)	;should be 2
((lambda () 3))			;should be 3
((lambda (x) (eval x)) 4)	;should be 4
