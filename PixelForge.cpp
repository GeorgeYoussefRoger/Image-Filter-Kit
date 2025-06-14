#include "Image_Class.h"
using namespace std;

Image* ptr = nullptr; // Global pointer to track the currently loaded image

void main_menu(); // Forward declaration of main menu function

// Convert image to grayscale
void gray(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < image.channels; ++k) {
                avg += image(i, j, k); // Sum all channel values
            }
            avg /= image.channels; // Calculate average
            for (int k = 0; k < image.channels; ++k) {
                image.setPixel(i, j, k, avg);
            }
        }
    }
}

// Convert image to black and white
void black_white(Image& image) {
    gray(image); // First convert to grayscale
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < image.channels; ++k) {
                unsigned int avg = image(i, j, 0);

                // Above 127 becomes white, below becomes black
                if (avg > 127) {
                    image.setPixel(i, j, k, 255);
                } 
                else {
                    image.setPixel(i, j, k, 0);
                }
            }
        }
    }
}

// Invert image colors
void invert(Image& image) {
    for (int i = 0; i < image.width; ++i) {
       for (int j = 0; j < image.height; ++j) {
          for (int k = 0; k < image.channels; ++k) {
            image.setPixel(i, j, k, 255 - image(i, j, k)); // Subtract each channel value from 255
          }
       }
    }
}

// Merge two images
void merge(Image& image) {
    string img2;
    Image image2;
    while (true) {
        cout << "Enter the name of the second image with extension (.png or .jpeg or .jpg or .bmp): ";
        cin >> img2;
        try {
            image2.loadNewImage(img2);
            break;
        } 
        catch (const invalid_argument& e) {}
    }

    // Case 1: Images are same size - simple merge
    if (image.width == image2.width && image.height == image2.height) { 
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    unsigned int pixel1 = image.getPixel(i, j, k);
                    unsigned int pixel2 = image2.getPixel(i, j, k);
                    image.setPixel(i, j, k, max(pixel1, pixel2)); // Take maximum value of each channel
                }
            }
        }
    }

    // Case 2: Images are different sizes - provide options
    else {
        cout << "The images are of different sizes. What to do?" << endl;
        cout << "1. Resize to the biggest width and height" << endl;
        cout << "2. Merge the common area of the smaller width and height\n=>";
        char choice;
        while (!(cin >> choice && (choice == '1' || choice == '2'))) {
            cout << "Invalid choice. Enter 1 or 2: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Option 1: Resize both to max dimensions
        if (choice == '1') {
            unsigned int max_width = max(image.width, image2.width);
            unsigned int max_height = max(image.height, image2.height);
            Image merged(max_width, max_height);

            // Calculate scaling factors
            float scale_row1 = (float)image.width / max_width;
            float scale_column1 = (float)image.height / max_height;
            float scale_row2 = (float)image2.width / max_width;
            float scale_column2 = (float)image2.height / max_height;

            // Scale and merge
            for (int i = 0; i < max_width; ++i) {
                for (int j = 0; j < max_height; ++j) {
                    for (int k = 0; k < image.channels; ++k) {
                        unsigned char pixel1 = image.getPixel(round(i * scale_row1), round(j * scale_column1), k);
                        unsigned char pixel2 = image2.getPixel(round(i * scale_row2), round(j * scale_column2), k);
                        merged.setPixel(i, j, k, max(pixel1, pixel2));
                    }
                }
            }
            image = merged; 
        }

        // Option 2: Crop to common area
        else if (choice == '2') {
            int common_width = min(image.width, image2.width);
            int common_height = min(image.height, image2.height);
            Image merged(common_width, common_height);

            // Merge the common area
            for (int i = 0; i < common_width; ++i) {
                for (int j = 0; j < common_height; ++j) {
                    for (int k = 0; k < image.channels; ++k) {
                        unsigned char pixel1 = image.getPixel(i, j, k);
                        unsigned char pixel2 = image2.getPixel(i, j, k);
                        merged.setPixel(i, j, k, max(pixel1, pixel2));
                    }
                }
            }
            image = merged;
        }
    }
}

