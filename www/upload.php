<?php
// Vérifier si un fichier a été envoyé
if(isset($_FILES['photo'])) {
	// Vérifier si le fichier ne présente pas d'erreurs
	if($_FILES['photo']['error'] === UPLOAD_ERR_OK) {
		// Vérifier le type du fichier (ici, une image)
		$allowed_types = array('image/jpeg', 'image/png', 'image/gif');
		if(in_array($_FILES['photo']['type'], $allowed_types)) {
			// Copier le fichier dans le répertoire "uploads" du serveur
			move_uploaded_file($_FILES['photo']['tmp_name'], 'uploads/' . $_FILES['photo']['name']);
			echo 'Le fichier a été uploadé avec succès !';
		} else {
			echo 'Le fichier n\'est pas une image valide.';
		}
	} else {
		echo 'Une erreur est survenue lors de l\'upload du fichier.';
	}
}
?>
