% Grafo
camino(s, p1, 1).
camino(p1, p3, 5).
camino(p3, p4, 6).
camino(p4, p5, -5).
camino(p5, p6, 2).
camino(p6, jefe, 4).

camino(s, p2, 8).
camino(p2, p11, 1).
camino(p11, p12, -12).
camino(p12, p9, 2).
camino(p9, p10, -7).
camino(p10, jefe, 1).
camino(p10, p6, 2).

camino(p3, p7, -2).
camino(p7, p8, 10).
camino(p7, p9, 2).
camino(p8, p9, -9).
camino(p3, p8, 9).
camino(p8, p5, -1).
camino(p8, p6, 5).