// Flip image horizontally or vertically
void flip(Image& image) {
    cout << "Choose flip direction:" << endl;
    cout << "1. Flip Horizontally" << endl;
    cout << "2. Flip Vertically\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2'))) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Horizontal flip
    if (choice == '1') {
        for (int i = 0; i < image.width / 2; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    swap(image(i, j, k), image(image.width - 1 - i, j, k)); // Swap pixels from left and right
                }
            }
        }
    }

    // Vertical flip 
    else if (choice == '2') {
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height / 2; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    swap(image(i, j, k), image(i, image.height - 1 - j, k)); // Swap pixels from top and bottom
                }
            }
        }
    }
}

// Rotate image by 90, 180, or 270 degrees
void rotate(Image& image) {
    cout << "Choose rotation degree:" << endl;
    cout << "1. 90 degree" << endl;
    cout << "2. 180 degree" << endl;
    cout << "3. 270 degree\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2' || choice == '3'))) {
        cout << "Invalid choice. Enter 1, 2, or 3: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // 90 degree rotation
    if (choice == '1') {
        Image rotated(image.height, image.width);
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    rotated(image.height - 1 - j, i, k) = image(i, j, k);
                }
            }
        }
        image = rotated;
    }

    // 180 degree rotation
    else if (choice == '2') {
        Image rotated(image.width, image.height);
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    rotated(i, j, k) = image(image.width - 1 - i, image.height - 1 - j, k);

                }
            }
        }
        image = rotated;
    }

    // 270 degree rotation
    else if (choice == '3') {
        Image rotated(image.height, image.width);
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    rotated(j, image.width - 1 - i, k) = image(i, j, k);
                }
            }
        }
        image = rotated;
    }
}

// Adjust image brightness by percentage
void brightness(Image& image) {
    cout << "Enter the percentage to adjust brightness (-50 to 50): ";
    float percentage;
    while (!(cin >> percentage && percentage >= -50 && percentage <= 50)) {
        cout << "Invalid input. Enter a number between -50 and 50: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    float factor = 1 + (percentage / 100); // Calculate brightness factor
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < image.channels; ++k) {
                // Apply factor and clamp to 0-255 range
                float new_value = round(image(i, j, k) * factor);
                image(i, j, k) = new_value < 0 ? 0 : new_value > 255 ? 255 : new_value;
            }
        }
    }
}

// Crop a rectangular region from the image
void crop(Image& image) {
    cout << "Image width = " << image.width << endl;
    cout << "Image height = " << image.height << endl;

    // Get top-left corner coordinates
    unsigned int x, y;
    cout << "Enter top-left corner (x, y) of crop area: ";
    while (!(cin >> x >> y && x < image.width && y < image.height)) {
        cout << "Invalid input. Enter valid image bounds: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Get width and height of crop area
    unsigned int w, h;
    cout << "Enter crop width and height: ";
    while (!(cin >> w >> h && x + w <= image.width && y + h <= image.height)) {
        cout << "Invalid input. Enter valid width and height: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Image cropped(w, h);
    for(int i = 0; i < w; ++i) {
        for(int j = 0; j < h; ++j) {
            for(int k = 0; k < image.channels; ++k) {
                cropped(i, j, k) = image(x + i, y + j, k);
            }
        }
    }
    image = cropped;
}

// Add a frame around the image
void frame(Image& image) {
    unsigned int frame_size;
    cout << "Enter frame thickness (10 to 100): ";
    while (!(cin >> frame_size && frame_size >= 10 && frame_size <= 100)) {
        cout << "Invalid input. Enter a number between 10 and 100: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Choose frame style:" << endl;
    cout << "1. Simple frame" << endl;
    cout << "2. Decorated frame\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2'))) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Calculate new dimensions with frame
    unsigned int new_width = image.width + 2 * frame_size;
    unsigned int new_height = image.height + 2 * frame_size;
    Image framed(new_width, new_height);

    // Simple solid color frame
    if (choice == '1') {
        unsigned int r, g, b;
        cout << "Enter the frame colors (red, green, blue) from (0 to 255) for each: ";
        while (!(cin >> r >> g >> b && r <= 255 && g <= 255 && b <= 255)) {
            cout << "Invalid input. Enter three numbers between (0 to 255): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Fill entire new image with frame color
        for (int i = 0; i < new_width; ++i) {
            for (int j = 0; j < new_height; ++j) {
                framed(i, j, 0) = r;
                framed(i, j, 1) = g;
                framed(i, j, 2) = b;
            }
        }
    }

    // Decorated frame with inner border
    else if (choice == '2') {
        // Outer frame color (gold)
        for (int i = 0; i < new_width; ++i) {
            for (int j = 0; j < new_height; ++j) {
                framed(i, j, 0) = 212;
                framed(i, j, 1) = 175;
                framed(i, j, 2) = 55;
            }
        }

        // Inner border (red)
        unsigned int inner_border = frame_size / 3;
        for (int i = inner_border; i < new_width - inner_border; ++i) {
            for (int j = inner_border; j < new_height - inner_border; ++j) {
                framed(i, j, 0) = 160;
                framed(i, j, 1) = 0;
                framed(i, j, 2) = 0;
            }
        }
    }

    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < image.channels; ++k) {
                framed(i + frame_size, j + frame_size, k) = image(i, j, k);
            }
        }
    }
    image = framed;
}

// Detect edges
void edges(Image& image) { 
    gray(image); // First convert to grayscale
    Image detected(image.width, image.height);

    // Sobel operator kernels
    int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Apply Sobel operator to detect edges
    for (int i = 1; i < image.width - 1; ++i) {
        for (int j = 1; j < image.height - 1; ++j) {
            long Gx = 0, Gy = 0;
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    Gx += sobel_x[x + 1][y + 1] * image(i + x, j + y, 0);
                    Gy += sobel_y[x + 1][y + 1] * image(i + x, j + y, 0);
                }
            }

            // Calculate gradient magnitude and threshold
            unsigned char value = (unsigned char)min(255.0, sqrt(Gx * Gx + Gy * Gy));
            for (int k = 0; k < image.channels; ++k) {
                detected.setPixel(i, j, k, value > 127 ? 0 : 255);
            }
        }
    }
    image = detected;
}

