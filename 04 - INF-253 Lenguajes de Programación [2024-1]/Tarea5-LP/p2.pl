% Predicado que calcula la Distancia Manhattan entre dos puntos
distManhattan(punto(X1, Y1), punto(X2, Y2), D) :- D is abs(X1 - X2) + abs(Y1 - Y2).

% Predicado que encuentra la minima distancia Manhattan entre un punto fijo y una lista de puntos
minDistManhattan(_, [], inf).

minDistManhattan(P, [H|T], R) :- distManhattan(P, H, D), minDistManhattan(P, T, R1), R is min(D, R1).

minDistManhattan(P, [H|T], R) :- distManhattan(P, H, R), minDistManhattan(P, T, R1), R < R1.

% Predicado para calcular el minimo de dos numeros
min(A, B, A) :- A =< B.
min(A, B, B) :- B < A.
