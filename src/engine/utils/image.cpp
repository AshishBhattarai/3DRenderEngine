#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "utils/slogger.hpp"

// load image
void Image::loadImageData(const std::string& path, bool flip) {
	stbi_set_flip_vertically_on_load(flip);

	// load the image
	data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
	if(!data)
		SLOG("Couldn't load the image", path, stbi_failure_reason());
}

Image::Image(const std::string& path, bool flip) :
	width(0),
	height(0),
	numChannels(0)
{
	loadImageData(path, flip);
}

// get rbg value of pixel
float Image::getRGB(unsigned int x, unsigned int y) {
	if(x >= (unsigned int)height || y >= (unsigned int)height)
		return 0.0f;

	// index for pixel color(red), g = red_index + 1, b = red_index + 2
	int pixel = (x + width * y) * numChannels; //numChannels is offset
	// get rgb values
	unsigned char r = data[pixel];
	unsigned char g = data[pixel + 1];
	unsigned char b = data[pixel + 2];
	// unsigned char a = data[pixel + 3]; -- if numChannes == 4

	return (float)r + (float)g + (float)b; // return total color
}

// free image
Image::~Image() {
	if(data)
		stbi_image_free(data);
}