#include "ImageHandler.h"

ImageHandler ImageHandler::Screenshot() {
  CGImageRef screenShot = CGWindowListCreateImage(CGRectInfinite, kCGWindowListOptionOnScreenOnly, kCGNullWindowID, kCGWindowImageDefault);
  ImageHandler a = ImageHandler(screenShot);
  CGImageRelease(screenShot);
  return a;
}

ImageHandler ImageHandler::LoadPNG(std::string pngFilePath) {
  CGDataProviderRef dataProvider = CGDataProviderCreateWithFilename(pngFilePath.c_str());
  CGImageRef image = CGImageCreateWithPNGDataProvider(dataProvider, nullptr, false, kCGRenderingIntentDefault);
  ImageHandler a = ImageHandler(image);
  CGImageRelease(image);
  CGDataProviderRelease(dataProvider);
  return a;
}

ImageHandler::ImageHandler() {
  pixels = nullptr;
  width = 0;
  height = 0;
}

ImageHandler::ImageHandler(uint8_t* argb, int imagewidth, int imageheight) {
  pixels = argb;
  width = imagewidth;
  height = imageheight;
}

ImageHandler::ImageHandler(const ImageHandler& other) {
  height = other.height;
  width = other.width;
  void* block = malloc(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
  uint8_t* newPixels = static_cast<uint8_t*>(block);
  uint8_t* writer = newPixels;
  uint8_t* reader = other.pixels;
  for (long row = 0; row < height; ++row) {
    for (long col = 0; col < width; ++col) {
      *writer++ = *reader++; //copy alpha
      *writer++ = *reader++; //copy red
      *writer++ = *reader++; //copy green
      *writer++ = *reader++; //copy blue
    }
  }
  pixels = newPixels;
}

ImageHandler& ImageHandler::operator=(const ImageHandler& other) {
  free(pixels);
  height = other.height;
  width = other.width;
  void* block = malloc(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
  uint8_t* newPixels = static_cast<uint8_t*>(block);
  uint8_t* writer = newPixels;
  uint8_t* reader = other.pixels;
  for (long row = 0; row < height; ++row) {
    for (long col = 0; col < width; ++col) {
      *writer++ = *reader++; //copy alpha
      *writer++ = *reader++; //copy red
      *writer++ = *reader++; //copy green
      *writer++ = *reader++; //copy blue
    }
  }
  pixels = newPixels;
  return *this;
}

ImageHandler& ImageHandler::operator=(ImageHandler&& other) {
  free(pixels);
  height = other.height;
  width = other.width;
  pixels = other.pixels;
  other.pixels = nullptr;
  return *this;
}

ImageHandler::ImageHandler(ImageHandler&& other) {
  height = other.height;
  width = other.width;
  pixels = other.pixels;
  other.pixels = nullptr;
}

int ImageHandler::Height() {
  return height;
}

int ImageHandler::Width() {
  return width;
}

std::tuple<int, int, int> ImageHandler::RGBAtPixel(int pixelx, int pixely) {
  long startpixel = static_cast<long>(pixely) * static_cast<long>(width) + pixelx;
  uint8_t* oldpixel = pixels + startpixel*4;
  int a = *oldpixel++; //copy alpha
  int r = *oldpixel++; //copy red
  int g = *oldpixel++; //copy green
  int b = *oldpixel++; //copy blue
  return std::make_tuple(r,g,b);
}

std::vector<std::pair<int, int>> ImageHandler::PixelsWithRGB(int red, int green, int blue) {
  uint8_t* reader = pixels;
  std::vector<std::pair<int, int>> matches;
  for (long row = 0; row < height; ++row) {
    for (long col = 0; col < width; ++col) {
      int a = *reader++;
      int r = *reader++;
      int g = *reader++;
      int b = *reader++;
      if (r == red && g == green && b == blue) {
	matches.push_back(std::make_pair(col, row));
      }
    }
  }
  return matches;
}

ImageHandler ImageHandler::Scale(int newWidth, int newHeight) {
  void* block = malloc(static_cast<size_t>(newWidth) * static_cast<size_t>(newHeight) * 4);
  uint8_t* newPixels = static_cast<uint8_t*>(block);
  uint8_t* writer = newPixels;
  for (long row = 0; row < newHeight; ++row) {
    for (long col = 0; col < newWidth; ++col) {
      long oldpixelx = static_cast<double>(col) / static_cast<double>(newWidth) * width;
      long oldpixely = static_cast<double>(row) / static_cast<double>(newHeight) * height;
      uint8_t* oldpixel = pixels + (oldpixely*width + oldpixelx)*4;
      *writer++ = *oldpixel++; //copy alpha
      *writer++ = *oldpixel++; //copy red
      *writer++ = *oldpixel++; //copy green
      *writer++ = *oldpixel++; //copy blue
    }
  }
  return ImageHandler(newPixels, newWidth, newHeight);
}

ImageHandler ImageHandler::Crop(int topleftx, int toplefty, int cropwidth, int cropheight) {
  void* block = malloc(static_cast<size_t>(cropwidth) * static_cast<size_t>(cropheight) * 4);
  uint8_t* newPixels = static_cast<uint8_t*>(block);
  uint8_t* writer = newPixels;
  long startpixel = static_cast<long>(toplefty) * static_cast<long>(width) + topleftx;
  for (long row = 0; row < cropheight; ++row) {
    for (long col = 0; col < cropwidth; ++col) {
      uint8_t* oldpixel = pixels + (row*width + col + startpixel)*4;
      *writer++ = *oldpixel++; //copy alpha
      *writer++ = *oldpixel++; //copy red
      *writer++ = *oldpixel++; //copy green
      *writer++ = *oldpixel++; //copy blue
    }
  }
  return ImageHandler(newPixels, cropwidth, cropheight);
}

ImageHandler ImageHandler::ConvertToBlackAndWhite() {
  void* block = malloc(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
  uint8_t* newPixels = static_cast<uint8_t*>(block);
  uint8_t* writer = newPixels;
  uint8_t* reader = pixels;
  for (long row = 0; row < height; ++row) {
    for (long col = 0; col < width; ++col) {
      int a = *reader++; //copy alpha
      int r = *reader++; //copy red
      int g = *reader++; //copy green
      int b = *reader++; //copy blue
      double luminosity = (0.2126*r +0.7152*g + 0.0722*b)/255;
      if (luminosity < 0.5) {
	//Black pixel
	*writer++ = 255;
	*writer++ = 0;
	*writer++ = 0;
	*writer++ = 0;
      }
      else {
	//White pixel
	*writer++ = 255;
	*writer++ = 255;
	*writer++ = 255;
	*writer++ = 255;
      }
    }
  }
  return ImageHandler(newPixels, width, height);
}

ImageHandler ImageHandler::FilterRGB(int red, int green, int blue) {
  void* block = malloc(static_cast<size_t>(width) * static_cast<size_t>(height) * 4);
  uint8_t* newPixels = static_cast<uint8_t*>(block);
  uint8_t* writer = newPixels;
  uint8_t* reader = pixels;
  for (long row = 0; row < height; ++row) {
    for (long col = 0; col < width; ++col) {
      int a = *reader++; //copy alpha
      int r = *reader++; //copy red
      int g = *reader++; //copy green
      int b = *reader++; //copy blue
      if (red == r && blue == b && green == g) {
	//Black pixel
	*writer++ = 255;
	*writer++ = 0;
	*writer++ = 0;
	*writer++ = 0;
      }
      else {
	//White pixel
	*writer++ = 255;
	*writer++ = 255;
	*writer++ = 255;
	*writer++ = 255;
      }
    }
  }
  return ImageHandler(newPixels, width, height);
}

double ImageHandler::Similarity(ImageHandler& other) {
  double same = 0;
  ImageHandler copy = other.Scale(width, height);
  uint8_t* reader1 = pixels;
  uint8_t* reader2 = copy.pixels;
  for (long row = 0; row < height; ++row) {
    for (long col = 0; col < width; ++col) {
      bool alpha = (*reader1++ == *reader2++);
      bool red = (*reader1++ == *reader2++);
      bool green = (*reader1++ == *reader2++);
      bool blue = (*reader1++ == *reader2++);
      if (alpha && red && green && blue) {
  	same++;
      }
    }
  }
  double numPixels = width * height;
  return same/numPixels;
}

std::tuple<int, int, int> ImageHandler::MostCommonRGB() {
  std::map<std::tuple<int, int, int>, int> count;
  uint8_t* reader = pixels;
  for (long row = 0; row < height; ++row) {
    for (long col = 0; col < width; ++col) {
      int alpha = *reader++;
      int red = *reader++;
      int green = *reader++;
      int blue = *reader++;
      std::tuple<int, int, int> pix = std::make_tuple(red, green, blue);
      if (count.find(pix) == count.end()) {
	count[pix] = 0;
      }
      else {
	count[pix] += 1;
      }
    }
  }
  int max = 0;
  std::tuple<int, int, int> rgb;
  for (auto kv : count) {
    if (kv.second > max) {
      max = kv.second;
      rgb = kv.first;
    }
  }
  return rgb;
}

void ImageHandler::SaveAsPPM(std::string outfile) {
  uint8_t* argb = pixels;
  std::ofstream dest(outfile);
  dest << "P3" << std::endl;
  dest << width << " " << height << std::endl;
  dest << "255" << std::endl;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      int a = static_cast<int>(*argb++);
      int r = static_cast<int>(*argb++);
      int g = static_cast<int>(*argb++);
      int b = static_cast<int>(*argb++);
      if (col != 0) {
	dest << " ";
      }
      dest << r << " " << g << " " << b;
    }
    dest << std::endl;
  }
}

ImageHandler::~ImageHandler() {
  free(pixels);
}

ImageHandler::ImageHandler(CGImageRef image) {
  CGContextRef cgctx = CreateARGBBitmapContext(image);
  width = CGImageGetWidth(image);
  height = CGImageGetHeight(image); 
  CGRect rect = {{0,0},{static_cast<CGFloat>(width), static_cast<CGFloat>(height)}}; 
  CGContextDrawImage(cgctx, rect, image);
  
  void* data = CGBitmapContextGetData (cgctx);
  pixels = static_cast<uint8_t*>(data);
  CGContextRelease(cgctx);
}

CGContextRef ImageHandler::CreateARGBBitmapContext(CGImageRef inImage) {
  CGContextRef    context = NULL;
  CGColorSpaceRef colorSpace;
  void *          bitmapData;
  size_t             bitmapByteCount;
  size_t             bitmapBytesPerRow;

  // Get image width, height. We'll use the entire image.
  size_t pixelsWide = CGImageGetWidth(inImage);
  size_t pixelsHigh = CGImageGetHeight(inImage);

  // Declare the number of bytes per row. Each pixel in the bitmap in this
  // example is represented by 4 bytes; 8 bits each of red, green, blue, and
  // alpha.
  bitmapBytesPerRow   = (pixelsWide * 4);
  bitmapByteCount     = (bitmapBytesPerRow * pixelsHigh);

  // Use the generic RGB color space.
  colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
  if (colorSpace == NULL)
    {
      fprintf(stderr, "Error allocating color space\n");
      return NULL;
    }

  // Allocate memory for image data. This is the destination in memory
  // where any drawing to the bitmap context will be rendered.
  bitmapData = malloc( bitmapByteCount );
  if (bitmapData == NULL)
    {
      fprintf (stderr, "Memory not allocated!");
      CGColorSpaceRelease( colorSpace );
      return NULL;
    }

  // Create the bitmap context. We want pre-multiplied ARGB, 8-bits
  // per component. Regardless of what the source image format is
  // (CMYK, Grayscale, and so on) it will be converted over to the format
  // specified here by CGBitmapContextCreate.
  context = CGBitmapContextCreate (bitmapData,
				   pixelsWide,
				   pixelsHigh,
				   8,      // bits per component
				   bitmapBytesPerRow,
				   colorSpace,
				   kCGImageAlphaPremultipliedFirst);
  if (context == NULL)
    {
      free (bitmapData);
      fprintf (stderr, "Context not created!");
    }

  // Make sure and release colorspace before returning
  CGColorSpaceRelease( colorSpace );

  return context;
}
