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

// Verificar si el carrito está vacío
$sql = "SELECT c.id_producto, c.cantidad, c.precio_unitario, p.nombre, p.imagen 
        FROM Carrito c 
        JOIN Producto p ON c.id_producto = p.id_producto";
$result = $conn->query($sql);

$carrito_vacio = $result->num_rows === 0;

// Verificar si hay una oferta activa
$sqlOferta = "SELECT existe FROM Oferta WHERE id_oferta = 1";
$resultOferta = $conn->query($sqlOferta);
$oferta_activa = false;
if ($resultOferta->num_rows > 0) {
    $oferta = $resultOferta->fetch_assoc();
    $oferta_activa = $oferta['existe'];
}

// Procesar la finalización de compra
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['finalizar_compra'])) {

    // Resumen de la compra
    $compra_resumen = [];
    
    // Reiniciar el resultado para recorrer nuevamente
    $result->data_seek(0);
    while ($row = $result->fetch_assoc()) {
        $compra_resumen[] = [
            'nombre' => $row['nombre'],
            'precio_unitario' => $row['precio_unitario'],
            'cantidad' => $row['cantidad'],
        ];
    }

	// Redirigir a la página de agradecimiento
    $_SESSION['compra_resumen'] = $compra_resumen;
    header('Location: gracias.php');
    exit;
}
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>USteaM - Carrito</title>
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
            <div class="ml-auto">
                <a href="busqueda.php" class="btn btn-primary">Volver</a>
            </div>
        </div>
    </nav>

    <div class="container mt-4">
        <h1 class="text-center">Carrito de Compras</h1>

        <?php if ($carrito_vacio) { ?>
            <p class="text-center">Tu carrito está vacío. 😞</p>
            <a href="busqueda.php" class="btn btn-primary">Volver a buscar productos</a>
        <?php } else { ?>
            <table class="table">
                <thead>
                    <tr>
                        <th>Nombre</th>
                        <th>Imagen</th>
                        <th>Precio Unitario</th>
                        <th>Cantidad</th>
                        <th>Subtotal</th>
                        <th>Acciones</th>
                    </tr>
                </thead>
                <tbody>
                    <?php
                    $total = 0;

                    while ($row = $result->fetch_assoc()) {
                        $subtotal = $row['precio_unitario'] * $row['cantidad'];
                        $total += $subtotal;
                    ?>
                        <tr>
                            <td><?php echo htmlspecialchars($row['nombre']); ?></td>
                            <td><img src="<?php echo htmlspecialchars($row['imagen']); ?>" alt="<?php echo htmlspecialchars($row['nombre']); ?>" style="width: 100px;"></td>
                            <td>$ <?php echo $row['precio_unitario']; ?></td>
                            <td><?php echo $row['cantidad']; ?></td>
                            <td>$ <?php echo $subtotal; ?></td>
                            <td>
                                <form method="POST" action="eliminar_producto.php">
                                    <input type="hidden" name="id_producto" value="<?php echo $row['id_producto']; ?>">
                                    <button class="btn btn-danger" type="submit">Eliminar</button>
                                </form>
                            </td>
                        </tr>
                    <?php } ?>
                </tbody>
            </table>

            <h3 class="<?php echo $oferta_activa ? 'strikethrough' : ''; ?>">Total: $ <?php echo $total; ?></h3>

            <?php if ($oferta_activa): 
                $descuento = 0.10; // 10%
                $total_con_descuento = $total * (1 - $descuento);
            ?>
                <h3>Total con Descuento: $ <?php echo $total_con_descuento; ?></h3>
                <p>Descuento aplicado: 10%</p>
            <?php endif; ?>

            <form method="POST">
                <button class="btn btn-success" type="submit" name="finalizar_compra">Pagar</button>
            </form>
        <?php } ?>
    </div>

    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.4/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>