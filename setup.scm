(define (map fun lst)
  (if (null? lst)
    '()
    (cons
     (fun (car lst))
     (map fun (cdr lst)))))

(define (for-each fun lst)
  (if (null? lst)
    '()
    (begin
     (fun (car lst))
     (for-each fun (cdr lst)))))

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

;; this function is not intended used as basic function, but it was need
;; by ->> macro, and we don't support define function in macro body yet
(define (list-if-not x)
  (if (list? x)
      x
    (list x)))

(defmacro (->> x #!rest form)
  (if (null? form)
      x
    `(->> ,(append (list-if-not (car form))
                   (list x))
      ,@(cdr form))))
