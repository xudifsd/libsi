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

(define body
  '(let ((x -3)) (+ 2 x)))	;should be ()

(eval body)			;should be -1

(macroexpand body)		;shoud be ((lambda (x) (+ 2 x)) -3)

(define (caar lst)
  (car (car lst)))		;should be ()

(define (cdar lst)
  (cdr (car lst)))		;should be ()

(defmacro (cond #!rest conds)
  (if (null? conds)
    '()
    (if (eq? (caar conds) 'else)
      `(begin ,@(cdar conds))
      `(if ,(caar conds)
        (begin ,@(cdar conds))
        (cond ,@(cdr conds))))));should be ()

(define x 20)			;should be ()

(define body2
  '(cond ((= x 10) #t)
         ((eq? x 's) #f)
         (else
           200)))		;should be ()
(eval body2)			;should be 200

(defmacro (and e1 #!rest rest)
  `(if (null? (list ,@rest))
    ,e1
    (if ,e1
      (and ,@rest)
      ,e1)))			;should be ()

(and 1 2 3)			;should be 3
