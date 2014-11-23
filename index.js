jQuery(document).ready(function(){
  var redSliderValue;
  var greenSliderValue;
  var blueSliderValue;
  jQuery("#redSlider").slider(
    {
      max: 255,
      min: 0,
      slide: function(event, ui) {
        redSliderValue = ui.value;
        jQuery("#redSliderLabel").text("Value: " + redSliderValue);
      },
      create: function(event, ui) {
        ui.value = 0;
      }
    }).css("background","#ff0000");

    jQuery("#greenSlider").slider(
      {
        max: 255,
        min: 0,
        slide: function(event, ui) {
          greenSliderValue = ui.value;
          jQuery("#greenSliderLabel").text("Value: " + greenSliderValue);
        },
        create: function(event, ui) {
          ui.value = 0;
        }
      }).css("background","#00ff00");

      jQuery("#blueSlider").slider(
        {
          max: 255,
          min: 0,
          slide: function( event, ui ) {
            blueSliderValue = ui.value;
            jQuery("#blueSliderLabel").text("Value: " + blueSliderValue);
          },
          create: function(event, ui) {
            ui.value = 0;
          }
        }).css("background","#0000ff");
      });
