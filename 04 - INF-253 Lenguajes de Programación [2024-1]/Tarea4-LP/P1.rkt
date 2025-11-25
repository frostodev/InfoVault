#lang scheme

;; Función que obteniene el planeta con el menor porcentaje de liberación
;;
;; lista: Lista que contiene planetas y su porcentaje, en pares.
(define (liberar lista)
  (if (null? lista) ;; Si la lista está vacía
      '() ;; Retornar una lista vacía
      (let ((mejor-planeta (car lista)) ;; Sino, descomponer la lista en el primer elemento y el resto
            (resto (cdr lista))) ;; 'resto' es la lista sin el primer elemento
        (liberar-rec mejor-planeta resto)))) ;; Llamar a la función 'liberar-rec'` con 'mejor-planeta' y el resto de la lista

;; Función función auxiliar `liberar-rec` que toma el mejor planeta actual y una lista como argumentos
;;
;; mejor-planeta: El planeta con el menor porcentaje de liberación encontrado hasta ahora.
;; lista: La lista de planetas restantes por evaluar.
(define (liberar-rec mejor-planeta lista)
  (if (null? lista) ;; Si la lista está vacía
      (list (car mejor-planeta)) ;; Retornar una lista con 'mejor-planeta' como único elemento
      (let ((planeta (car lista)))  ;; De lo contrario, descomponer la lista en el primer elemento (planeta) y el resto
        (if (< (cadr planeta) (cadr mejor-planeta)) ;; Comparar el segundo elemento de 'planeta' y 'mejor-planeta'
            (liberar-rec planeta (cdr lista)) ;; Si el segundo elemento de 'planeta' es menor, llamra por recursión simple a 'liberar-rec' con 'planeta' como nuevo mejor-planeta
            (liberar-rec mejor-planeta (cdr lista)))))) ;; De lo contrario, llamar recursivamente a 'liberar-rec' con el 'mejor-planeta' actual
