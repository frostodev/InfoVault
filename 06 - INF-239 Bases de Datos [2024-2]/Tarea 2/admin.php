<?php
// Conexión a la base de datos
$servername = "localhost";
$username = "pagina";
$password = "contraseña";
$dbname = "usteam";

$conn = new mysqli($servername, $username, $password, $dbname);

// En caso de error
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Manejo de acciones CRUD
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $action = $_POST['action'];

    if ($action == "add_consola") {
        // Agregar Consola
        $marca = $_POST['marca'];
        $formato = $_POST['formato'];
        $num_controles = $_POST['num_controles'];
        $precio = $_POST['precio'];
        $nombre = $_POST['nombre'];
        $unidades_disponibles = $_POST['unidades_disponibles'];

        // Manejo de imagen
        $imagen = $_FILES['imagen']['name'];
        $target_dir = "imagenes/";
        $target_file = $target_dir . basename($imagen);
        move_uploaded_file($_FILES['imagen']['tmp_name'], $target_file);

        // Guardar la ruta completa en la base de datos
        $imagen_con_ruta = '/imagenes/' . basename($imagen);
        
        // Insertar Producto
		$stmt = $conn->prepare("INSERT INTO Producto (nombre, precio, imagen, tipo_producto, unidades_disponibles) VALUES (?, ?, ?, 'consola', ?)");
		$stmt->bind_param("sisi", $nombre, $precio, $target_file, $unidades_disponibles);
        $stmt->execute();
        $id_producto = $stmt->insert_id;

        // Insertar Consola
        $stmt = $conn->prepare("INSERT INTO Consola (id_producto, marca, formato, num_controles) VALUES (?, ?, ?, ?)");
        $stmt->bind_param("issi", $id_producto, $marca, $formato, $num_controles);
        $stmt->execute();
	}

	// Actualizar Consola
    if ($action == "update_consola") {
		$id_producto = $_POST['id_producto'];
		$marca = $_POST['marca'];
		$formato = $_POST['formato'];
		$num_controles = $_POST['num_controles'];
		$precio = $_POST['precio'];
		$nombre = $_POST['nombre'];
		$unidades_disponibles = $_POST['unidades_disponibles'];
		$imagen = $_FILES['imagen']['name'];

		// Manejo de imagen
		if ($imagen) {
			$target_dir = "imagenes/";
			$target_file = $target_dir . basename($imagen);
			move_uploaded_file($_FILES['imagen']['tmp_name'], $target_file);
			$imagen_con_ruta = '/imagenes/' . basename($imagen);
		} else {
			// Si no hay nueva imagen, obtener la existente
			$stmt = $conn->prepare("SELECT imagen FROM Producto WHERE id_producto=?");
			$stmt->bind_param("i", $id_producto);
			$stmt->execute();
			$result = $stmt->get_result();
			$row = $result->fetch_assoc();
			$imagen_con_ruta = $row['imagen'];
			// No modificar el $target_file si no hay nueva imagen
			$target_file = $imagen_con_ruta; // Mantener la imagen existente
		}

        // Actualizar Producto
        $stmt = $conn->prepare("UPDATE Producto SET nombre=?, precio=?, imagen=?, unidades_disponibles=? WHERE id_producto=?");
        $stmt->bind_param("sisii", $nombre, $precio, $target_file, $unidades_disponibles, $id_producto);
        $stmt->execute();

        // Actualizar Consola
        $stmt = $conn->prepare("UPDATE Consola SET marca=?, formato=?, num_controles=? WHERE id_producto=?");
        $stmt->bind_param("ssii", $marca, $formato, $num_controles, $id_producto);
        $stmt->execute();
    }
	
	// Eliminar Consola
	if ($action == 'delete_consola') {
		$id_producto = $_POST['id_producto'];

		// Eliminar la consola y el producto
		$sql_consola = "DELETE FROM Consola WHERE id_producto=?";
		$stmt_consola = $conn->prepare($sql_consola);
		$stmt_consola->bind_param("i", $id_producto);
		if ($stmt_consola->execute()) {
			$sql_producto = "DELETE FROM Producto WHERE id_producto=?";
			$stmt_producto = $conn->prepare($sql_producto);
			$stmt_producto->bind_param("i", $id_producto);
			$stmt_producto->execute();
		}
	}

    // Añadir videojuego
	if ($action == 'add_videojuego') {
		// Obtener datos del formulario
		$descripcion = $_POST['descripcion'];
		$clasificacion = $_POST['clasificacion'];
		$calificacion = $_POST['calificacion'];
		$nombre = $_POST['nombre'];
		$precio = $_POST['precio'];
		$unidades_disponibles = $_POST['unidades_disponibles'];

		// Manejo de imagen
		$imagen = $_FILES['imagen']['name'];
		$target_dir = "imagenes/";
		$target_file = $target_dir . basename($imagen);
		
		if (move_uploaded_file($_FILES['imagen']['tmp_name'], $target_file)) {

			// Consulta para insertar el videojuego
			$sql = "INSERT INTO Producto (nombre, precio, unidades_disponibles, imagen, tipo_producto) VALUES (?, ?, ?, ?, 'videojuego')";
			$stmt = $conn->prepare($sql);
			
			$stmt->bind_param("ssis", $nombre, $precio, $unidades_disponibles, $target_file);
			
			if ($stmt->execute()) {
				$id_producto = $stmt->insert_id; // Obtener el ID del producto recién creado
				$sql_videojuego = "INSERT INTO Videojuego (id_producto, descripcion, clasificacion, calificacion) VALUES (?, ?, ?, ?)";
				$stmt_videojuego = $conn->prepare($sql_videojuego);
				$stmt_videojuego->bind_param("isss", $id_producto, $descripcion, $clasificacion, $calificacion);
				$stmt_videojuego->execute();
			} 
		}
	}

	// Actualizar Videojuego
	if ($action == 'update_videojuego') {
		$id_producto = $_POST['id_producto'];
		$descripcion = $_POST['descripcion'];
		$clasificacion = $_POST['clasificacion'];
		$calificacion = $_POST['calificacion'];
		$nombre = $_POST['nombre'];
		$precio = $_POST['precio'];
		$unidades_disponibles = $_POST['unidades_disponibles'];

		// Manejo de la imagen
		if ($_FILES['imagen']['name']) {
			$imagen = $_FILES['imagen']['name'];
			$ruta_imagen = 'imagenes/' . basename($imagen);
			
			// Verificar si la imagen se subió correctamente
			if (move_uploaded_file($_FILES['imagen']['tmp_name'], $ruta_imagen)) {
				$sql = "UPDATE Producto SET nombre=?, precio=?, unidades_disponibles=?, imagen=? WHERE id_producto=?";
				$stmt = $conn->prepare($sql);
				$stmt->bind_param("ssisi", $nombre, $precio, $unidades_disponibles, $ruta_imagen, $id_producto);
			}
			
		} else {
			$sql = "UPDATE Producto SET nombre=?, precio=?, unidades_disponibles=? WHERE id_producto=?";
			$stmt = $conn->prepare($sql);
			// No se necesita el campo de imagen
			$stmt->bind_param("siii", $nombre, $precio, $unidades_disponibles, $id_producto);
		}

		if ($stmt->execute()) {
			$sql_videojuego = "UPDATE Videojuego SET descripcion=?, clasificacion=?, calificacion=? WHERE id_producto=?";
			$stmt_videojuego = $conn->prepare($sql_videojuego);
			$stmt_videojuego->bind_param("ssii", $descripcion, $clasificacion, $calificacion, $id_producto);
			$stmt_videojuego->execute();
		}
	}

    // Eliminar Videojuego
    if ($action == 'delete_videojuego') {
        $id_producto = $_POST['id_producto'];

        // Eliminar el videojuego y el producto
        $sql_videojuego = "DELETE FROM Videojuego WHERE id_producto=?";
        $stmt_videojuego = $conn->prepare($sql_videojuego);
        $stmt_videojuego->bind_param("i", $id_producto);
        if ($stmt_videojuego->execute()) {
            $sql_producto = "DELETE FROM Producto WHERE id_producto=?";
            $stmt_producto = $conn->prepare($sql_producto);
            $stmt_producto->bind_param("i", $id_producto);
            $stmt_producto->execute();
        }
    }
}

