<?php
// Vérifie si le formulaire a été soumis
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["fileToUpload"])) {
    
    // Chemin où le fichier sera enregistré
    $target_dir = "uploads/";
    
    // Nom du fichier sur le serveur
    $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
    
    // Vérifie si le fichier existe déjà sur le serveur
    if (file_exists($target_file)) {
        echo "Le fichier existe déjà.";
        exit;
    }
    
    // Vérifie la taille du fichier
    if ($_FILES["fileToUpload"]["size"]  > 500000) {
        echo "Le fichier est trop volumineux.";
        exit;
    }
    
    // Vérifie le type de fichier
    $fileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
    if($fileType != "pdf" && $fileType != "doc" && $fileType != "docx") {
        echo "Seuls les fichiers PDF, DOC et DOCX sont autorisés.";
        exit;
    }
    
    // Télécharge le fichier sur le serveur
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        echo "Le fichier ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " a été téléchargé sur le serveur.";
        
        // Télécharge le fichier depuis le serveur
        header('Content-Description: File Transfer');
        header('Content-Type: application/octet-stream');
        header('Content-Disposition: attachment; filename="'.basename($target_file).'"');
        header('Expires: 0');
        header('Cache-Control: must-revalidate');
        header('Pragma: public');
        header('Content-Length: ' . filesize($target_file));
        readfile($target_file);
        exit;
    } else {
        echo "Une erreur s'est produite lors du téléchargement du fichier.";
    }
}
?>

<!-- Formulaire d'upload de fichier -->
<form action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" enctype="multipart/form-data">
    <input type="file" name="fileToUpload" id="fileToUpload">
    <input type="submit" value="Uploader le fichier" name="submit">
</form>
