<?php
// Déclaration de variables
$message = "Bonjour, monde !";
$nombre = 42;

// Affichage de variables
echo $message . "<br>";
echo "Le nombre est : " . $nombre . "<br>";

// Condition
if ($nombre > 50) {
    echo "Le nombre est supérieur à 50.<br>";
} else {
    echo "Le nombre est inférieur ou égal à 50.<br>";
}

// Boucle
for ($i = 0; $i < 5; $i++) {
    echo "Boucle " . ($i + 1) . "<br>";
}
?>