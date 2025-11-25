<?php
session_start();
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <title>Error</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
</head>
<body>

    <!-- Barra superior -->
    <nav class="navbar navbar-light">
        <div class="container">
            <a class="navbar-brand" href="index.php">USteaM</a>
            <div class="ml-auto">
                <a href="carrito.php" class="btn btn-primary">Volver</a>
            </div>
        </div>
    </nav>

    <div class="container mt-4">
        <h1 class="text-danger">Error</h1>
        <?php if (isset($_SESSION['error'])) { ?>
            <div class="alert alert-danger">
                <?php echo htmlspecialchars($_SESSION['error']); ?>
            </div>
            <?php unset($_SESSION['error']); // Limpiar el mensaje después de mostrarlo ?>
        <?php } else { ?>
            <p>No se ha proporcionado un mensaje de error.</p>
        <?php } ?>
        <a href="busqueda.php" class="btn btn-primary">Volver a buscar productos</a>
    </div>
</body>
</html>
