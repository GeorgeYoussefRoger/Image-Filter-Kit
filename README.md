# PixelForge

**PixelForge** is an interactive, menu-driven C++ application for applying a wide variety of image processing filters. This project supports loading, editing and saving images in multiple formats. Demonstrating core image processing techniques through a console-based interface.

## Features
PixelForge offers a rich set of 18 image filters and transformations:
- **Basic Adjustments**: Grayscale, Black & White, Invert, Brightness, Resize, Crop
- **Color Effects**: Sunlight, Purple, Infrared
- **Transformations**: Flip, Rotate, Skew
- **Enhancements**: Blur, Oil Painting, TV Effect
- **Editing**: Frame (Simple/Decorated), Merge Images, Edge Detection

## Technologies & Concepts
- **Language**: C++
- **Image Processing**: Direct pixel manipulation using RGB channels
- **Third-party Libraries**: 
  - `stb_image.h` and `stb_image_write.h` for image I/O
- **Image Formats Supported**: `.jpg`, `.jpeg`, `.png`, `.bmp`

## Prerequisites
- A C++17 (or later) compatible compiler
- Ensure that `Image_Class.h` , `stb_image.h` and `stb_image_write.h` are present in the same directory or properly linked.

## How to Use
1. **Load Image**  
   Accepts valid filenames with supported extensions.
2. **Apply Filters**  
   Choose from the menu to apply one or more transformations sequentially.
3. **Save Image**  
   Save your result with a specified filename and extension.

## Acknowledgment
This project was built using a custom image handling library provided by:
> **Dr. Mohamed El Ramly**  
> Faculty of Computers and Artificial Intelligence  
> Cairo University
