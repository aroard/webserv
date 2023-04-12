<?php
    // Répertoire contenant les fichiers à télécharger
    $directory = "../upload/";
    
    // Liste tous les fichiers dans le répertoire
    $files = array_diff(scandir($directory), array('.', '..'));
    
    // Parcourt tous les fichiers et affiche des liens de téléchargement
    foreach ($files as $file) {
        echo '<div class="file-container">';
        echo '<span>' . $file . '</span>';
        echo '<a href="download.php?file=' . urlencode($directory . $file) . '"><button>Télécharger</button></a>';
        echo '</div>';
    }
?>

<!-- Le code HTML pour le dialogue de sélection de fichier -->
<div id="file-dialog" style="display: none;">
    <input type="file" id="file-input">
</div>

<script>
    // Fonction pour afficher le dialogue de sélection de fichier
    function showFileDialog() {
        var fileInput = document.getElementById("file-input");
        fileInput.addEventListener("change", function() {
            var files = fileInput.files;
            if (files.length > 0) {
                var file = files[0];
                window.location.href = "download.php?file=" + encodeURIComponent(file.path);
            }
        });
        fileInput.click();
    }
</script>
