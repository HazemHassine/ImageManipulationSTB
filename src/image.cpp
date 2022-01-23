#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"

Image::Image(const char *filename)
{
    if (read(filename))
    {
        printf("file read %s", filename);
        size = w * h * channels;
    }
    else
    {
        printf("Failed reading file %s", filename);
    }
}
Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels)
{
    size = w * h * channels;
    data = new uint8_t[size];
}
Image::Image(const Image &img) : Image(img.w, img.h, img.channels)
{
    memcpy(data, img.data, img.size);
}
Image::~Image()
{
    stbi_image_free(data);
}
bool Image::read(const char *filename)
{
    data = stbi_load(filename, &w, &h, &channels, 0);
    return data != NULL;
}
bool Image::write(const char *filename)
{
    ImageType type = getFileType(filename);
    int success;
    switch (type)
    {
    case PNG:
        success = stbi_write_png(filename, w, h, channels, data, w * channels);
        break;

    case JPG:
        success = stbi_write_jpg(filename, w, h, channels, data, 100);
        break;

    case BMP:
        success = stbi_write_bmp(filename, w, h, channels, data);
        break;

    case TGA:
        success = stbi_write_tga(filename, w, h, channels, data);
        break;
    }
    return success != 0;
}
ImageType Image::getFileType(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    if (ext != nullptr)
    {
        if (strcmp(ext, ".png") == 0)
        {
            return PNG;
        }
        if (strcmp(ext, ".jpg") == 0)
        {
            return JPG;
        }

        if (strcmp(ext, ".bmp") == 0)
        {
            return BMP;
        }

        if (strcmp(ext, ".tga") == 0)
        {
            return TGA;
        }
    }
}

Image &Image::grayscale_avg()
{
    if (channels < 3)
    {
        printf("Image %p has less than 3 channels. I will assume that it's already in grayscale", this);
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
            memset(data+i,gray,3);
        }
    }
    return *this;
}
Image &Image::grayscale_lum()
{
    if (channels < 3)
    {
        printf("Image %p has less than 3 channels. I will assume that it's already in grayscale", this);
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            int gray = (0.2126*data[i] + 0.7152*data[i + 1] + 0.0722*data[i + 2]);
            memset(data+i,gray,3);
        }
    }
    return *this;
}
int main(int argc, char **argv)
{
    Image test("test.jpg");
    Image gray_avg = test;
    Image gray_lum = test;
    gray_avg.grayscale_avg();
    gray_avg.write("grayscaleavg.jpg");
    gray_lum.grayscale_lum();
    gray_lum.write("grayscalelum.jpg");
    return 0;
}
