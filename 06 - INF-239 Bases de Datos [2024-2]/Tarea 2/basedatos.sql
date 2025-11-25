create table producto(
	id_producto int auto_increment primary key,
	nombre varchar(100) not null,
	precio int not null,
	unidades_disponibles int,
	imagen varchar(255),
	tipo_producto enum('consola','videojuego') not null
);

create table consola(
	id_producto int primary key,
	marca varchar(100) not null,
	formato enum('sobremesa','portatil') not null,
	num_controles int not null,
	foreign key (id_producto) references producto(id_producto) on delete cascade on update cascade
);

create table videojuego(
	id_producto int primary key,
	descripcion text not null,
	clasificacion enum('RP','E','E10+','T','M','A') not null,
	calificacion int,
	foreign key (id_producto) references producto(id_producto) on delete cascade on update cascade
);

create table carrito(
	id_carrito int auto_increment primary key,
	id_producto int not null,
	cantidad int not null,
	precio_unitario int not null,
	precio_total int not null,
	foreign key (id_producto) references producto(id_producto) on delete cascade on update cascade
);

create table oferta(
	id_oferta int auto_increment primary key,
	existe boolean
);

INSERT INTO producto(nombre, precio, unidades_disponibles, imagen, tipo_producto) VALUES
('PlayStation 5', 500, 20, 'imagenes/ps5.jpg', 'consola'),
('Wii', 200, 89, 'imagenes/wii.jpg', 'consola'),
('Nintendo Switch', 300, 27, 'imagenes/nintendo_switch.jpg', 'consola'),
('PlayStation 4', 300, 44, 'imagenes/ps4.jpg', 'consola'),
('Xbox One', 280, 32, 'imagenes/xbox_one.jpg', 'consola'),
('Super Nintendo', 180, 15, 'imagenes/super_nintendo.jpg', 'consola'),
('PlayStation 3', 200, 38, 'imagenes/ps3.jpg', 'consola'),
('Xbox 360', 200, 45, 'imagenes/xbox_360.jpg', 'consola'),
('PlayStation Portable (PSP)', 120, 24, 'imagenes/psp.jpg', 'consola'),
('Nintendo DS', 110, 65, 'imagenes/nintendo_ds.jpg', 'consola'),
('Game Boy Advance', 100, 50, 'imagenes/gba.jpg', 'consola');

INSERT INTO consola(id_producto, marca, formato, num_controles) VALUES
(1, 'Sony', 'sobremesa', 1),
(2, 'Nintendo', 'sobremesa', 1),
(3, 'Nintendo', 'portatil', 2),
(4, 'Sony', 'sobremesa', 1),
(5, 'Microsoft', 'sobremesa', 1),
(6, 'Nintendo', 'sobremesa', 2),
(7, 'Sony', 'sobremesa', 1),
(8, 'Microsoft', 'sobremesa', 1),
(9, 'Sony', 'portatil', 1),
(10, 'Nintendo', 'portatil', 1),
(11, 'Nintendo', 'portatil', 1);

INSERT INTO producto(nombre, precio, unidades_disponibles, imagen, tipo_producto) VALUES
('The Legend of Zelda: Breath of the Wild', 60, 100, 'imagenes/zelda.jpg', 'videojuego'),
('Red Dead Redemption 2', 70, 50, 'imagenes/rdr2.jpg', 'videojuego'),
('Animal Crossing: New Horizons', 60, 65, 'imagenes/animal_crossing.jpg', 'videojuego'),
('Minecraft', 30, 972, 'imagenes/minecraft.jpg', 'videojuego'),
('Grand Theft Auto V', 40, 43, 'imagenes/gta_v.jpg', 'videojuego'),
('Call of Duty: Modern Warfare', 60, 80, 'imagenes/cod_mw.jpg', 'videojuego'),
('Doom Eternal', 50, 65, 'imagenes/doom_eternal.jpg', 'videojuego'),
('Among Us', 5, 842, 'imagenes/among_us.jpg', 'videojuego'),
('Valorant', 20, 218, 'imagenes/valorant.jpg', 'videojuego'),
('Dark Souls III', 50, 85, 'imagenes/dark_souls_3.jpg', 'videojuego'),
('Clash of Clans', 25, 500, 'imagenes/clash_of_clans.jpg', 'videojuego'),
('Candy Crush Saga', 0, 592, 'imagenes/candy_crush.jpg', 'videojuego'),
('Genshin Impact', 15, 446, 'imagenes/genshin_impact.jpg', 'videojuego'),
('Pokémon GO', 5, 859, 'imagenes/pokemon_go.jpg', 'videojuego'),
('Free Fire', 0, 599, 'imagenes/free_fire.jpg', 'videojuego'),
('Subway Surfers', 0, 594, 'imagenes/subway_surfers.jpg', 'videojuego'),
('Dragon City', 7, 400, 'imagenes/dragon_city.jpg', 'videojuego'),
('Pou', 100, 553, 'imagenes/pou.jpg', 'videojuego'),
('Pacman', 0, 498, 'imagenes/pacman.jpg', 'videojuego'),
('Pet Society', 1, 349, 'imagenes/pet_society.jpg', 'videojuego'),
('Club Penguin', 24, 858, 'imagenes/club_penguin.jpg', 'videojuego'),
('Terraria', 10, 25, 'imagenes/header.jpg', 'videojuego');

