/**
 * Joshua Hull (jhull@clemson.edu) and Alex Berk (aberk@clemson.edu)
 * CPSC 4040-001 Fall 2014 Final Project
 */

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include "pixel.h"
#include <OpenImageIO/imageio.h>
#include <array>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

OIIO_NAMESPACE_USING

int width;
int height;
int channels;
static std::vector<float> oiioPixels;       // oiio pixels to read in and out
rgba_pixel** pixels;                        // Original image pixels
rgba_pixel** processedPixels;               // Warpped image pixels
char *inImage;                              // File name for the input image
char *outImage;								              // File name for the output image

bool smoothFlag = false;
bool sharpFlag = false;
int filterStrength;

bool globalGainFlag;
float globalGainValue;
float redGainValue;
float blueGainValue;
float greenGainValue;

bool globalBiasFlag;
float globalBiasValue;
float redBiasValue;
float blueBiasValue;
float greenBiasValue;

inline int modulo(int a, int b) {
  const int result = a % b;
  return result >= 0 ? result : result + b;
}

/**
 * @brief Read in image
 * @details Read image into program
 *
 * @param image Image name to read in.
 */
void readImage(char *image) {

  // Open image input
  ImageInput *in = ImageInput::open(image);

  // Error handeling
  if (!in) {
    printf("Error reading image: %s\n", geterror().c_str());
    exit(EXIT_FAILURE);
  }

  // Get input spec
  const ImageSpec &spec = in->spec();

  // Get image details
  width = spec.width;
  height = spec.height;
  channels = spec.nchannels;

  // Init the global copy of the pixels
  // Read in the pixels and close the file
  oiioPixels.resize(width*height*channels*sizeof(float));
  in->read_image(TypeDesc::FLOAT, &oiioPixels[0]);
  in->close();
  delete in;

  // Initalize 2d array
  pixels = new rgba_pixel*[height];
  pixels[0] = new rgba_pixel[width*height];

  for (int i=1; i<height; i++) {
    pixels[i] = pixels[i-1] + width;
  }

  // Transfer into custom data structure
  for (int row = 0; row < height; row++)
    for (int col = 0; col < width; col++){
      pixels[row][col].r = oiioPixels[(row*width+col)*channels + 0];
      pixels[row][col].g = oiioPixels[(row*width+col)*channels + 1];
      pixels[row][col].b = oiioPixels[(row*width+col)*channels + 2];
      if(channels == 4) pixels[row][col].a = oiioPixels[(row*width+col)*channels + 3];
      else pixels[row][col].a = 1.0;
    }
}

/**
* @brief Write imgage.
* @details Write compostied image to the file system.
*/
void writeImage() {

  // Transfer to something OpenImageIO understands
  oiioPixels.resize(width*height*4*sizeof(float));

  for (int row = 0; row < height; row++)
    for (int col = 0; col < width; col++){
      oiioPixels[(row*width+col)*4 + 0] = processedPixels[row][col].r;
      oiioPixels[(row*width+col)*4 + 1] = processedPixels[row][col].g;
      oiioPixels[(row*width+col)*4 + 2] = processedPixels[row][col].b;
      oiioPixels[(row*width+col)*4 + 3] = processedPixels[row][col].a;
    }

    // Create output image
    ImageOutput *out = ImageOutput::create(outImage);

    // Error handeling
    if (!out) {
      printf("Error writing image: %s\n", geterror().c_str());
      exit(EXIT_FAILURE);
    }

    // Create output image spec
    ImageSpec spec (width, height, 4, TypeDesc::FLOAT);

    // Open output image file
    out->open(outImage, spec);

    // Write output image to disk and close
    out->write_image(TypeDesc::FLOAT, &oiioPixels[0]);
    out->close();
    delete out;
}

