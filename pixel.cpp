/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 1
 *
 * Generic RGBA pixel class. Copied from
 * http://people.cs.clemson.edu/~levinej/courses/6040/code/flatview.zip
 *
 * See pixel.h for comprehensive documentation.
 */

#include "pixel.h"

float& rgba_pixel::operator[] (const unsigned int index) {
   if (index == 0) {
      return r;
   } else if (index == 1) {
      return g;
   } else if (index == 2) {
      return b;
   } else if (index == 3) {
      return a;
   } else {
      std::cerr << "invalid index" << std::endl;
      exit(-2);
   }
}

/**
 * Added by Joshua Hull (jhull@clemson.edu)
 */
rgba_pixel& operator* (const rgba_pixel p, const float scalar){
  rgba_pixel *retP = new rgba_pixel();

  retP->r = std::max(std::min(p.r * scalar, 1.0f), 0.0f);
  retP->g = std::max(std::min(p.g * scalar, 1.0f), 0.0f);
  retP->b = std::max(std::min(p.b * scalar, 1.0f), 0.0f);
  retP->a = p.a;

  return *retP;
}

rgba_pixel& operator* (const float scalar, rgba_pixel p){
  return p * scalar;
}

std::ostream& operator<<(std::ostream& os, const rgba_pixel& p){
  os << std::fixed;
  os << std::setprecision(4);
  os << "(" << p.r << "," << p.g << "," << p.b << "," << p.a << ")";
  return os;
}

rgba_pixel& operator+ (rgba_pixel p, const float scalar){
  rgba_pixel *retP = new rgba_pixel();

  retP->r = std::max(std::min(p.r + scalar, 1.0f), 0.0f);
  retP->g = std::max(std::min(p.g + scalar, 1.0f), 0.0f);
  retP->b = std::max(std::min(p.b + scalar, 1.0f), 0.0f);
  retP->a = p.a;

  return *retP;
}

rgba_pixel& operator+ (const float scalar, rgba_pixel p){
  return p + scalar;
}