INSERT INTO videojuego(id_producto, descripcion, clasificacion, calificacion) VALUES
(12, 'Un épico juego de aventura en un mundo abierto.', 'E', 4.0),
(13, 'Acción y aventuras en el lejano oeste', 'M', 5.0),
(14, 'Simulación de vida en una isla encantadora', 'E', 5.0),
(15, 'Juego de construcción y aventuras en bloques', 'E', 5.0),
(16, 'Juego de acción en un mundo abierto criminal', 'M', 3.5),
(17, 'Shooter en primera persona con acción intensa', 'M', 4.0),
(18, 'Shooter en primera persona con demonios', 'M', 4.0),
(19, 'Multijugador en equipo con roles secretos', 'E10+', 1.9),
(20, 'Shooter táctico en equipos de 5', 'T', 4.0),
(21, 'Acción y dificultad extrema en un mundo oscuro', 'M', 4.0),
(22, 'Juego de estrategia multijugador en línea', 'E', 4.0),
(23, 'Juego de puzles y combinación de colores', 'E', 4.0),
(24, 'RPG de mundo abierto en una fantasía animada', 'T', 3.0),
(25, 'Juego de realidad aumentada para capturar Pokémon', 'E', 5.0),
(26, 'Shooter battle royale para móviles', 'T', 4.5),
(27, 'Carreras infinitas en el metro', 'E', 4.8),
(28, 'Juego de simulación de criar dragones y combatir', 'E', 4.0),
(29, 'Juego de mascota virtual donde cuidas a Pou', 'E', 3.0),
(30, 'Clásico juego de arcade donde evades fantasmas', 'E', 5.0),
(31, 'Simulación de vida y mascotas en línea', 'E', 4.0),
(32, 'Juego en línea multijugador donde los jugadores controlan pingüinos', 'E', 1.6),
(33, 'Minecraft en 2D', 'E', 5.0);

insert into oferta(existe) values
(0);

DELIMITER //
CREATE PROCEDURE pagarTodosLosCarritos()
BEGIN
    DECLARE v_id_carrito INT;
    DECLARE v_id_producto INT;
    DECLARE v_cantidad INT;
    DECLARE done INT DEFAULT FALSE;
    DECLARE carrito_cursor CURSOR FOR 
    SELECT id_carrito, id_producto, cantidad FROM Carrito;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
    OPEN carrito_cursor;
    read_loop: LOOP
        FETCH carrito_cursor INTO v_id_carrito, v_id_producto, v_cantidad;
        IF done THEN
            LEAVE read_loop;
        END IF;
        UPDATE Producto
        SET unidades_disponibles = unidades_disponibles - v_cantidad
        WHERE id_producto = v_id_producto;
    END LOOP;
    CLOSE carrito_cursor;
    DELETE FROM Carrito;
END //
DELIMITER ;

DELIMITER //
CREATE TRIGGER verificarUnidadesDisponibles
BEFORE UPDATE ON Producto
FOR EACH ROW
BEGIN
    IF NEW.unidades_disponibles < 0 THEN
        SIGNAL SQLSTATE '45000' 
        SET MESSAGE_TEXT = 'Unidades insuficientes para realizar la compra';
    END IF;
END //
DELIMITER ;