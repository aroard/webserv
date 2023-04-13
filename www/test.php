<?php
// Nom du dossier à afficher
$dir = '../upload/';

// Vérification que le dossier existe
if(is_dir($dir)) {

  // Ouverture du dossier
  if($dh = opendir($dir)) {

    // Parcours des fichiers du dossier
    while(($file = readdir($dh)) !== false) {
      // Vérification que le fichier est un fichier (et non un dossier)
      if(is_file($dir.$file)) {
        // Affichage du nom du fichier avec un lien pour le télécharger
        echo '<a href="?file='.$file.'">'.$file.'</a><br>';
      }
    }

    // Fermeture du dossier
    closedir($dh);
  }
}

// Vérification que le paramètre "file" est présent dans l'URL
if(isset($_GET['file'])) {
  // Récupération du nom du fichier depuis l'URL
  $file = $_GET['file'];

  // Vérification que le fichier existe
  if(file_exists($dir.$file)) {
    // Envoi du fichier au navigateur
    header('Content-Type: application/octet-stream');
    header('Content-Disposition: attachment; filename="'.$file.'"');
    readfile($dir.$file);
    exit;
  }
}
?>
