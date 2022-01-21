#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"

Image::Image(const char *filename)
{
    if (read(filename))
    {
        printf("file read %s", &filename);
        size = w * h * channels;
    }
    else
    {
        printf("Failed reading file %s", &filename);
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
        success = stbi_write_png(filename, w,h, channels,data, w*channels);
        break;
    
    case JPG:
        success = stbi_write_jpg(filename, w,h, channels,data, 100);
        break;
    
    case BMP:
        success = stbi_write_bmp(filename, w,h, channels,data);
        break;
    
    case TGA:
        success = stbi_write_tga(filename, w,h, channels,data);
        break;

    }
    return success !=0;
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
int main(int argc, char** argv)
{
    Image test("test.jpg");
    test.write("new.png");
    Image copy = test;
    for (int i=0;i < copy.w*copy.channels;i++) {
        copy.data[i] = 255;
    }
    copy.write("copy.png");
    Image blank(100,100,3);
    blank.write("blank.jpg");
    return 0;
}
