CPSC 404 Final Project
======================
* Joshua Hull (jhull@clemson.edu)
* Alex Berk (aberk@clemson.edu)

Contributions
-------------
* Alex:
	* Backend - process.cpp (pixel.cpp provided by Josh)
* Josh:
	* Frontend/Webservices - index.html, about.html, js/index.js, upload.php

Setup
-----
**Requirements:**
* OpenImageIO
* Webserver with PHP
	* MAMP was used during our in class presentation - [MAMP Homepage](http://www.mamp.info)

* Download code into htdocs or the path that MAMP gives in it's preferences pane.
* See the included Makefile for build instructions. If OpenImageIO is installed with homebrew then no changes should be needed and you can simply type `make`.



process.cpp
-----------
###Compiling
Type `make` or see the included Makefile for compiling instructions if it doesn't work.
### Options
**I/O**
* -I *input_file*
	* Input file to process.
* -O *output_file*
	* Output file to write to.

**Gain Adjustments**
* -1 *float*
	* Adjust all channels by the specified gain. Overwrite individual
channels.
* -r *float*
	* Adjust only the red channel of the image by a specified gain.
* -g *float*
	* Adjust only the green channel of the image by a specified gain.
* -b *float*
	* Adjust only the blue channel of the image by a specified gain.

**Bias Adjustments**
* -2 *float*
	* Adjust all channels by the specified bias. Overwrited individual
channels.
* -r *float*
	* Adjust only the red channel of the image by a specified bias.
* -g *float*
	* Adjust only the green channel of the image by a specified bias.
* -b *float*
	* Adjust only the blue channel of the image by a specified bias.

**Filtering Options**
* -s
	* Smooth the image.
* -S
	* Sharpen the image.
* -# *1|2|3*
	* The strength of the filter with 1 being the smallest adjustment.
