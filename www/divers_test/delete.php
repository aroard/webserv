<?php
if ($_SERVER['REQUEST_METHOD'] === 'DELETE') {
    // Récupérer le nom de fichier à supprimer depuis l'URI
    $filename = basename($_SERVER['REQUEST_URI']);

    // Vérifier si le fichier existe et peut être supprimé
    if (file_exists($filename) && is_writable($filename)) {
        // Supprimer le fichier
        unlink($filename);
        http_response_code(204); // Réponse "No Content"
    } else {
        http_response_code(404); // Réponse "Not Found"
    }
} else {
    http_response_code(405); // Réponse "Method Not Allowed"
}
?>