// Consultar Consolas
$consolas = $conn->query("SELECT * FROM Consola JOIN Producto ON Consola.id_producto = Producto.id_producto");

// Consultar Videojuegos
$videojuegos = $conn->query("SELECT * FROM Videojuego JOIN Producto ON Videojuego.id_producto = Producto.id_producto");
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
    <title>USteaM - Administración</title>
    <style>
        /* Ajuste adicional para alinear todo a la izquierda */
        body {
            margin: 0;
            padding: 20px;
        }
        .table {
            width: auto; /* Evitar el ancho completo de la tabla */
        }
    </style>
</head>
<body>
<div class="container-fluid">

	<!-- Barra superior -->
	<nav class="navbar navbar-light">
		<div class="container">
			<a class="navbar-brand" href="index.php">USteaM</a>
			<div class="ml-auto d-flex align-items-center">
				<a href="index.php" class="btn btn-primary">Volver...</a>
			</div>
		</div>
	</nav>
	
	<h1>Administración de Productos</h1>

    <!-- Consolas -->
    <h2>Consolas</h2>
    <table class="table">
        <thead>
            <tr>
                <th>ID</th>
                <th>Marca</th>
                <th>Formato</th>
                <th>Número de Controles</th>
                <th>Nombre</th>
                <th>Precio</th>
                <th>Unidades Disponibles</th>
                <th>Imagen</th>
                <th>Acciones</th>
            </tr>
        </thead>
        <tbody>
            <?php while ($consola = $consolas->fetch_assoc()): ?>
                <tr>
                    <form method="POST" enctype="multipart/form-data">
                        <td><input type="text" name="id_producto" value="<?= $consola['id_producto'] ?>" readonly></td>
                        <td><input type="text" name="marca" value="<?= $consola['marca'] ?>" required></td>
                        <td>
                            <select name="formato" required>
                                <option value="sobremesa" <?= $consola['formato'] == 'sobremesa' ? 'selected' : '' ?>>Sobremesa</option>
                                <option value="portatil" <?= $consola['formato'] == 'portatil' ? 'selected' : '' ?>>Portátil</option>
                            </select>
                        </td>
                        <td><input type="number" name="num_controles" value="<?= $consola['num_controles'] ?>" required></td>
                        <td><input type="text" name="nombre" value="<?= $consola['nombre'] ?>" required></td>
                        <td><input type="number" name="precio" value="<?= $consola['precio'] ?>" required></td>
                        <td><input type="number" name="unidades_disponibles" value="<?= $consola['unidades_disponibles'] ?>" required></td>
                        <td>
                            <img src="<?= $consola['imagen'] ?>" alt="Imagen" width="50">
                            <input type="file" name="imagen">
                        </td>
                        <td>
                            <button type="submit" name="action" value="update_consola" class="btn btn-primary">Actualizar</button>
                            <button type="submit" name="action" value="delete_consola" class="btn btn-danger">Eliminar</button>
                        </td>
                    </form>
                </tr>
            <?php endwhile; ?>
        </tbody>
    </table>

    <h3>Agregar Consola</h3>
    <form method="POST" enctype="multipart/form-data">
        <input type="hidden" name="action" value="add_consola">
        <div class="form-group">
            <label>Marca</label>
            <input type="text" name="marca" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Formato</label>
            <select name="formato" class="form-control" required>
                <option value="sobremesa">Sobremesa</option>
                <option value="portatil">Portátil</option>
            </select>
        </div>
        <div class="form-group">
            <label>Número de Controles</label>
            <input type="number" name="num_controles" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Nombre</label>
            <input type="text" name="nombre" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Precio</label>
            <input type="number" name="precio" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Unidades Disponibles</label>
            <input type="number" name="unidades_disponibles" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Imagen</label>
            <input type="file" name="imagen" class="form-control" required>
        </div>
        <button type="submit" class="btn btn-success">Agregar Consola</button>
    </form>

	<br>
	<br>

    <!-- Videojuegos -->
    <h2>Videojuegos</h2>
    <table class="table">
        <thead>
            <tr>
                <th>ID</th>
                <th>Descripción</th>
                <th>Clasificación</th>
                <th>Calificación</th>
                <th>Nombre</th>
                <th>Precio</th>
                <th>Unidades Disponibles</th>
                <th>Imagen</th>
                <th>Acciones</th>
            </tr>
        </thead>
        <tbody>
            <?php while ($videojuego = $videojuegos->fetch_assoc()): ?>
                <tr>
                    <form method="POST" enctype="multipart/form-data">
                        <td><input type="text" name="id_producto" value="<?= $videojuego['id_producto'] ?>" readonly></td>
                        <td><input type="text" name="descripcion" value="<?= $videojuego['descripcion'] ?>" required></td>
                        <td>
                            <select name="clasificacion" required>
                                <option value="RP" <?= $videojuego['clasificacion'] == 'RP' ? 'selected' : '' ?>>RP</option>
                                <option value="E" <?= $videojuego['clasificacion'] == 'E' ? 'selected' : '' ?>>E</option>
                                <option value="E10+" <?= $videojuego['clasificacion'] == 'E10+' ? 'selected' : '' ?>>E10+</option>
                                <option value="T" <?= $videojuego['clasificacion'] == 'T' ? 'selected' : '' ?>>T</option>
                                <option value="M" <?= $videojuego['clasificacion'] == 'M' ? 'selected' : '' ?>>M</option>
                                <option value="A" <?= $videojuego['clasificacion'] == 'A' ? 'selected' : '' ?>>A</option>
                            </select>
                        </td>
                        <td>
							<input type="number" name="calificacion" value="<?= $videojuego['calificacion'] ?>" min="1.0" max="5.0" step="0.1">
						</td>
                        <td><input type="text" name="nombre" value="<?= $videojuego['nombre'] ?>" required></td>
                        <td><input type="number" name="precio" value="<?= $videojuego['precio'] ?>" required></td>
                        <td><input type="number" name="unidades_disponibles" value="<?= $videojuego['unidades_disponibles'] ?>" required></td>
                        <td>
                            <img src="<?= $videojuego['imagen'] ?>" alt="Imagen" width="50">
                            <input type="file" name="imagen">
                        </td>
                        <td>
                            <button type="submit" name="action" value="update_videojuego" class="btn btn-primary">Actualizar</button>
                            <button type="submit" name="action" value="delete_videojuego" class="btn btn-danger">Eliminar</button>
                        </td>
                    </form>
                </tr>
            <?php endwhile; ?>
        </tbody>
    </table>

    <h3>Agregar Videojuego</h3>
    <form method="POST" enctype="multipart/form-data">
        <input type="hidden" name="action" value="add_videojuego">
        <div class="form-group">
            <label>Descripción</label>
            <input type="text" name="descripcion" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Clasificación</label>
            <select name="clasificacion" class="form-control" required>
                <option value="RP">RP</option>
                <option value="E">E</option>
                <option value="E10+">E10+</option>
                <option value="T">T</option>
                <option value="M">M</option>
                <option value="A">A</option>
            </select>
        </div>
        <div class="form-group">
            <label>Calificación</label>
            <input type="number" name="calificacion" class="form-control">
        </div>
        <div class="form-group">
            <label>Nombre</label>
            <input type="text" name="nombre" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Precio</label>
            <input type="number" name="precio" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Unidades Disponibles</label>
            <input type="number" name="unidades_disponibles" class="form-control" required>
        </div>
        <div class="form-group">
            <label>Imagen</label>
            <input type="file" name="imagen" class="form-control" required>
        </div>
        <button type="submit" class="btn btn-success">Agregar Videojuego</button>
    </form>
</div>

<br>
<br>

<script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.0.7/dist/umd/popper.min.js"></script>
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
