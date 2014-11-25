/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 1
 *
 * Generic RGBA pixel class. Copied from
 * http://people.cs.clemson.edu/~levinej/courses/6040/code/flatview.zip
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iomanip>

/**
 * Generic RGBA pixel class.
 */
class rgba_pixel {
   public:
      float r;
      float g;
      float b;
      float a;

      /**
       * @brief Array operator.
       * @details Access the different compenents via array accessors.
       *
       * @param int Array index.
       * @return Pointer to index.
       */
      float& operator[] (const unsigned int index);

      // Added by Joshua Hull (jhull@clemson.edu)
      /**
       * @brief Ouput pixel values.
       * @details Output pixel values in the form (R,G,B,A), limited to 4
       *          decimal places.
       *
       * @param os Ouput stream we are give.
       * @return The output stream we have written to.
       */
      std::ostream& operator<<(std::ostream& os);
};

// Added by Joshua Hull (jhull@clemson.edu)
/**
 * @brief Scale pixel by a float.
 * @details Scales the RGB values of a pixel (not alpha though) by a scalar.
 *
 * @param p Pixel we are scaling.
 * @param scalar Float we are scaling the pixel by.
 * @return Pixel pointer to a new, scaled pixel.
 */
rgba_pixel& operator* (rgba_pixel p, const float scalar);

/**
 * @brief Scale pixel by a float.
 * @details Scales the RGB values of a pixel (not alpha though) by a scalar.
 *
 * @param p Pixel we are scaling.
 * @param scalar Float we are scaling the pixel by.
 * @return Pixel pointer to a new, scaled pixel.
 */
rgba_pixel& operator* (const float scalar, rgba_pixel p);

rgba_pixel& operator+ (rgba_pixel p, const float scalar);

rgba_pixel& operator+ (const float scalar, rgba_pixel p);
