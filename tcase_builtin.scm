(length '(1 2 3 4))		;should be 4
(list 1 2 3 4)			;should be (1 2 3 4)
(list)				;should be ()
(cons 1 '(2))			;should be (1 2)
(car (cons 1 '(2)))		;should be 1
(car (cdr (cons 1 '(2))))	;should be 2
(min 1 0.7 -2)			;should be -2
(max 2 1 3.3)			;should be 3.3
(eval '(min 2 3))		;should be 2
((lambda (x) (* x x)) 1)	;should be 1
((lambda (x y) x y) 3 2)	;should be 2
((lambda () 3))			;should be 3
((lambda (x) (eval x)) 4)	;should be 4
(if #t 2 3)			;should be 2
(if 0 1 2)			;should be 1, 0 is also treated as true
(if 3 2)			;should be 2
(if #f 1 2)			;should be 2
