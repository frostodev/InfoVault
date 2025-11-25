
-- Crear tabla temporal para importar desde el csv de atletas
CREATE TABLE #TemporalAthlete (
	id varchar(255),
	name varchar(255),
	sex varchar(255),
	age varchar(255),
	height varchar(255),
	weight varchar(255),
	team varchar(255),
	noc varchar(255),
	games varchar(255),
	years varchar(255),
	season varchar(255),
	city varchar(255),
	sport varchar(255),
	event varchar(255),
	medal varchar(255)
);

-- Lo mismo pero para el csv de países
CREATE TABLE #TemporalPaises (
	noc varchar(50),
	region varchar(50),
	notes varchar(255)
);

-- Insertar los datos del csv de atletas hacia la tabla temporal
BULK INSERT #TemporalAthlete
FROM 'D:\athlete_events.csv'
WITH (
    FIELDTERMINATOR = ',',
    ROWTERMINATOR = '\n',
    FIRSTROW = 2
);

-- Misma idea, pero con el csv de países
BULK INSERT #TemporalPaises
FROM 'D:\noc_regions.csv'
WITH (
    FIELDTERMINATOR = ',',
    ROWTERMINATOR = '\n',
    FIRSTROW = 2
);

-- Eliminar los casos inválidos...
delete from #TemporalAthlete where age = '"M"' or age = '"F"';
delete from #TemporalAthlete where height = '"M"' or height = '"F"';
delete from #TemporalAthlete where weight = '"M"' or weight = '"F"';
delete from #TemporalAthlete where sport = '"St. Louis"' or sport = '"Athina"' or sport = '"Stockholm"' or sport = '"Paris"';
delete from #TemporalAthlete where medal not in ('"Bronze"', '"Silver"', '"Gold"') and medal is not null;

-- PAISES
INSERT INTO paises (codigo_pais, nombre)
SELECT noc, region
FROM #TemporalPaises;

-- DEPORTE
INSERT INTO deporte (nombre)
SELECT distinct sport
FROM #TemporalAthlete;

-- JUEGO OLIMPICO
INSERT INTO juego_olimpico(edicion)
SELECT distinct games
FROM #TemporalAthlete;

-- CATEGORÍA
INSERT INTO categoria(nombre)
SELECT distinct event
from #TemporalAthlete;

update c
set id_deporte = d.id_deporte
from categoria c
join #TemporalAthlete t on t.event = c.nombre
join deporte d on d.nombre = t.sport;

-- DEPORTISTAS
;WITH RankedAthletes AS (
    SELECT
        CAST(REPLACE(id, '"', '') AS INT) AS id,
        name,
        CAST(REPLACE(sex, '"', '') AS char(1)) AS sex,
        age,
        height,
        weight,
        ROW_NUMBER() OVER (PARTITION BY CAST(REPLACE(id, '"', '') AS INT) ORDER BY (SELECT NULL)) AS rn
    FROM #TemporalAthlete
)
INSERT INTO deportista (id_deportista, nombre, sexo, edad, altura, peso)
SELECT
    id,
    name,
    sex,
    age,
    height,
    weight
FROM RankedAthletes
WHERE rn = 1;

UPDATE d
SET d.id_pais = p.id_pais
FROM deportista d
JOIN #TemporalAthlete t ON CAST(REPLACE(t.id, '"', '') AS int) = d.id_deportista
JOIN paises p ON CAST(REPLACE(t.noc, '"', '') AS char(3)) = p.codigo_pais;

-- RESULTADOS
INSERT INTO resultados (id_juego_olimpico, id_categoria, id_deportista, medalla)
SELECT j.id_juego_olimpico, c.id_categoria, d.id_deportista, t.medal
FROM #TemporalAthlete t
JOIN deportista d ON d.id_deportista = CAST(REPLACE(t.id, '"', '') AS int)
JOIN juego_olimpico j ON j.edicion = t.games
JOIN categoria c on c.nombre = t.event
WHERE t.medal IS NOT NULL;