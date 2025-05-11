# Image-Filter-App
**ImageFilterLab** is a C++-based command-line application designed for applying various filters to images. Developed as a collaborative team project for a university assignment, this repository includes a single program implementing a collection of image filters. The project inherits from an `Image` class provided by our professor, Dr. Mohamed El-Ramly.

## Features
- Apply 15 distinct filters, including grayscale, merge, brightness, edge detection, blur, frame, rotation, invert, infrared, sunlight, flip, black & white, resize, crop and purple.
- Supports user inputs for filter customization (e.g., brightness percentage, merge options, crop dimensions).
- Validates inputs to ensure valid image files and parameters (e.g., PNG, JPEG, BMP formats).
- User-friendly menu-driven interface for loading, filtering, and saving images.
- Handles image merging with a second image and brightness adjustments within specified ranges.

## My Contributions
- Developed the **Grayscale Filter**, converting images to grayscale by averaging RGB channels for a uniform monochrome effect.
- Implemented the **Merge Filter**, enabling blending of two images by resizing to the largest dimensions or using the common area, with pixel value maximization.
- Created the **Brightness Filter**, adjusting image brightness based on a user-specified percentage (-50 to 50) with input validation.
- Built the **Edge Detection Filter**, applying Sobel operators to detect edges after converting images to black-and-white for enhanced contrast.