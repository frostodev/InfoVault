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

// Obtener los datos del producto
$id_producto = $_POST['id_producto'];
$cantidad = $_POST['cantidad'];

// Consultar la base de datos para obtener el precio y unidades disponibles
$sql = "SELECT precio, unidades_disponibles FROM Producto WHERE id_producto = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $id_producto);
$stmt->execute();
$stmt->bind_result($precio_unitario, $unidades_disponibles);
$stmt->fetch();
$stmt->close();

// Consultar si el producto ya está en el carrito
$sql = "SELECT cantidad FROM Carrito WHERE id_producto = ?";
$stmt = $conn->prepare($sql);
$stmt->bind_param("i", $id_producto);
$stmt->execute();
$stmt->bind_result($cantidad_existente);
$stmt->fetch();
$stmt->close();

// Si el producto ya existe, actualizar la cantidad y el precio total
if ($cantidad_existente !== null) {
    $nueva_cantidad = $cantidad_existente + $cantidad;
    $precio_total = $precio_unitario * $nueva_cantidad;

    $sql = "UPDATE Carrito SET cantidad = ?, precio_total = ? WHERE id_producto = ?";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("idi", $nueva_cantidad, $precio_total, $id_producto);
    $stmt->execute();
    $stmt->close();
} else {
	
    // Calcular el precio total para el nuevo producto
    $precio_total = $precio_unitario * $cantidad;

    // Añadir el producto a la tabla Carrito
    $sql = "INSERT INTO Carrito (id_producto, cantidad, precio_unitario, precio_total) VALUES (?, ?, ?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("iidd", $id_producto, $cantidad, $precio_unitario, $precio_total); 
    $stmt->execute();
    $stmt->close();
}

// Redirigir de vuelta a la página de búsqueda
header("Location: busqueda.php");
exit();
?>
