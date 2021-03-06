/*
 * Joshua Hull (jhull@clemson.edu) and Alex Berk (aberk@clemson.edu)
 * CPSC 4040-001 Fall 2014 Final Project
 */

// Gain variables
var gainRedSliderValue = 1;
var gainGreenSliderValue = 1;
var gainBlueSliderValue = 1;
var gainGlobalSliderValue = 1;

// Bias variables
var biasRedSliderValue;
var biasGreenSliderValue;
var biasBlueSliderValue;
var biasGlobalSliderValue;

function submitPressed () {
  var submit = {};
  var filter = {};
  var gain = {};
  var bias = {};

  submit['filter'] = filter;
  submit['gain'] = gain;
  submit['bias'] = bias;

  var gainForm = document.getElementById('gainGlobal');
  if(gainForm[0].checked) {
    submit['gain']['global'] = true;
    submit['gain']['value'] = gainGlobalSliderValue;
  } else {
    submit['gain']['global'] = false;
    submit['gain']['r'] = gainRedSliderValue;
    submit['gain']['g'] = gainGreenSliderValue;
    submit['gain']['b'] = gainBlueSliderValue;
  }

  var biasForm = document.getElementById('biasGlobal');
  if(biasForm[0].checked) {
    submit['bias']['global'] = true;
    submit['bias']['value'] = biasGlobalSliderValue;
  } else {
    submit['bias']['global'] = false;
    submit['bias']['r'] = biasRedSliderValue;
    submit['bias']['g'] = biasGreenSliderValue;
    submit['bias']['b'] = biasBlueSliderValue;
  }

  var filterForm = document.getElementById('filtering');
  if(filterForm[0].checked) {
    submit['filter']['type'] = 'sharp';
  } else if(filterForm[1].checked) {
    submit['filter']['type'] = 'smooth';
  } else {
    submit['filter']['type'] = 'none';
  }

  if(filterForm[2].checked) {
    submit['filter']['strength'] = 'high';
  } else if(filterForm[3].checked) {
    submit['filter']['strength'] = 'medium';
  } else if(filterForm[4].checked) {
    submit['filter']['strength'] = 'low';
  } else {
    submit['filter']['strength'] = 'none';
  }

  var imageForm = document.getElementById('imageForm');
  var imageData = imageForm[1].files[0];
  var form = new FormData();

  form.append('image', imageData);
  form.append('metaData', JSON.stringify(submit));

  var xhr = new XMLHttpRequest();
  xhr.onload = function(e) {
    console.log(xhr.response);
    var response = JSON.parse(xhr.responseText);
    jQuery('#photoDiv').html('<img src=' + response['dataUrl'] + ' />');
  };

  xhr.onload.onprogess = function(e) {
    if(e.lenthComputable) {
      var percentComplete = (e.loaded/e.total) * 100;
      console.log(percentComplete + "% uploaded");
    }
  };
  xhr.open('post', 'upload.php', true);
  xhr.send(form);

}

jQuery(document).ready(function(){
  // Initalize values
  gainRedSliderValue = 1;
  gainGreenSliderValue = 1;
  gainBlueSliderValue = 1;
  gainGlobalSliderValue = 1;

  biasRedSliderValue = 0;
  biasGreenSliderValue = 0;
  biasBlueSliderValue = 0;
  biasGlobalSliderValue = 0;

  jQuery("#gainRedSliderLabel").text("Value: " + gainRedSliderValue);
  jQuery("#gainGreenSliderLabel").text("Value: " + gainRedSliderValue);
  jQuery("#gainBlueSliderLabel").text("Value: " + gainRedSliderValue);
  jQuery("#gainGlobalSliderLabel").text("Value: " + gainRedSliderValue);

  // Adjusting Gain

  // Red Channel
  jQuery("#gainRedSlider").slider({
    max: 2.501,
    min: -2.5,
    step: 0.1,
    value: 1.0,
    slide: function(event, ui) {
      gainRedSliderValue = ui.value;
      jQuery("#gainRedSliderLabel").text("Value: " + gainRedSliderValue);
    }
  }).css("background","#ff0000");

  // Green Channel
  jQuery("#gainGreenSlider").slider({
    max: 2.501,
    min: -2.5,
    step: 0.1,
    value: 1,
    slide: function(event, ui) {
      gainGreenSliderValue = ui.value;
      jQuery("#gainGreenSliderLabel").text("Value: " + gainGreenSliderValue);
    }
  }).css("background","#00ff00");

  // Blue Channel
  jQuery("#gainBlueSlider").slider({
    max: 2.501,
    min: -2.5,
    step: 0.1,
    value: 1,
    slide: function( event, ui ) {
      gainBlueSliderValue = ui.value;
      jQuery("#gainBlueSliderLabel").text("Value: " + gainBlueSliderValue);
    }
  }).css("background","#0000ff");

  // Global Adjustment
  jQuery("#gainGlobalSlider").slider({
    max: 2.501,
    min: -2.5,
    step: 0.1,
    value: 1,
    slide: function( event, ui ) {
      gainGlobalSliderValue = ui.value;
      jQuery("#gainGlobalSliderLabel").text("Value: " + gainGlobalSliderValue);
    }
  }).css("background","#000000");


  // Adjust Bias

  // Red Channel
  jQuery("#biasRedSlider").slider({
    max: 1,
    min: -1,
    step: 0.1,
    slide: function(event, ui) {
      biasRedSliderValue = ui.value;
      jQuery("#biasRedSliderLabel").text("Value: " + biasRedSliderValue);
    }
  }).css("background","#ff0000");

  // Green Channel
  jQuery("#biasGreenSlider").slider({
    max: 1,
    min: -1,
    step: 0.1,
    slide: function(event, ui) {
      biasGreenSliderValue = ui.value;
      jQuery("#biasGreenSliderLabel").text("Value: " + biasGreenSliderValue);
    }
  }).css("background","#00ff00");

  // Blue Channel
  jQuery("#biasBlueSlider").slider({
    max: 1,
    min: -1,
    step: 0.1,
    slide: function( event, ui ) {
      biasBlueSliderValue = ui.value;
      jQuery("#biasBlueSliderLabel").text("Value: " + biasBlueSliderValue);
    }
  }).css("background","#0000ff");

  // Global Adjustment
  jQuery("#biasGlobalSlider").slider({
    max: 1,
    min: -1,
    step: 0.1,
    slide: function( event, ui ) {
      biasGlobalSliderValue = ui.value;
      jQuery("#biasGlobalSliderLabel").text("Value: " + biasGlobalSliderValue);
    }
  }).css("background","#000000");

  // Image File Input
  $("#imageInput").fileinput({
    showCaption: false,
    showUpload: false,
    maxFileCount: 1,
    allowedFileTypes: ['image'],
    browseLabel: 'Select Image'
  });

  // Tooltips
  $('[data-toggle="tooltip"]').tooltip({'html': true});
});
