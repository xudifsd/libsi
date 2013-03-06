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
(define x 10)			;should be ()
(define (y z) 1 x)		;should be ()
(y 100)				;should be 10
(define (yy z) (set! x z))	;should be ()
x				;should be 10
(yy 200)			;should be ()
x				;should be 200
(< 1 2)				;should be #t
(< 1.2 2)			;should be #t
(> 1.2 2)			;should be #f
(<= 0 2)			;should be #t
(= 2 2.0)			;should be #t
(>= -1.2 2)			;should be #f
(>= 1.0 0)			;should be #t
(define (map fun lst)
  (if (null? lst)
    '()
     (cons
      (fun (car lst))
      (map fun (cdr lst)))))	;should be ()
(map (lambda (x) (* x 3))
        '(2 3 4 5))		;should be (6 9 12 15)
(define (cadr lst)
  (car (cdr lst)))		;should be ()
(defmacro (let args #!rest body)
  `((lambda ,(map car args)
      ,@body)
    ,@(map cadr args)))		;should be ()
(let ((x -3)) (+ 2 x))		;should be -1