// Resize image by dimensions or ratio
void resize(Image& image) {
    cout << "Choose resize method:" << endl;
    cout << "1. By dimensions" << endl;
    cout << "2. By ratio\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2'))) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    float w, h;

    // Resize by absolute dimensions
    if (choice == '1') {
        cout << "Enter width then height: ";
        while (!(cin >> w >> h)) {
            cout << "Invalid input. Enter valid numbers: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Resize by ratio
    else if (choice == '2') {
        float ratio_width, ratio_height;
        cout << "Enter width ratio then height ratio: ";
        while (!(cin >> ratio_width >> ratio_height)) {
            cout << "Invalid input. Please enter valid numbers: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        w = round(image.width * ratio_width);
        h = round(image.height * ratio_height);
    }

    // Calculate scaling factors
    float scale_row = image.width / w;
    float scale_column = image.height / h;

    Image resized(w, h);
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            for (int k = 0; k < image.channels; ++k) {
                resized(i, j, k) = image(round(i * scale_row), round(j * scale_column), k);
            }
        }
    }
    image = resized;
}

// Apply blur effect
void blur(Image& image) {
    unsigned int kernelSize;
    cout << "Enter blur intensity (a positive odd number): ";
    while (!(cin >> kernelSize) || kernelSize % 2 == 0 || kernelSize < 1) {
        cout << "Invalid input. Enter a positive odd number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Image blurred = image;
    int kernelRadius = kernelSize / 2;

    // Horizontal blur pass
    for (int j = 0; j < image.height; ++j) {
        for (int i = 0; i < image.width; ++i) {
            for (int k = 0; k < image.channels; ++k) {
                unsigned int sum = 0;
                int count = 0;
                for (int m = -kernelRadius; m <= kernelRadius; ++m) {
                    if (i + m >= 0 && i + m < image.width) {
                        sum += image(i + m, j, k);
                        count++;
                    }
                }
                blurred(i, j, k) = sum / count;
            }
        }
    }

    // Vertical blur pass
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < image.channels; ++k) {
                unsigned int sum = 0;
                int count = 0;
                for (int n = -kernelRadius; n <= kernelRadius; ++n) {
                    if (j + n >= 0 && j + n < image.height) {
                        sum += blurred(i, j + n, k);
                        count++;
                    }
                }
                image(i, j, k) = sum / count;
            }
        }
    }
}

// Apply sunlight effect
void sunlight(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image(i, j, 2) *= 0.5; // Reduce blue channel by half
        } 
    }
}

// Apply purple effect
void purple(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image(i, j, 1) *= 0.5; // Reduce green channel by half
        }
    }
}

