<?php
// Chemin du dossier à afficher
$dir = "/home/aroard/Bureau/webserv/upload/";

// Ouvrir le dossier
if (is_dir($dir)) {
    if ($dh = opendir($dir)) {

        // Afficher le nom de chaque fichier
        while (($file = readdir($dh)) !== false) {
            if ($file != "." && $file != "..") {
                echo "<div>";
                echo $file;
                echo "<form method=\"POST\" action=\"" . urldecode($dir . $file) . "\">";
                echo "<input type=\"hidden\" name=\"Methode-http\" value=\"DELETE\">";
                echo "<input type=\"submit\" name=\"delete\" value=\"Delete\">";
                echo "</form>";
                echo "</div>";
            }
        }

        // Fermer le dossier
        closedir($dh);
    }
}
?>
