-- Consulta 1
Select
    nombre
From deportista
Where sexo = 'F';

-- Consulta 2
Select
    codigo_pais,
    nombre
From paises
Where nombre like 'C%'
Order by codigo_pais desc;

-- Consulta 3
Select
    sexo,
    count(*) as total_atletas
From deportista
Group by sexo;

-- Consulta 4
Select
    id_pais,
    count(*) as n_atletas
From deportista
Group by id_pais;

-- Consulta 5
Select
    string_agg(c.nombre, ' | ') as eventos
From categoria c
Join deporte s
    on c.id_deporte = s.id_deporte
Where s.nombre = '"Athletics"';

-- Consulta 6
Select
    d.nombre,
    c.nombre as evento
From deportista d
Join resultados r
    on d.id_deportista = r.id_deportista
Join categoria c
    on c.id_categoria = r.id_categoria;

-- Consulta 7
Select distinct
    d.nombre
From deportista d
Join resultados r
    on d.id_deportista = r.id_deportista;

-- Consulta 8
Select
    d.nombre,
    count(distinct r.id_categoria) as num_eventos
From deportista d
Join resultados r
    on d.id_deportista = r.id_deportista
Group by d.nombre
Having count(distinct r.id_categoria) >= 3;

-- Consulta 9
Select
    c.nombre as evento,
    count(*) as total_medallas
From resultados r
Join categoria c
    on r.id_categoria = c.id_categoria
Group by c.nombre
Having count(*) > 500;

-- Consulta 10
Select
    c.nombre as evento,
    avg(d.altura) as altura_promedio,
    max(d.altura) as altura_maxima,
    min(d.altura) as altura_minima
From resultados r
Join deportista d
    on r.id_deportista = d.id_deportista
Join categoria c
    on r.id_categoria = c.id_categoria
Group by c.nombre
Having avg(d.altura) > 170;

-- Consulta 11
Select
    p.nombre as pais,
    s.nombre as deporte,
    count(*) as total_medallas
From resultados r
Join deportista d
    on r.id_deportista = d.id_deportista
Join paises p
    on d.id_pais = p.id_pais
Join categoria c
    on r.id_categoria = c.id_categoria
Join deporte s
    on c.id_deporte = s.id_deporte
Group by
    p.nombre,
    s.nombre
Having count(*) > 300;

-- Consulta 12
Select top 5
    p.nombre as pais,
    count(*) as total_medallas
From resultados r
Join deportista d
    on r.id_deportista = d.id_deportista
Join paises p
    on d.id_pais = p.id_pais
Join categoria c
    on r.id_categoria = c.id_categoria
Join deporte s
    on c.id_deporte = s.id_deporte
Where s.nombre = '"Swimming"'
Group by p.nombre
Order by count(*) desc;

-- Consulta 13
Update deportista
Set peso = peso + 10
Where sexo = 'M';

-- Consulta 14
Delete
From deportista
Where id_deportista in (
    Select
        id_deportista
    From resultados
    Where medalla = '"Silver"'
);