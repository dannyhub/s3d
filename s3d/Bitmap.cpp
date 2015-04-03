#include "Bitmap.h"

#include <Windows.h>
#include <fstream>

using namespace std;

namespace s3d
{

void BMPBitmap::fromFile(const std::string& filename) {
  fstream fin;
  fin.exceptions(ios_base::failbit | ios_base::badbit | ios_base::eofbit);
  fin.open(filename.c_str(), ios_base::in | ios_base::binary);
  BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
  fin.read((char*)&bitmapfileheader, sizeof(bitmapfileheader));
  const int bitmapID = 0x4D42;
  if (bitmapfileheader.bfType != bitmapID) {
    throw exception("file is not a .BMP");
  }

  BITMAPINFOHEADER bitmapHeader;
  fin.read((char*)&bitmapHeader, sizeof(bitmapHeader));
  if (bitmapHeader.biBitCount != 24) {
    throw exception("bitcount incorrect,  only support 24 bitcount");;
  }

  if (bitmapHeader.biCompression != BI_RGB) {
    throw exception("biCompression != BI_RGB");
  }

  buffer_.resize(sizeof (uint32_t)* bitmapHeader.biWidth * abs(bitmapHeader.biHeight));
  assert(bitmapHeader.biSizeImage >= bitmapHeader.biWidth * 3U);
  if (bitmapHeader.biSizeImage < bitmapHeader.biWidth * 3U || bitmapHeader.biSizeImage <= 0U) {
    throw exception("biSizeImage was less than one line");
  }

  vector<uint8_t> bmpbytes(bitmapHeader.biSizeImage);
  fin.read((char*)&bmpbytes[0], bmpbytes.size());

  width_ = bitmapHeader.biWidth;
  height_ = abs(bitmapHeader.biHeight);

  if (bitmapHeader.biHeight < 0) {
    for (int row = 0; row < height_; ++row) {
      for (int col = 0; col < width_; ++col) {
        auto rgb24Index = (row*width_ + col) * 3;
        uint32_t value = bmpbytes[rgb24Index] | bmpbytes[rgb24Index + 1] << 8 | bmpbytes[rgb24Index + 2] << 16;
        setPixel(row, col, value);
      }
    }
  }
  else if (bitmapHeader.biHeight > 0) {
    for (int row = height_ - 1; row >= 0; --row) {
      for (int col = 0; col < width_; ++col) {
        auto rgb24Index = (row*width_ + col) * 3;
        uint32_t value = bmpbytes[rgb24Index] | bmpbytes[rgb24Index + 1] << 8 | bmpbytes[rgb24Index + 2] << 16;
        setPixel(col, height_ - row - 1, value);
      }
    }
  }
  else{
    throw exception("biHeight is zero");
  }
}


}