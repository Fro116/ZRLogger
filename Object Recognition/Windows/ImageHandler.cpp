#include "ImageHandler.h"
#include "lodepng.h"

ImageHandler ImageHandler::Screenshot() {
  int x1, y1, x2, y2, w, h;

  // get screen dimensions
  x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
  y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
  x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
  y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
  w = x2 - x1;
  h = y2 - y1;

  // copy screen to bitmap
  HDC     hScreen = GetDC(NULL);
  HDC     hDC = CreateCompatibleDC(hScreen);
  HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
  HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
  BOOL    bRet = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY);

  // copy bitmap pixels
  BITMAPINFO MyBMInfo = { 0 };
  MyBMInfo.bmiHeader.biSize = sizeof(MyBMInfo.bmiHeader);
  GetDIBits(hDC, hBitmap, 0, 0, NULL, &MyBMInfo, DIB_RGB_COLORS);
  void* newBlock = malloc(MyBMInfo.bmiHeader.biSizeImage);
  uint8_t* newPixels = static_cast<uint8_t*>(newBlock);
  MyBMInfo.bmiHeader.biBitCount = 32;
  MyBMInfo.bmiHeader.biCompression = BI_RGB;
  GetDIBits(hDC, hBitmap, 0, MyBMInfo.bmiHeader.biHeight, (LPVOID)newPixels, &MyBMInfo, DIB_RGB_COLORS);
  void* block = malloc(MyBMInfo.bmiHeader.biSizeImage);
  uint8_t* pixels = static_cast<uint8_t*>(block);
  uint8_t* writer = pixels;
  for (long row = 0; row < h; ++row) {
    uint8_t* reader = newPixels + (h-1-row)*w*4;
    for (long col = 0; col < w; ++col) {
      int b = *reader++;
      int g = *reader++;
      int r = *reader++;
      int a = *reader++;
      *writer++ = a;
      *writer++ = r;
      *writer++ = g;
      *writer++ = b;
    }
  }

  // clean up
  SelectObject(hDC, old_obj);
  DeleteDC(hDC);
  ReleaseDC(NULL, hScreen);
  DeleteObject(hBitmap);
  free(newPixels);
  return ImageHandler(pixels, w, h);
}

ImageHandler ImageHandler::LoadPNG(std::string pngFilePath) {
  unsigned char* image;
  unsigned width, height;
  unsigned error;
  error = lodepng_decode32_file(&image, &width, &height, pngFilePath.c_str());
  if (error) printf("error %u: %s\n", error, lodepng_error_text(error));

  void* newBlock = malloc(width * height * 4);
  uint8_t* newPixels = static_cast<uint8_t*>(newBlock);
  uint8_t* reader = static_cast<uint8_t*>(image);
  uint8_t* writer = newPixels;
  for (long row = 0; row < height; ++row) {
    for (long col = 0; col < width; ++col) {
      int r = *reader++;
      int g = *reader++;
      int b = *reader++;
      int a = *reader++;
      *writer++ = a;
      *writer++ = r;
      *writer++ = g;
      *writer++ = b;
    }
  }
  free(image);
  return ImageHandler(newPixels, width, height);
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
  other.height = 0;
  other.width = 0;  
  return *this;
}

ImageHandler::ImageHandler(ImageHandler&& other) {
  height = other.height;
  width = other.width;
  pixels = other.pixels;
  other.pixels = nullptr;
  other.height = 0;
  other.width = 0;  
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

ImageHandler ImageHandler::ConvertToBlackAndWhite(int whiter, int whiteg, int whiteb) {
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
      bool black = (r < whiter || g < whiteg || b < whiteb);
      if (black) {
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


