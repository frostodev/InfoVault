<?php
session_start();

// Conexión a la base de datos
$servername = "localhost";
$username = "pagina";
$password = "contraseña";
$dbname = "usteam";

// En caso de error al conectar
$conn = new mysqli($servername, $username, $password, $dbname);
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Inicializar variables
$searchTerm = '';
$precioMin = 0;
$precioMax = 1000000;
$soloConsolas = false;
$soloVideojuegos = false;
$clasificacion = '';

// Procesar búsqueda
if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    $searchTerm = $conn->real_escape_string($_POST['search']);
    $precioMin = isset($_POST['precio_min']) ? (int)$_POST['precio_min'] : 0;
    $precioMax = isset($_POST['precio_max']) ? (int)$_POST['precio_max'] : 1000000;
    $soloConsolas = isset($_POST['solo_consolas']);
    $soloVideojuegos = isset($_POST['solo_videojuegos']);
    $clasificacion = isset($_POST['clasificacion']) ? $conn->real_escape_string($_POST['clasificacion']) : '';

    // Redirigir para evitar reenvío del formulario
    header("Location: busqueda.php?search=".urlencode($searchTerm)."&precio_min=".$precioMin."&precio_max=".$precioMax."&solo_consolas=".($soloConsolas ? '1' : '0')."&solo_videojuegos=".($soloVideojuegos ? '1' : '0')."&clasificacion=".$clasificacion);
    exit();
}

// Recuperar parámetros de la URL
if (isset($_GET['search'])) {
    $searchTerm = $conn->real_escape_string($_GET['search']);
    $precioMin = isset($_GET['precio_min']) ? (int)$_GET['precio_min'] : 0;
    $precioMax = isset($_GET['precio_max']) ? (int)$_GET['precio_max'] : 1000000;
    $soloConsolas = isset($_GET['solo_consolas']) && $_GET['solo_consolas'] == '1';
    $soloVideojuegos = isset($_GET['solo_videojuegos']) && $_GET['solo_videojuegos'] == '1';
    $clasificacion = isset($_GET['clasificacion']) ? $conn->real_escape_string($_GET['clasificacion']) : '';
}

// Crear la consulta
$sql = "SELECT p.id_producto, p.nombre, p.precio, p.imagen, p.unidades_disponibles, p.tipo_producto,
            c.marca, c.formato, c.num_controles, v.clasificacion, v.calificacion, v.descripcion
        FROM Producto p 
        LEFT JOIN Consola c ON p.id_producto = c.id_producto 
        LEFT JOIN Videojuego v ON p.id_producto = v.id_producto 
        WHERE p.nombre LIKE '%$searchTerm%' 
        AND p.precio BETWEEN $precioMin AND $precioMax";

// Filtro para ver sólo consolas o sólo videojuegos
if ($soloConsolas && !$soloVideojuegos) {
    $sql .= " AND p.tipo_producto = 'consola'";
} elseif ($soloVideojuegos && !$soloConsolas) {
    $sql .= " AND p.tipo_producto = 'videojuego'";
}

// Filtro para la clasificación del videojuego
if (!empty($clasificacion)) {
    $sql .= " AND EXISTS (SELECT 1 FROM Videojuego v WHERE v.id_producto = p.id_producto AND v.clasificacion = '$clasificacion')";
}

// Ordenar por cantidad de unidades disponibles, en orden descendiente
$sql .= " ORDER BY p.unidades_disponibles DESC";

// Resultado de la consulta
$result = $conn->query($sql);

