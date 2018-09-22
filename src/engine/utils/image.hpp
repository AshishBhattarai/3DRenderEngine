#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>

// this class represents raw image data

class Image {
private:
	// image attributes
	int width;
	int height;
	int numChannels; //no.of color channels
	// image bytes data
	u_char* data;

	void loadImageData(const std::string& path, bool flip = false);

public:
	Image(const std::string& path, bool flip);
	~Image();

	// get r+g+b value of x,y pixel
	float getRGB(u_int x, u_int y);

	// getters
	const u_char* getData() const {
		return data;
	}

	int getWidth() const {
		return width;
	}

	int getHeight() const {
		return height;
	}

	int getNumChannels() const {
		return numChannels;
	}
};

#endif