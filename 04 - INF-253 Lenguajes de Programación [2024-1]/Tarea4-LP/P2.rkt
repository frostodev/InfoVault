#lang scheme

;; Implementa el cálculo del numero de catalán usando recursión simple
;;
;; n: n-ésimo número a calcular
(define (catalan_simple n)
  (if (= n 0) ;; Si n es 0
      1 ;; Retornar 1
      (* (/ (* 2 (- (* 2 n) 1)) (+ n 1)) (catalan_simple (- n 1))))) ;; Hacer el calculo recursivo

;; Implementa el cálculo del número de Catalán usando recursión de cola
;;
;; n: n-ésimo número a calcular
(define (catalan_cola n)  
  (letrec ((catalan_cola_rec
            (lambda (n a)  
              (if (= n 0)  ;; Si n es 0
                  a  ;; Retornar el acumulador a
                  (catalan_cola_rec  ;; Sino, llamar recursivamente a catalan_cola_rec
                   (- n 1)  ;; Decrementa n en 1
                   (* a (/ (* 2 (- (* 2 n) 1)) (+ n 1))))))))  ;; Calcular el nuevo valor del acumulador a, usando la fórmula
    (catalan_cola_rec n 1)))  ;; Llama a la función recursiva con n y el acumulador inicial 1