// Cerrar la conexión
$conn->close();
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>USteaM - Búsqueda</title>
    <link href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" rel="stylesheet">
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
        <h1 class="text-center">Buscar Productos</h1>
		<br>
		<br>

        <!-- Formulario de búsqueda -->
        <form method="POST" class="mb-4">
            <div class="input-group">
                <input type="text" name="search" class="form-control" placeholder="Buscar consolas o videojuegos..." value="<?php echo htmlspecialchars($searchTerm); ?>">
                <div class="input-group-append">
                    <button class="btn btn-primary" type="submit">Buscar</button>
                </div>
            </div>
			<br>
			<h3>Búsqueda Avanzada</h3>
            <div class="form-group">
                <label for="precio_min">Precio Mínimo ($):</label>
                <input type="number" name="precio_min" class="form-control" value="<?php echo htmlspecialchars($precioMin); ?>">
            </div>
            <div class="form-group">
                <label for="precio_max">Precio Máximo ($):</label>
                <input type="number" name="precio_max" class="form-control" value="<?php echo htmlspecialchars($precioMax); ?>">
            </div>
            <div class="form-check">
                <input type="checkbox" name="solo_consolas" class="form-check-input" <?php echo $soloConsolas ? 'checked' : ''; ?>>
                <label class="form-check-label">Sólo Consolas</label>
            </div>
            <div class="form-check">
                <input type="checkbox" name="solo_videojuegos" class="form-check-input" <?php echo $soloVideojuegos ? 'checked' : ''; ?>>
                <label class="form-check-label">Sólo Videojuegos</label>
            </div>
			<br>
            <div class="form-group">
                <label for="clasificacion">Clasificación:</label>
                <select name="clasificacion" class="form-control">
                    <option value="">Seleccione...</option>
                    <option value="RP" <?php echo $clasificacion == 'RP' ? 'selected' : ''; ?>>RP</option>
                    <option value="E" <?php echo $clasificacion == 'E' ? 'selected' : ''; ?>>E</option>
                    <option value="E10+" <?php echo $clasificacion == 'E10+' ? 'selected' : ''; ?>>E10+</option>
                    <option value="T" <?php echo $clasificacion == 'T' ? 'selected' : ''; ?>>T</option>
                    <option value="M" <?php echo $clasificacion == 'M' ? 'selected' : ''; ?>>M</option>
                    <option value="A" <?php echo $clasificacion == 'A' ? 'selected' : ''; ?>>A</option>
                </select>
            </div>
        </form>

        <h2>Resultados de Búsqueda</h2>
        <table class="table">
            <thead>
                <tr>
					<th align="center">Nombre</th>
					<th align="center">Imagen</th>
					<th align="center">Precio</th>
					<th align="center">Unidades Disponibles</th>
					<th align="center">Comprar</th>
                </tr>
            </thead> 
			<tbody>
				<?php if (isset($result) && $result->num_rows > 0) {
					while ($row = $result->fetch_assoc()) {
						?>
						<tr>
							<td><?php echo htmlspecialchars($row['nombre']); ?></td>
							<td>
								<img src="<?php echo htmlspecialchars($row['imagen']); ?>" alt="<?php echo htmlspecialchars($row['nombre']); ?>" style="width: 100px;">
							</td>
							<td><?php echo "$ " . htmlspecialchars($row['precio']); ?></td>
							<td>
								<?php if ($row['unidades_disponibles'] > 0) {
									echo htmlspecialchars($row['unidades_disponibles']);
								} else {
									echo "<span class='text-danger'>Agotado</span>";
								} ?>
							</td>
							<td>
								<?php if ($row['unidades_disponibles'] > 0) { ?>
									<button class="btn btn-primary" data-toggle="modal" data-target="#productModal"
											data-id="<?php echo $row['id_producto']; ?>"
											data-nombre="<?php echo htmlspecialchars($row['nombre']); ?>"
											data-precio="<?php echo htmlspecialchars($row['precio']); ?>"
											data-unidades="<?php echo $row['unidades_disponibles']; ?>"
											data-imagen="<?php echo htmlspecialchars($row['imagen']); ?>"
											data-marca="<?php echo htmlspecialchars($row['marca'] ?? ''); ?>"
											data-formato="<?php echo htmlspecialchars($row['formato'] ?? ''); ?>"
											data-num_controles="<?php echo htmlspecialchars($row['num_controles'] ?? ''); ?>"
											data-clasificacion="<?php echo htmlspecialchars($row['clasificacion'] ?? ''); ?>"
											data-calificacion="<?php echo htmlspecialchars($row['calificacion'] ?? ''); ?>"
											data-descripcion="<?php echo htmlspecialchars($row['descripcion'] ?? ''); ?>"
									>➕ Ver Detalles</button>
								<?php } else { ?>
									<button class="btn btn-secondary" disabled>Agotado</button>
								<?php } ?>
							</td>
						</tr>
						<?php
					}
				} else { ?>
					<tr><td colspan="5">No se encontraron resultados.</td></tr>
				<?php } ?>
			</tbody>
        </table>
    </div>

	<!-- Modal -->
	<div class="modal fade" id="productModal" tabindex="-1" role="dialog" aria-labelledby="productModalLabel" aria-hidden="true">
		<div class="modal-dialog" role="document">
			<div class="modal-content">
				<div class="modal-header">
					<h5 class="modal-title" id="productModalLabel">Detalles del Producto</h5>
					<button type="button" class="close" data-dismiss="modal" aria-label="Close">
						<span aria-hidden="true">&times;</span>
					</button>
				</div>
				<div class="modal-body">
					<img id="modalImage" src="" alt="" style="max-width: 100%; max-height: 400px;"/>
					<h4 id="modalNombre"></h4>
					<p id="modalPrecio"></p>
					<p id="modalUnidades"></p>

					<!-- Información adicional -->
					<div id="modalDetalles" style="display: none;">
						<p id="modalMarca"></p>
						<p id="modalFormato"></p>
						<p id="modalNumControles"></p>
						<p id="modalClasificacion"></p>
						<p id="modalCalificacion"></p>
						<p id="modalDescripcion"></p>
					</div>

					<form method="POST" action="añadir_carrito.php">
						<input type="hidden" name="id_producto" id="modalId" value="">
						<input type="hidden" name="precio_unitario" id="modalPrecioUnitario" value="">
						<div class="form-group">
							<label for="cantidad">Cantidad:</label>
							<input type="number" name="cantidad" id="modalCantidad" class="form-control" value="1" min="1" max="" required>
						</div>
						<button type="submit" class="btn btn-primary">Añadir al carrito</button>
					</form>
				</div>
			</div>
		</div>
	</div>

    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/@popperjs/core@2.5.4/dist/umd/popper.min.js"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>

	<!-- JavaScript para el manejo del Modal de los detalles del producto -->
    <script>
		$('#productModal').on('show.bs.modal', function (event) {
			var button = $(event.relatedTarget); // Botón que abrió el modal
			var id = button.data('id');
			var nombre = button.data('nombre');
			var precio = button.data('precio');
			var unidades = button.data('unidades');
			var imagen = button.data('imagen');
			var marca = button.data('marca');
			var formato = button.data('formato');
			var numControles = button.data('num_controles');
			var clasificacion = button.data('clasificacion');
			var calificacion = button.data('calificacion');
			var descripcion = button.data('descripcion');
			
			var modal = $(this);
			modal.find('#modalId').val(id);
			modal.find('#modalNombre').text(nombre);
			modal.find('#modalImage').attr('src', imagen);
			modal.find('#modalPrecio').html('<strong>Precio:</strong> $' + precio);
			modal.find('#modalUnidades').html('<strong>Unidades Disponibles:</strong> ' + unidades);
			modal.find('#modalPrecioUnitario').val(precio);
			modal.find('#modalCantidad').attr('max', unidades);

			// Limpiar todos los campos del modal
			modal.find('#modalMarca').text('');
			modal.find('#modalFormato').text('');
			modal.find('#modalNumControles').text('');
			modal.find('#modalClasificacion').text('');
			modal.find('#modalCalificacion').text('');
			modal.find('#modalDescripcion').text('');
			modal.find('#modalDetalles').hide();

			// Mostrar información adicional solo si existen
			if (marca || formato || numControles) {
				modal.find('#modalMarca').html('<strong>Marca:</strong> ' + (marca || 'N/A'));
				modal.find('#modalFormato').html('<strong>Formato:</strong> ' + (formato || 'N/A'));
				modal.find('#modalNumControles').html('<strong>Número de Controles:</strong> ' + (numControles || 'N/A'));
			}

			if (clasificacion || calificacion || descripcion) {
				modal.find('#modalClasificacion').html('<strong>Clasificación:</strong> ' + (clasificacion || 'N/A'));
				modal.find('#modalCalificacion').html('<strong>Calificación:</strong> ' + (calificacion || 'N/A') + ' ★');
				modal.find('#modalDescripcion').html('<strong>Descripción:</strong> ' + (descripcion || 'N/A'));
			}

			// Mostrar detalles solo si hay información
			if (marca || formato || numControles || clasificacion || calificacion || descripcion) {
				modal.find('#modalDetalles').show();
			}
		});
    </script>
	
</body>
</html>