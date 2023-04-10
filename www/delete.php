<?php
// Chemin du dossier à afficher
$dir = "/home/aroard/Bureau/webserv/upload/";

// Ouvrir le dossier
if (is_dir($dir)) {
    if ($dh = opendir($dir)) {
        echo '<!DOCTYPE html>
                <html>
                    <head>
                        <title>Mon site</title>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <style>
                            body {
                                font-family: Arial, sans-serif;
                                background-color: #f2f2f2;
                                margin: 0;
                            }
                            header {
                                background-color: #333;
                                color: white;
                                padding: 20px;
                                text-align: center;
                            }
                            h1 {
                                font-size: 40px;
                                margin: 0;
                            }
                            nav {
                                background-color: #444;
                                color: white;
                                padding: 10px;
                                text-align: center;
                            }
                            nav a {
                                color: white;
                                text-decoration: none;
                                margin: 10px;
                            }
                            section {
                                padding: 20px;
                                text-align: center;
                                display: flex;
                                justify-content: center;
                            }
                            section img {
                                max-width: 100%;
                                max-height: 100%;
                            }
                            .file-container {
                                display: flex;
                                justify-content: space-between;
                                align-items: center;
                                padding: 10px;
                                background-color: white;
                                margin-bottom: 10px;
                            }
                            .file-container span {
                                font-weight: bold;
                                margin-right: 10px;
                            }
                            .file-container form {
                                margin: 0;
                            }
                            .file-container input[type="submit"] {
                                background-color: #dc3545;
                                color: white;
                                border: none;
                                padding: 5px 10px;
                                cursor: pointer;
                                margin-right: 10px;
                            }
                            .file-container a {
                                background-color: #007bff;
                                color: white;
                                border: none;
                                padding: 5px 10px;
                                cursor: pointer;
                                text-decoration: none;
                            }
                        </style>
                    </head>
                    <body>
                        <header>
                            <h1>My Files</h1>
                        </header>
                        <nav>
                            <a href="./index.html">Homepage</a>
                            <a href="./aboutus.html">About us</a>
                            <a href="upload.html">Upload</a>
                            <a href="delete.php">My Files</a>
                        </nav>
                        <section>';
        // Afficher le nom, le bouton de suppression et le bouton de téléchargement de chaque fichier
        while (($file = readdir($dh)) !== false) {
            if ($file != "." && $file != "..") {
                echo '<div class="file-container">';
                echo '<span>' . $file . '</span>';
                echo '<form method="POST" action="' . urldecode($dir . $file) . '">';
                echo '<input type="hidden" name="Methode-http" value="DELETE">';
                echo '<input type="submit" name="delete" value="Delete">';
                echo '</form>';
                echo '<a href="' . urldecode($dir . $file) . '" download="' . $file . '">Download</a>';
                echo '</div>';
            }
        }
    }
}
