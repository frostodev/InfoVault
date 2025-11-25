% Definir verificar() como función recursiva 
verificar(S) :- verificarRecursivo(S, []).

% Si el nodo a verificar es la meta, entonces este es válido
verificarRecursivo(m, _).

% Definición recursiva de verificar()
verificarRecursivo(Node, Visitados) :- camino(Node, Node2), \+ revisar(Node2, Visitados), verificarRecursivo(Node2, [Node2 | Visitados]).

% Predicado para verificar si un elemento pertenece en la lista
revisar(elemento, [elemento | _]).
revisar(elemento, [_ | lista]) :- revisar(elemento, lista).

% Laberinto (se mantiene igual)
camino(p11, p12).
camino(p12, p13).
camino(p13, p14).
camino(p14, p15).
camino(p15, p25).
camino(p25, m).

camino(p11, p21).
camino(p21, p31).

camino(p13, p23).
camino(p23, p33).
camino(p33, p34).
camino(p34, m).