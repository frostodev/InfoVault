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

// Obtener calificaciones del formulario
if (isset($_POST['calificacion'])) {
    foreach ($_POST['calificacion'] as $nombre_producto => $calificacion) {
        // Obtener el id_producto basado en el nombre (no es la mejor opción, pero asi noma)
        $sql = "SELECT id_producto FROM Producto WHERE nombre = ?";
        $stmt = $conn->prepare($sql);
        $stmt->bind_param('s', $nombre_producto);
        $stmt->execute();
        $result = $stmt->get_result();
        $producto = $result->fetch_assoc();

        if ($producto) {
            $id_producto = $producto['id_producto'];
            // Actualizar calificación ponderada
            $sql = "UPDATE Videojuego SET calificacion = (SELECT AVG(calificacion) FROM (
                        SELECT calificacion FROM Videojuego WHERE id_producto = ?
                        UNION ALL
                        SELECT ?) AS temp) WHERE id_producto = ?";
            $stmt = $conn->prepare($sql);
            $stmt->bind_param('iii', $id_producto, $calificacion, $id_producto);
            $stmt->execute();
        }
    }
}

$conn->close();

// Redirigir a la página de inicio
header("Location: index.php");
exit;
?>
