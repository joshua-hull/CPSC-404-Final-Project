jQuery(document).ready(function(){
  var redSlider;
  jQuery( "#slider" ).slider(
    {
      max: 255,
      min: 0,
      change: function( event, ui ) {
        redSlider = ui.value;
        console.log("slider value changed: " . redSlider);
        jQuery("#sliderLabel").text(redSlider);
      }
    });
  });