// Apply infrared effect
void infrared(Image& image) {
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            // Set red to max, invert green and blue
            image.setPixel(i, j, 0, 255);
            image.setPixel(i, j, 1, 255 - image(i, j, 1));
            image.setPixel(i, j, 2, 255 - image(i, j, 2));
        }
    }
}

// Apply oil painting effect
void oil(Image& image) {
    int radius = 2;
    int intensitylevels = 8;
    Image oiled(image.width, image.height);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            int Hist[intensitylevels] = {0}; // Histogram to count pixels at each intensity level

            // Arrays to accumulate color sums for each intensity level
            int rSum[intensitylevels] = {0};
            int gSum[intensitylevels] = {0};
            int bSum[intensitylevels] = {0};

            // Calculate neighborhood boundaries
            int left = max(0, i - radius);
            int right = min(image.width - 1, i + radius);
            int top = max(0, j - radius);
            int bottom = min(image.height - 1, j + radius);

            for (int x = left; x <= right; ++x) {
                for (int y = top; y <= bottom; ++y) {
                    // Get RGB of current pixel
                    unsigned int sum = 0;
                    for (int k = 0; k < image.channels; ++k) {
                        sum += image(x, y, k);
                    }

                    // Calculate intensity level (0 to intensitylevels-1)
                    unsigned int intensity = ((sum) / 3 * (intensitylevels - 1)) / 255;

                    // Update histogram and color sums for this intensity level
                    Hist[intensity]++;
                    rSum[intensity] += image(x, y, 0);
                    gSum[intensity] += image(x, y, 1);
                    bSum[intensity] += image(x, y, 2);
                }
            }
            // Find the most frequent intensity level in neighborhood
            int maxCount = 0, maxIndex = 0;
            for (int x = 0; x < intensitylevels; ++x) {
                if (Hist[x] > maxCount) {
                    maxCount = Hist[x];
                    maxIndex = x;
                }
            }
            oiled(i, j, 0) = rSum[maxIndex] / maxCount;
            oiled(i, j, 1) = gSum[maxIndex] / maxCount;
            oiled(i, j, 2) = bSum[maxIndex] / maxCount;
        }
    }
    image = oiled;
}

// Apply TV effect
void tv(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; j += 2) {
            for (int k = 0; k < image.channels; ++k) {
                image(i, j, k) = image(i, j, k) * 0.6; // Darken every other line
                // Add random noise
                if (rand() % 100 < 5) {
                    image(i, j, k) = (rand() % 2) ? 0 : 255;
                }
            }
        }
    }
}

