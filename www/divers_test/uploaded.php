<!DOCTYPE html>
<html>

<head>
  <title>Upload and Delete File</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    /* votre code CSS ici */
  </style>
  <meta http-equiv="refresh" content="3;URL=./upload.html"> <!-- rafraîchit la page après 3 secondes -->
</head>

<body>
  <header>
    <h1>Upload and Delete File</h1>
  </header>
  <nav>
    <a href="./index.html">Homepage</a>
    <a href="upload.html">Upload</a>
    <a href="delete.php">My Files</a>
    <a href="./aboutus.html">About us</a>
  </nav>
  <h1>Upload de fichier</h1>
  <form action="upload.php" method="post" enctype="multipart/form-data">
    <label for="file">Sélectionnez un fichier à uploader :</label>
    <input type="file" name="file" id="file"><br><br>
    <input type="submit" value="Uploader">
  </form>
  <div id="message">
    <?php
      if(isset($_GET['upload']) && $_GET['upload'] == 'success') {
        echo '<span style="color:green;">Upload réussi !</span>';
      }
    ?>
  </div>
</body>

</html>
