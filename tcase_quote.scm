`,1			;should be 1
`,(+ 1 2)		;should be 3
`(1 2 ,3)		;should be (1 2 3)
'()			;should be ()
`()			;should be ()
`(1 ,(+ 1 1) 3)		;should be (1 2 3)
`(1 (3))		;should be (1 (3))
`(1 ,@'(1 2) 3)		;should be (1 1 2 3)
`(1 (,(+ 1 1)) 3)	;should be (1 (2) 3)
`(1 (,@'(1 2)) 3)	;should be (1 (1 2) 3)
(define x '(1 2 3))	;should be ()
`(0 ,@x)		;should be (0 1 2 3)
