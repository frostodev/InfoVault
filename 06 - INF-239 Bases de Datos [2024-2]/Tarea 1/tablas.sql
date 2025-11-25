create table juego_olimpico (
	id_juego_olimpico int primary key identity(1,1),
	edicion varchar(14)
);

create table deporte (
	id_deporte int primary key identity(1,1),
	nombre varchar(30)
);

create table categoria (
	id_categoria int primary key identity(1,1),
	nombre varchar(100) not null,
	id_deporte int,
	foreign key (id_deporte) references deporte(id_deporte)
);

create table paises (
	id_pais int primary key identity(1,1),
	codigo_pais char(3) not null,
	nombre varchar(50) not null
);

create table deportista (
	id_deportista int primary key,
	nombre varchar(200) not null,
	sexo char(1) not null,
	edad smallint,
	altura smallint,
	peso float,
	id_pais int,
	foreign key (id_pais) references paises(id_pais)
);

create table resultados (
	id_resultado int primary key identity(1,1),
	id_juego_olimpico int,
	id_categoria int,
	id_deportista int not null,
	medalla varchar(8),
	foreign key (id_juego_olimpico) references juego_olimpico(id_juego_olimpico),
	foreign key (id_categoria) references categoria(id_categoria),
	foreign key (id_deportista) references deportista(id_deportista)
	on delete cascade
);