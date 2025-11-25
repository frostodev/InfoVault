% Si la lista es vacia
obtenerRango([], 0).

% Si la lista tiene un solo elemento, el rango es 0.
obtenerRango([_], 0).

% Definición para obtener el rango de una lista no vacia.
obtenerRango(L, R) :- L \= [], obtenerMin(L, Min), obtenerMax(L, Max), R is Max - Min.

% Obtener el minimo de una lista
obtenerMin([X], X).
obtenerMin([X|Xs], Min) :- obtenerMin(Xs, MinXs), Min is min(X, MinXs).

% Obtener el maximo de una lista
obtenerMax([X], X).
obtenerMax([X|Xs], Max) :- obtenerMax(Xs, MaxXs), Max is max(X, MaxXs).