// Skew image
void skew(Image& image) {
    cout << "Enter the skew degree (-45 to 45): ";
    float degree;
    while (!(cin >> degree && degree >= -45 && degree <= 45)) {
        cout << "Invalid input. Enter a number between -45 and 45: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Convert degrees to radians
    float radian = degree * M_PI / 180.0;
    float tan_angle = tan(radian);

    unsigned int max_shift = abs(image.height * tan_angle); // Calculate maximum shift

    // Add white background
    Image skewed(image.width + max_shift, image.height);
    for (int i = 0; i < skewed.width; ++i) {
        for (int j = 0; j < skewed.height; ++j) {
            for (int k = 0; k < skewed.channels; ++k) {
                skewed.setPixel(i, j, k, 255);
            }
        }
    }

    // Calculate x offset for negative angles
    unsigned int offset_x = 0;
    if (tan_angle < 0) {
        offset_x = abs((image.height - 1) * tan_angle);
    }

    for (int j = 0; j < image.height; ++j) {
        float shift = (image.height - 1 - j) * tan_angle;
        for (int i = 0; i < image.width; ++i) {
            unsigned int new_x = i + shift + offset_x;
            if (new_x < skewed.width) {
                for (int k = 0; k < image.channels; ++k) {
                    skewed(new_x, j, k) = image(i, j, k);
                }
            }
        }
    }
    image = skewed;
}

// Display filter menu and apply selected filter
void filter_menu(Image& image) {
    cout << "Choose the filter you want to apply:\n";
    cout << "1) Gray\n2) Black & White\n3) Invert\n4) Merge\n5) Flip" << endl;
    cout << "6) Rotate\n7) Brightness\n8) Crop\n9) Frame\n10) Edge detection" << endl;
    cout << "11) Resize\n12) Blur\n13) Sunlight\n14) Purple\n15) Infrared" << endl;
    cout << "16) Oil Painting\n17) TV Effect\n18) Skew\n==>";
    int choice;
    while (!(cin >> choice && (choice >= 1 && choice <= 18))) {
        cout << "Invalid choice. Enter a number between 1 and 18: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (choice == 1) {
        gray(image);
    }
    else if (choice == 2) {
        black_white(image);
    }
    else if (choice == 3) {
        invert(image);
    }
    else if(choice == 4) {
        merge(image);
    }
    else if(choice == 5) {
        flip(image);
    }
    else if (choice == 6) {
        rotate(image);
    }
    else if(choice == 7) {
        brightness(image);
    }
    else if(choice == 8) {
        crop(image);
    }
    else if(choice == 9) {
        frame(image);
    }
    else if(choice == 10) {
        edges(image);
    }
    else if (choice == 11) {
        resize(image);
    }
    else if (choice == 12) {
        blur(image);
    }
    else if (choice == 13) {
        sunlight(image);
    }
    else if (choice == 14) {
        purple(image);
    }
    else if (choice == 15) {
        infrared(image);
    }
    else if (choice == 16) {
        oil(image);
    }
    else if (choice == 17) {
        tv(image);
    }
    else if (choice == 18) {
        skew(image);
    }
    cout << "Filter applied successfully" << endl;
    ptr = &image;
}

// Save the current image to file
void saving() {
    string saved;
    if (ptr == nullptr) {
        cout << "No image to save. Please load an image first." << endl;
        main_menu();
    }
    cout << "Enter the name of the image to save with extension (.png or .jpeg or .jpg or .bmp): ";
    while (true) {
        cin >> saved;
        string ext;
        if (saved.length() > 4) {
            ext = saved.substr(saved.length() - 4);
        }
        string ext_jpeg;
        if (saved.length() > 5) {
            ext_jpeg = saved.substr(saved.length() - 5);
        }

        // Validate file extension
        if (ext == ".jpg" || ext == ".png" || ext == ".bmp" || ext_jpeg == ".jpeg") {
            ptr->saveImage(saved);
            cout << "Image saved as " << saved << endl;
            ptr = nullptr;
            break;
        }
        else {
            cout << "Invalid file format. Please use .jpg, .jpeg, .png, or .bmp: ";
        }
    }
}

// Main menu with program flow control
void main_menu() {
    Image image;
    string img;
    while (true) {
        cout << "--- Main Menu ---" << endl;
        cout << "1. Load image" << endl;
        cout << "2. Save image" << endl;
        cout << "3. Apply another filter to the image" << endl;
        cout << "4. Exit\n==>";
        char choice;
        while (!(cin >> choice) || (choice != '1' && choice != '2' && choice != '3' && choice != '4')) {
            cout << "Invalid choice. Enter a number between 1 and 4: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        // Load image
        if (choice == '1') {
            // Handle case where image is already loaded
            if(ptr != nullptr) {
                cout << "There is a loaded image already. What to do?" << endl;
                cout << "1) Ignore the old image and work on a new one\n2) Return to the main menu\n==>";
                char c;
                while (!(cin >> c) || (c != '1' && c != '2')) {
                    cout << "Invalid choice. Enter 1 or 2: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (c == '2') {
                    main_menu();
                }
            }

            cout << "Enter the name of the image to load with extension (.png or .jpeg or .jpg or .bmp): ";
            cin >> img;
            try {
                image.loadNewImage(img);
                filter_menu(image);
            }
            catch (const invalid_argument& e) {}
        }

        // Save image
        else if (choice == '2') {
            saving();
        }

        // Apply another filter
        else if (choice == '3') {
            if (ptr == nullptr) {
                cout << "There is no image loaded." << endl;
                main_menu();
            }
            else {
                filter_menu(image);
            }
        }

        // Exit
        else if (choice == '4') {
            // Prompt to save the loaded image before exiting
            if (ptr != nullptr) {
                cout << "Are you sure you want to exit without saving?" << endl;
                cout << "1) Yes 2) No =>";
                char c;
                while (!(cin >> c) || (c != '1' && c != '2')) {
                    cout << "Invalid choice. Enter 1 or 2: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                if (c == '1') {
                    exit(0);
                }
                else if (c == '2') {
                    saving();
                }
            }
            else {
                exit(0);
            }
        }
    }
}

int main() {
    cout << "====== Welcome to the Image Filter App ======\n";
    main_menu();
    return 0;
}
