#ifndef ImageHandler_1468280724853253
#define ImageHandler_1468280724853253

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>
#include <chrono>
#include <set>
#include <cstdlib>
#include <tuple>
#include <map>
#include <ApplicationServices/ApplicationServices.h>

class ImageHandler {
 public:
  static ImageHandler Screenshot();
  static ImageHandler LoadPNG(std::string pngFilePath);
  ImageHandler(const ImageHandler& other);
  ImageHandler& operator=(const ImageHandler& other);
  ~ImageHandler();

  int Height();
  int Width();
  ImageHandler Crop(int topleftx, int toplefty, int cropwidth, int cropheight);
  ImageHandler Scale(int newWidth, int newHeight);
  ImageHandler ConvertToBlackAndWhite();
  ImageHandler FilterRGB(int red, int green, int blue); //selected color is turned to black, all other colors will be turned to white
  void SaveAsPPM(std::string outfile);
  std::tuple<int, int, int> RGBAtPixel(int pixelx, int pixely); //Zero indexed
  std::vector<std::pair<int, int>> PixelsWithRGB(int red, int green, int blue); //returns all pixel coordinates (x,y) matching color
  double Similarity(ImageHandler& other); //Scales other and returns the fraction of pixels that have the same RGBA values
  std::tuple<int, int, int> MostCommonRGB(); //Returns the color that appears most often in the image. Note: slow
 private:
  ImageHandler() = delete;
  ImageHandler(CGImageRef image);
  ImageHandler(uint8_t* argb, int imagewidth, int imageheight);
  CGContextRef CreateARGBBitmapContext(CGImageRef inImage);
    
  int height;
  int width;
  uint8_t* pixels; //Note: data is stored as ARGB with one byte for each channel (4 bytes per pixel)
};

#endif