void process() {
  std::array< std::array<float,3> ,3> smSharp = {{
    {{0.0, 1.0, 1.0}},
    {{1.0, 0.0, 1.0}},
    {{0.0, 1.0, 0.0}}
  }};

  float mdSharp[5][5] = {
    {0.0, 0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0, 0.0},
    {1.0, 1.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0, 0.0},
  };

  float lgSharp[7][7] = {
    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
    {1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0},
    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}
  };

  float smSmooth[3][3] = {
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0}
  };

  float mdSmooth[5][5] = {
    {1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0}
  };

  float lgSmooth[7][7] = {
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}
  };

  // Initalize 2d array
  processedPixels = new rgba_pixel*[height];
  processedPixels[0] = new rgba_pixel[width*height];

  for (int i=1; i<height; i++) {
    processedPixels[i] = processedPixels[i-1] + width;
  }


  for (int row = 0; row < height; row++)
    for (int col = 0; col < width; col++){
      rgba_pixel p = pixels[row][col];
      if(globalGainFlag) {
        p = p + globalGainValue;
      } else {
        p.r = p.r + redGainValue;
        p.g = p.g + greenGainValue;
        p.b = p.b + blueGainValue;
      }

      if(globalBiasFlag) {
        p = p * globalBiasValue;
      } else {
        p.r = p.r * redBiasValue;
        p.g = p.g * greenBiasValue;
        p.b = p.b * blueBiasValue;
      }

      processedPixels[row][col] = p;
    }

    int radius;
    int div;

    if(smoothFlag || sharpFlag) {
      if(smoothFlag) {
        switch (filterStrength) {
          case 1:
            radius = 3/2;
            div = 4;
            break;
          case 2:
            radius = 5/2;
            div = 8;
            break;
          case 3:
            radius = 7/2;
            div = 12;
            break;
        }
      } else if(sharpFlag) {
        switch (filterStrength) {
          case 1:
            radius = 3/2;
            div = 9;
            break;
          case 2:
            radius = 5/2;
            div = 25;
            break;
          case 3:
            radius = 7/2;
            div = 79;
            break;
        }
      }
    }

    for (int row = 0; row < height; row++)
      for (int col = 0; col < width; col++){
        float redSum = 0.0;
        float greenSum = 0.0;
        float blueSum = 0.0;
        for(int i = -radius; i <= radius; i++)
          for(int j = -radius; j <= radius; j++){
            if(smoothFlag || sharpFlag) {
              if(smoothFlag) {
                switch (filterStrength) {
                  case 1:
                    redSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].r * smSmooth[i][j];
                    greenSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].g * smSmooth[i][j];
                    blueSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].b * smSmooth[i][j];
                    break;
                  case 2:
                    redSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].r * mdSmooth[i][j];
                    greenSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].g * mdSmooth[i][j];
                    blueSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].b * mdSmooth[i][j];
                    break;
                  case 3:
                    redSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].r * lgSmooth[i][j];
                    greenSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].g * lgSmooth[i][j];
                    blueSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].b * lgSmooth[i][j];
                    break;
                }
              } else if(sharpFlag) {
                switch (filterStrength) {
                  case 1:
                    redSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].r * smSharp[i][j];
                    greenSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].g * smSharp[i][j];
                    blueSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].b * smSharp[i][j];
                    break;
                  case 2:
                    redSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].r * mdSharp[i][j];
                    greenSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].g * mdSharp[i][j];
                    blueSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].b * mdSharp[i][j];
                    break;
                  case 3:
                    redSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].r * lgSharp[i][j];
                    greenSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].g * lgSharp[i][j];
                    blueSum += processedPixels[modulo(row + i,height)][modulo(col + j,width)].b * lgSharp[i][j];
                    break;
                }
              }
            }
          }

          redSum /= div;
          greenSum /= div;
          blueSum /= div;

          rgba_pixel p;
          p.r = redSum;
          p.g = greenSum;
          p.b = blueSum;
          p.a = 1.0;

          processedPixels[row][col] = p;
        }
      }

/**
 * Main program
 * @param  argc Number of command line arguments, inlucing the program itself.
 * @param  argv Vector of command line arguments.
 * @return      EXIT_SUCCESS if program exits normally, EXIT_ERROR otherwise.
 */
int main(int argc, char** argv) {

  int c;

  while(true) {
    static struct option long_options[] = {
      {"globalGain", required_argument, 0, 'a'},
      {"redGain",    required_argument, 0, 'b'},
      {"greenGain",  required_argument, 0, 'c'},
      {"blueGain",   required_argument, 0, 'd'},
      {"globalBias", required_argument, 0, 'e'},
      {"redBias",    required_argument, 0, 'f'},
      {"greenBias",  required_argument, 0, 'g'},
      {"blueBias",   required_argument, 0, 'h'},
      {"smooth",     no_argument,       0, 'i'},
      {"sharp",      no_argument,       0, 'j'},
      {"strength",   required_argument, 0, 'k'},
      {"input",      required_argument, 0, 'l'},
      {"output",     required_argument, 0, 'm'},
      {0,0,0,0}
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "a:b:c:d:e:f:g:h:ijk:l:m:", long_options, &option_index);

    if(c == -1) break;

    switch(c) {
      case 'a':
        globalGainFlag = true;
        break;
      case 'b':
        redGainValue = atoi(optarg);
        break;
      case 'c':
        greenGainValue = atoi(optarg);
        break;
      case 'd':
        blueGainValue = atoi(optarg);
        break;
      case 'e':
        globalBiasFlag = true;
        break;
      case 'f':
        redBiasValue = atoi(optarg);
        break;
      case 'g':
        greenBiasValue = atoi(optarg);
        break;
      case 'h':
        blueBiasValue = atoi(optarg);
        break;
      case 'k':
        filterStrength = atoi(optarg);
        break;
      case 'l':
        inImage = optarg;
        break;
      case 'm':
        outImage = optarg;
        break;
      case 'i':
        smoothFlag = true;
        break;
      case 'j':
        sharpFlag = true;
        break;
    }
  }

  readImage(inImage);

  process();

  writeImage();

  return EXIT_SUCCESS;
}
