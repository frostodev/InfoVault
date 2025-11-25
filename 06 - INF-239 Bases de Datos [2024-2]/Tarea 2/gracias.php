<?php
session_start();

// Conexión a la base de datos
$servername = "localhost";
$username = "pagina";
$password = "contraseña";
$dbname = "usteam";

// En caso de error
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Consultar los productos del carrito
$sql = "SELECT c.id_producto, p.nombre, c.cantidad, c.precio_unitario, p.tipo_producto
        FROM Carrito c 
        JOIN Producto p ON c.id_producto = p.id_producto";
$result = $conn->query($sql);

$compra_resumen = [];
$total = 0;

while ($row = $result->fetch_assoc()) {
    $subtotal = $row['precio_unitario'] * $row['cantidad'];
    $total += $subtotal;

    // Verificar si es un videojuego y obtener la calificación
    if ($row['tipo_producto'] === 'videojuego') {
        $sql_calificacion = "SELECT calificacion FROM Videojuego WHERE id_producto = " . $row['id_producto'];
        $result_calificacion = $conn->query($sql_calificacion);
        $calificacion_actual = $result_calificacion->fetch_assoc()['calificacion'] ?? 0;

        $compra_resumen[] = [
            'nombre' => $row['nombre'],
            'precio_unitario' => $row['precio_unitario'],
            'cantidad' => $row['cantidad'],
            'subtotal' => $subtotal,
            'tipo_producto' => 'videojuego',
            'calificacion' => $calificacion_actual
        ];
    } else {
        $compra_resumen[] = [
            'nombre' => $row['nombre'],
            'precio_unitario' => $row['precio_unitario'],
            'cantidad' => $row['cantidad'],
            'subtotal' => $subtotal,
            'tipo_producto' => 'consola',
            'calificacion' => null
        ];
    }
}

// Verificar si hay una oferta activa
$sqlOferta = "SELECT existe FROM Oferta WHERE id_oferta = 1";
$resultOferta = $conn->query($sqlOferta);
$oferta_activa = false;
if ($resultOferta->num_rows > 0) {
	$oferta = $resultOferta->fetch_assoc();
	$oferta_activa = $oferta['existe'];
}

// Aplicar la oferta sobre el total
$total_descuento = 0;
if ($oferta_activa) {
	$descuento = 0.10; // 10%
    $total_descuento = $total * (1 - $descuento);
}

$error = false;
$error_mensaje = '';

// Intentar eliminar los productos del carrito, cambiar la cantidad de productos y borrar la oferta (con el procedimiento almacenado)
try {
	$sql = "CALL pagarTodosLosCarritos();";
	    if ($conn->query($sql) === FALSE) {
        throw new Exception("Error en la consulta: " . $conn->error);
    }
	
// Si esto falla (osea, si el trigger da error)
} catch (Exception $e) {
	$error = true;
	$error_mensaje .= "Error en el carrito: " . htmlspecialchars($e->getMessage()) . " ";
}

// Si hay un error, redirigir a la página de error
if ($error) {
	$_SESSION['error'] = $error_mensaje;
	header("Location: error.php");
	exit();
}

// Cerrar la conexión a la BD
$conn->close();
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Gracias por tu compra</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
    <style>
        .strikethrough {
            text-decoration: line-through;
            color: red; /* Puedes cambiar el color si lo deseas */
        }
    </style>
</head>
<body>
    <!-- Barra superior -->
    <nav class="navbar navbar-light">
        <div class="container">
            <a class="navbar-brand" href="index.php">USteaM</a>
            <div class="ml-auto d-flex align-items-center">
                <a href="index.php" class="btn btn-primary mr-2">Página principal</a>
                <a href="carrito.php" class="btn btn-outline-primary">🛒 Carrito de Compras</a>
            </div>
        </div>
    </nav>

    <div class="container mt-4">
        <h1 class="text-center">¡Gracias por tu compra!</h1>
        <h2 class="text-center">Resumen de tu compra:</h2>
        <form method="POST" action="guardar_calificacion.php">
            <table class="table">
                <thead>
                    <tr>
                        <th>Nombre</th>
                        <th>Precio Unitario</th>
                        <th>Cantidad</th>
                        <th>Subtotal</th>
                        <th>Calificación ★</th>
                    </tr>
                </thead>
                <tbody>
                    <?php foreach ($compra_resumen as $producto) { ?>
                        <tr>
                            <td><?php echo htmlspecialchars($producto['nombre']); ?></td>
                            <td>$ <?php echo $producto['precio_unitario']; ?></td>
                            <td><?php echo $producto['cantidad']; ?></td>
                            <td>$ <?php echo $producto['subtotal']; ?></td>
                            <?php if ($producto['tipo_producto'] === 'videojuego') { ?>
                                <td>
                                    <select name="calificacion[<?php echo $producto['nombre']; ?>]" class="form-control" style="width: 100px;">
                                        <option value="1" <?php echo ($producto['calificacion'] == 1 || ($producto['calificacion'] === null && 5 == 1)) ? 'selected' : ''; ?>>1</option>
                                        <option value="2" <?php echo ($producto['calificacion'] == 2 || ($producto['calificacion'] === null && 5 == 2)) ? 'selected' : ''; ?>>2</option>
                                        <option value="3" <?php echo ($producto['calificacion'] == 3 || ($producto['calificacion'] === null && 5 == 3)) ? 'selected' : ''; ?>>3</option>
                                        <option value="4" <?php echo ($producto['calificacion'] == 4 || ($producto['calificacion'] === null && 5 == 4)) ? 'selected' : ''; ?>>4</option>
                                        <option value="5" <?php echo ($producto['calificacion'] == 5 || ($producto['calificacion'] === null && 5 == 5)) ? 'selected' : ''; ?>>5</option>
                                    </select>
                                </td>
                            <?php } ?>
                        </tr>
                    <?php } ?>
                </tbody>
            </table>
            <h3 class="<?php echo $oferta_activa ? 'strikethrough' : ''; ?>">Total: $ <?php echo $total; ?></h3>
			
			<?php if ($oferta_activa): ?>
                <h3>Total con Descuento: $ <?php echo $total_descuento; ?></h3>
                <p>Descuento aplicado: 10%</p>
            <?php endif; ?>
			
            <button type="submit" class="btn btn-primary">Guardar calificación y volver</button>
			<a href="index.php" class="btn btn-primary">Volver sin calificar</a>
        </form>
    </div>

    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.4/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>