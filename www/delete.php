<?php
$dir = "./upload/";

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
                            }
                            .file-container {
                                display: flex;
                                justify-content: space-between;
                                align-items: center;
                                padding: 10px;
                                background-color: white;
                                margin-bottom: 10px;
                                list-style-type: none;
                            }
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
                                width: 60%;
                            }
                            .file-info {
                                display: flex;
                                align-items: center;
                                width: 40%;
                            }
                            .file-info form {
                                margin: 0;
                            }
                            .file-info input[type="submit"] {
                                background-color: #dc3545;
                                color: white;
                                border: none;
                                padding: 5px 10px;
                                cursor: pointer;
                                margin-right: 10px;
                            }
                            .file-info a {
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
                            <a href="./upload.html">Upload</a>
                            <a href="./delete.php">My Files</a>
		                    <a href="./aboutus.html">About us</a>
		                    <a href="./authenticated.html">Authentication</a>
                        </nav>
                        <section>';
        while (($file = readdir($dh)) !== false) {
            if ($file != "." && $file != "..") {
                echo '<div class="file-container">';
                echo '<span>' . $file . '</span>';
                echo '<form method="POST" action="' . urldecode($dir . $file) . '">';
                echo '<input type="hidden" name="Methode-http" value="DELETE">';
                echo '<input type="submit" name="delete" value="Delete">';
                echo '</form>';
                echo '<form method="POST" action="' . urldecode($dir . $file) . '">';
                echo '<input type="hidden" name="Methode-http" value="REDIRECT">';
                echo '<input type="submit" name="redirect" value="View">';
                echo '</form>';
                echo '</div>';
            }
        }
        
    }
}