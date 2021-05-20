<?php   
    //Crea un archivo de texto para guardar los datos que envía el ESP8266
    if (!file_exists("miTemp&Hum.txt")){
        // si no existe el archivo, lo creamos
        file_put_contents("miTemp&Hum.txt", "0.0\r\n0.0");
    }
    
    // Si se recibe Datos con el Método GET, los procesamos
    if (isset($_GET['Temp']) && isset($_GET['Hum'])){
        $var3 = $_GET['Temp'];
        $var4 = $_GET['Hum'];
        $fileContent = $var3 . "\r\n" . $var4;
        $fileSave = file_put_contents("miTemp&Hum.txt", $fileContent);
    }
   
    // Leemos los datos del archivo para guardarlos en variables
    $fileStr = file_get_contents("miTemp&Hum.txt");
    $pos1 = strpos($fileStr, "\r\n");
    $var1 = substr($fileStr, 0, $pos1);
    $var2 = substr($fileStr, $pos1 + 1); // de la pos1 +1 hasta el final
?>

<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="refresh" content="15">
    <title>IOTPROYECT</title>

</head>
    <style>
        h1 {
            color: antiquewhite;
            background-color: dodgerblue;
            text-align: center;
        }
    </style>
<body>
    <header>
        <h1>BIENVENIDOS AL SERVIDOR LOCAL</h1>
    </header>
    
    <section>
        <h3>Temperatura: <?php echo $var1; ?></h3>
        <h3>Humedad: <?php echo $var2; ?></h3>
    </section>
</body>
</html>
