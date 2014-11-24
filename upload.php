/*
 * Joshua Hull (jhull@clemson.edu) and Alex Berk (aberk@clemson.edu)
 * CPSC 4040-001 Fall 2014 Final Project
 */
<?php
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
    print_r($_FILES);
    print_r($_POST);
    $originalName = $_FILES['image']['tmp_name'];
    $newName = $originalName;
    //$newName = tmpfile();

    $meta = json_decode($_FILES['metaData'], true);
    $globalGain = $meta['gain']['global'];
    $globalBias = $meta['bias']['bias'];

    $parameters = '';
    if($globalGain) {
      $parameters = $parameters . ' --globalGain ' . $meta['gain']['global'];
    } else {
      $parameters = $parameters . ' --redGain ' . $meta['gain']['r'];
      $parameters = $parameters . ' --greenGain ' . $meta['gain']['g'];
      $parameters = $parameters . ' --blueGain ' . $meta['gain']['b'];
    }

    if($globalBias) {
      $parameters = $parameters . ' --globalBias '. $meta['bias']['global'];
    } else {
      $parameters = $parameters . ' --redBias ' . $meta['bias']['r'];
      $parameters = $parameters . ' --greenBias ' . $meta['bias']['g'];
      $parameters = $parameters . ' --blueBias ' . $meta['bias']['b'];
    }

    if(strcmp($meta['filter']['type'],'smooth') == 0) {
      $parameters = $parameters . ' --smooth';
      switch ($meta['filter']['strength']) {
        case 'high':
          $parameters = $parameters . ' --strength 3';
          break;
        case 'medium':
          $parameters = $parameters . ' --strength 2';
          break;
        case 'low':
          $parameters = $parameters . ' --strength 1';
          break;
        default:
          break;
      }
    } else if(strcmp($meta['filter']['type'],'sharp') == 0) {
      $parameters = $parameters . ' --sharp';
      switch ($meta['filter']['strength']) {
        case 'high':
          $parameters = $parameters . ' --strength 3';
          break;
        case 'medium':
          $parameters = $parameters . ' --strength 2';
          break;
        case 'low':
          $parameters = $parameters . ' --strength 1';
          break;
        default:
          break;
      }
    }
    //exec('process' . $parameters);


    $fileContent = file_get_contents($newName);
    $finfo_open(FINFO_MEME_TYPE);
    $dataUrl = 'data:' . finfo_file($finfo,$newName) . ';base64,' . base64_encode($fileContent);
    $json = json_encode(array(
      'dataUrl' => $dataUrl
    ));

    //fclose($newName);
    echo $json;
  };
?>
