(define (map fun lst)
  (if (null? lst)
    '()
    (cons
     (fun (car lst))
     (map fun (cdr lst)))))

(define (cadr lst)
  (car (cdr lst)))

(defmacro (let args #!rest body)
  `((lambda ,(map car args)
      ,@body)
    ,@(map cadr args)))

(define (caar lst)
  (car (car lst)))

(define (cdar lst)
  (cdr (car lst)))

(defmacro (cond #!rest conds)
  (if (null? conds)
    '()
    (if (eq? (caar conds) 'else)
      `(begin ,@(cdar conds))
      `(if ,(caar conds)
        (begin ,@(cdar conds))
        (cond ,@(cdr conds))))))

(defmacro (and e1 #!rest rest)
  `(if (null? (list ,@rest))
    ,e1
    (if ,e1
      (and ,@rest)
      ,e1)))
