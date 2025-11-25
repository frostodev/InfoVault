<?php
session_start();

// Conexión a la base de datos
$servername = "localhost";
$username = "pagina";
$password = "contraseña";
$dbname = "usteam";

$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Generar oferta
$discountOffer = false;
if (rand(1, 100) <= 20) { // 20% de probabilidad
    $discountOffer = true;
}

// Procesar aceptación o rechazo de la oferta
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['accept_offer'])) {

        // Actualizar el estado de la oferta en la base de datos
        $updateOfferSql = "UPDATE Oferta SET existe = true WHERE id_oferta = 1";
        $conn->query($updateOfferSql);
		
    } elseif (isset($_POST['reject_offer'])) {
        $updateOfferSql = "UPDATE Oferta SET existe = false WHERE id_oferta = 1";
        $conn->query($updateOfferSql);
    }
}

// Procesar búsqueda
$searchTerm = '';
if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['search'])) {
    $searchTerm = $conn->real_escape_string($_POST['search']);
}

// Consultas para consolas y videojuegos
$sqlConsolas = "SELECT p.id_producto, p.nombre, p.precio, p.imagen, p.unidades_disponibles 
                FROM Producto p 
                JOIN Consola c ON p.id_producto = c.id_producto 
                WHERE p.tipo_producto = 'consola' AND p.nombre LIKE '%$searchTerm%' 
                ORDER BY p.unidades_disponibles DESC LIMIT 3";

$resultConsolas = $conn->query($sqlConsolas);

$sqlVideojuegos = "SELECT p.id_producto, p.nombre, p.precio, p.imagen, p.unidades_disponibles 
                   FROM Producto p 
                   JOIN Videojuego v ON p.id_producto = v.id_producto 
                   WHERE p.tipo_producto = 'videojuego' AND p.nombre LIKE '%$searchTerm%' 
                   ORDER BY p.unidades_disponibles DESC LIMIT 3";

$resultVideojuegos = $conn->query($sqlVideojuegos);

// Cerrar la conexión
$conn->close();
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>USteaM</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>

    <!-- Barra superior -->
    <nav class="navbar navbar-light">
        <div class="container">
            <a class="navbar-brand" href="index.php">USteaM</a>
            <div class="ml-auto d-flex align-items-center">
                <a href="admin.php" class="btn btn-secondary mr-2">⚙️ Administración</a>
                <a href="carrito.php" class="btn btn-outline-primary">🛒 Carrito de Compras</a>
            </div>
        </div>
    </nav>

    <div class="container mt-4">
        <h1 class="text-center">Bienvenidos a USteaM</h1> 
        
        <!-- Oferta espontánea -->
        <?php if ($discountOffer): ?>
            <div class="alert alert-warning text-center">
                <strong>¡💸 Tienes una oferta especial!</strong> Acepta para obtener un descuento del <strong>10%</strong> en el total de tu compra 🤩.
                <form method="POST" action="index.php">
                    <button type="submit" name="accept_offer" class="btn btn-success">Aceptar</button>
                    <button type="submit" name="reject_offer" class="btn btn-danger">Rechazar</button>
                </form>
            </div>
        <?php endif; ?>
        
        <!-- Barra de búsqueda -->
        <h1>Productos</h1>
        <form method="POST" class="mb-4" action="busqueda.php">
            <div class="input-group">
                <input type="text" name="search" class="form-control" placeholder="Buscar consolas o videojuegos..." value="<?php echo htmlspecialchars($searchTerm ?? ''); ?>">
                <div class="input-group-append">
                    <button class="btn btn-primary" type="submit">Buscar</button>
                </div>
            </div>
        </form>

        <h2>Top Consolas</h2>
        <table class="table">
            <thead>
                <tr>
                    <th>Nombre</th>
                    <th>Imagen</th>
                    <th>Precio</th>
                    <th>Unidades Disponibles</th>
                </tr>
            </thead>
            <tbody>
                <?php if (isset($resultConsolas) && $resultConsolas->num_rows > 0) {
                    while ($row = $resultConsolas->fetch_assoc()) { ?>
                        <tr>
                            <td><?php echo htmlspecialchars($row['nombre']); ?></td>
                            <td><img src="<?php echo htmlspecialchars($row['imagen']); ?>" style="width: 100px;"></td>
                            <td>$ <?php echo htmlspecialchars($row['precio']); ?></td>
                            <td><?php echo htmlspecialchars($row['unidades_disponibles']); ?></td>
                        </tr>
                    <?php }
                } else { ?>
                    <tr><td colspan="4">No se encontraron consolas.</td></tr>
                <?php } ?>
            </tbody>
        </table>

        <h2>Top Videojuegos</h2>
        <table class="table">
            <thead>
                <tr>
                    <th>Nombre</th>
                    <th>Imagen</th>
                    <th>Precio</th>
                    <th>Unidades Disponibles</th>
                </tr>
            </thead>
            <tbody>
                <?php if (isset($resultVideojuegos) && $resultVideojuegos->num_rows > 0) {
                    while ($row = $resultVideojuegos->fetch_assoc()) { ?>
                        <tr>
                            <td><?php echo htmlspecialchars($row['nombre']); ?></td>
                            <td><img src="<?php echo htmlspecialchars($row['imagen']); ?>" style="width: 100px;"></td>
                            <td>$ <?php echo htmlspecialchars($row['precio']); ?></td>
                            <td><?php echo htmlspecialchars($row['unidades_disponibles']); ?></td>
                        </tr>
                    <?php }
                } else { ?>
                    <tr><td colspan="4">No se encontraron videojuegos.</td></tr>
                <?php } ?>
            </tbody>
        </table>
    </div>

    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.4/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
</body>
</html>
