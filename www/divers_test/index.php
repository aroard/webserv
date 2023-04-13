<?php
    $path = '../tools/ma_photo.jpg';
    if (file_exists($path)) {
      echo '<a href="' . $path . '" download>Télécharger la photo</a>';
    }
  ?>