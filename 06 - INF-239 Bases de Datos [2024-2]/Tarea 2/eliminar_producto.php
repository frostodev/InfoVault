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

// Verificar si se ha enviado un ID de producto
if (isset($_POST['id_producto'])) {
    $id_producto = (int)$_POST['id_producto'];

    // Eliminar el producto del carrito
    $sql = "DELETE FROM Carrito WHERE id_producto = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("i", $id_producto);
    $stmt->execute();
    $stmt->close();
}

// Redirigir de vuelta al carrito
header("Location: carrito.php");
exit();
?>
