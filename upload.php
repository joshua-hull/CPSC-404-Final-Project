<?php
  /*
  * Joshua Hull (jhull@clemson.edu) and Alex Berk (aberk@clemson.edu)
  * CPSC 4040-001 Fall 2014 Final Project
  */
  if($_SERVER['REQUEST_METHOD'] == 'POST') {
    $originalName = $_FILES['image']['tmp_name'];
    $ext = pathinfo($_FILES['image']['name'], PATHINFO_EXTENSION);

    rename($originalName, $originalName . $ext);

    $originalName = $originalName . $ext;

    $newName = tmpfile();
    rename($newName, $newName . '.png');
    $newName = $newName . '.png';

    $meta = json_decode($_POST['metaData'], true);
    $globalGain = $meta['gain']['global'];
    $globalBias = $meta['bias']['global'];

    $parameters = '';
    $parameters = $parameters . '-I ' . $originalName;
    $parameters = $parameters . '-O ' . $newName;
    if($globalGain) {
      $parameters = $parameters . ' -1 ' . $meta['gain']['global'];
    } else {
      $parameters = $parameters . ' -r ' . $meta['gain']['r'];
      $parameters = $parameters . ' -g ' . $meta['gain']['g'];
      $parameters = $parameters . ' -b ' . $meta['gain']['b'];
    }

    if($globalBias) {
      $parameters = $parameters . ' -2 '. $meta['bias']['global'];
    } else {
      $parameters = $parameters . ' -R ' . $meta['bias']['r'];
      $parameters = $parameters . ' -G ' . $meta['bias']['g'];
      $parameters = $parameters . ' -B ' . $meta['bias']['b'];
    }

    if(strcmp($meta['filter']['type'],'smooth') == 0) {
      $parameters = $parameters . ' -s';
      switch ($meta['filter']['strength']) {
        case 'high':
          $parameters = $parameters . ' -# 3';
          break;
        case 'medium':
          $parameters = $parameters . ' -# 2';
          break;
        case 'low':
          $parameters = $parameters . ' -# 1';
          break;
        default:
          break;
      }
    } else if(strcmp($meta['filter']['type'],'sharp') == 0) {
      $parameters = $parameters . ' -S';
      switch ($meta['filter']['strength']) {
        case 'high':
          $parameters = $parameters . ' -# 3';
          break;
        case 'medium':
          $parameters = $parameters . ' -# 2';
          break;
        case 'low':
          $parameters = $parameters . ' -# 1';
          break;
        default:
          break;
      }
    }

    $output = array();
    putenv('LD_LIBRARY_PATH=/group/dpa/lib');
    exec('/web/home/jhull/public_html/CPSC404/process' . $parameters. ' 2>&1', $output, $retval);


    $fileContent = @file_get_contents($newName);
    $finfo = finfo_open(FILEINFO_MIME_TYPE);
    $dataUrl = 'data:' . @finfo_file($finfo,$newName) . ';base64,' . base64_encode($fileContent);
    $json = json_encode(array(
      'dataUrl' => $dataUrl,
      'debug' => array(
          'parameters' => $parameters,
          'output' => $output,
          'returnValue' => $retval
        )
    ));

    //fclose($newName);
    echo $json;
  };
?>
