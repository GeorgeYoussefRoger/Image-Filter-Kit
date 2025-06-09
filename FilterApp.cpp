#include "Image_Class.h"
using namespace std;

Image* ptr = nullptr;

void main_menu();

void gray(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < image.channels; ++k) {
                avg += image(i, j, k);
            }
            avg /= image.channels;
            for (int k = 0; k < image.channels; ++k) {
                image.setPixel(i, j, k, avg);
            }
        }
    }
}

void black_white(Image& image) {
    gray(image);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < image.channels; ++k) {
                unsigned int avg = image(i, j, 0);
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

void invert(Image& image) {
    for (int i = 0; i < image.width; ++i) {
       for (int j = 0; j < image.height; ++j) {
          for (int k = 0; k < image.channels; ++k) {
            image.setPixel(i, j, k, 255 - image(i, j, k));
          }
       }
    }
}

void merge(Image& image) {
    string img2;
    Image image2;
    while (true) {
        cout << "Enter the name of the second image with extension (.png, .jpeg, .jpg, .bmp): ";
        cin >> img2;
        try {
            image2.loadNewImage(img2);
            break;
        } 
        catch (const invalid_argument& e) {}
    }
    if (image.width == image2.width && image.height == image2.height) { 
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    unsigned int pixel1 = image.getPixel(i, j, k);
                    unsigned int pixel2 = image2.getPixel(i, j, k);
                    image.setPixel(i, j, k, max(pixel1, pixel2));
                }
            }
        }
    }
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
        if (choice == '1') {
            unsigned int max_width = max(image.width, image2.width);
            unsigned int max_height = max(image.height, image2.height);
            Image merged(max_width, max_height);
            float scale_row1 = (float)image.width / max_width;
            float scale_column1 = (float)image.height / max_height;
            float scale_row2 = (float)image2.width / max_width;
            float scale_column2 = (float)image2.height / max_height;
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
        else if (choice == '2') {
            int common_width = min(image.width, image2.width);
            int common_height = min(image.height, image2.height);
            Image merged(common_width, common_height);
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

void flip(Image& image) {
    cout << "Choose flip direction:\n1. Flip Horizontally\n2. Flip Vertically\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2'))) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (choice == '1') {
        for (int i = 0; i < image.width / 2; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    swap(image(i, j, k), image(image.width - 1 - i, j, k));
                }
            }
        }
    }
    else if (choice == '2') {
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height / 2; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    swap(image(i, j, k), image(i, image.height - 1 - j, k));
                }
            }
        }
    }
}

void rotate(Image& image) {
    cout << "Choose rotation degree:\n1. 90 degree\n2. 180 degree\n3. 270 degree\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2' || choice == '3'))) {
        cout << "Invalid choice. Enter 1, 2, or 3: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
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

void brightness(Image& image) {
    cout << "Enter the percentage to adjust brightness (-50 to 50): ";
    float percentage;
    while (!(cin >> percentage && percentage >= -50 && percentage <= 50)) {
        cout << "Invalid input. Please enter a number between (-50 to 50): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    float factor = 1 + (percentage / 100);
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < image.channels; ++k) {
                float new_value = round(image(i, j, k) * factor);
                image(i, j, k) = new_value < 0 ? 0 : new_value > 255 ? 255 : new_value;
            }
        }
    }
}

void crop(Image& image) {
    cout << "Image width = " << image.width << endl;
    cout << "Image height = " << image.height << endl;
    unsigned int x, y;
    cout << "Enter top-left corner (x, y) of crop area: ";
    while (!(cin >> x >> y && x < image.width && y < image.height)) {
        cout << "Invalid input. Please enter valid image bounds: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    unsigned int w, h;
    cout << "Enter crop width and height: ";
    while (!(cin >> w >> h && x + w <= image.width && y + h <= image.height)) {
        cout << "Invalid input. Please enter valid width and height: ";
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

void frame(Image& image) {
    unsigned int frame_size;
    cout << "Enter frame thickness (10-100 pixels): ";
    while (!(cin >> frame_size && frame_size >= 10 && frame_size <= 100)) {
        cout << "Invalid input. Please enter a number between 10 and 100: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cout << "Choose frame style:\n1. Simple frame\n2. Decorated frame\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2'))) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    unsigned int new_width = image.width + 2 * frame_size;
    unsigned int new_height = image.height + 2 * frame_size;
    Image framed(new_width, new_height);
    if (choice == '1') {
        unsigned int r, g, b;
        cout << "Enter the frame colors (red, green, blue) from 0-255 for each: ";
        while (!(cin >> r >> g >> b && r <= 255 && g <= 255 && b <= 255)) {
            cout << "Invalid input. Please enter three numbers between 0 and 255: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        for (int i = 0; i < new_width; ++i) {
            for (int j = 0; j < new_height; ++j) {
                framed(i, j, 0) = r;
                framed(i, j, 1) = g;
                framed(i, j, 2) = b;
            }
        }
    }
    else if (choice == '2') {
        for (int i = 0; i < new_width; ++i) {
            for (int j = 0; j < new_height; ++j) {
                framed(i, j, 0) = 212;
                framed(i, j, 1) = 175;
                framed(i, j, 2) = 55;
            }
        }
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

void edges(Image& image) {
    gray(image);
    Image detected(image.width, image.height);
    int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int i = 1; i < image.width - 1; ++i) {
        for (int j = 1; j < image.height - 1; ++j) {
            long Gx = 0, Gy = 0;
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    Gx += sobel_x[x + 1][y + 1] * image(i + x, j + y, 0);
                    Gy += sobel_y[x + 1][y + 1] * image(i + x, j + y, 0);
                }
            }
            unsigned char value = (unsigned char)min(255.0, sqrt(Gx * Gx + Gy * Gy));
            for (int k = 0; k < image.channels; ++k) {
                detected.setPixel(i, j, k, value > 127 ? 0 : 255);
            }
        }
    }
    image = detected;
}

void resize(Image& image) {
    cout << "Choose resize method:\n1. By dimensions\n2. By ratio\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2'))) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    float w, h;
    if (choice == '1') {
        cout << "Enter width then height: ";
        while (!(cin >> w >> h)) {
            cout << "Invalid input. Please enter valid numbers: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
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

void blur(Image& image) {
    unsigned int kernelSize;
    cout << "Enter blur intensity (a positive odd number): ";
    while (!(cin >> kernelSize) || kernelSize % 2 == 0 || kernelSize < 1) {
        cout << "Invalid input. Please enter a positive odd number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    Image blurred = image;
    int kernelRadius = kernelSize / 2;
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

void sunlight(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image(i, j, 2) *= 0.5;
        }
    }
}

void purple(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image(i, j, 1) *= 0.5;
        }
    }
}

void infrared(Image& image) {
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            image.setPixel(i, j, 0, 255);
            image.setPixel(i, j, 1, 255 - image(i, j, 1));
            image.setPixel(i, j, 2, 255 - image(i, j, 2));
        }
    }
}

void filter_menu(Image& image) {
    cout << "Choose the filter you want to apply:\n";
    cout << "1) Gray\n2) Black & White\n3) Invert\n4) Merge\n5) Flip" << endl;
    cout << "6) Rotate\n7) Brightness\n8) Crop\n9) Frame\n10) Edge detection" << endl;
    cout << "11) Resize\n12) Blur\n13) Sunlight\n14) Purple\n15) Infrared\n==>";
    int choice;
    while (!(cin >> choice && (choice >= 1 && choice <= 15))) {
        cout << "Invalid choice. Enter a number between 1 and 15: ";
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
    cout << "Filter applied successfully" << endl;
    ptr = &image;
}

void saving() {
    string saved;
    if (ptr == nullptr) {
        cout << "No image to save. Please load an image first." << endl;
        main_menu();
    }
    cout << "Enter the name of the image to save with extension (.png, .jpeg, .jpg, .bmp): ";
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
        if (choice == '1') {
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
            cout << "Enter the name of the image to load with extension (.png, .jpeg, .jpg, .bmp): ";
            cin >> img;
            try {
                image.loadNewImage(img);
                filter_menu(image);
            }
            catch (const invalid_argument& e) {}
        }
        else if (choice == '2') {
            saving();
        }
        else if (choice == '3') {
            if (ptr == nullptr) {
                cout << "There is no image loaded." << endl;
                main_menu();
            }
            else {
                filter_menu(image);
            }
        }
        else if (choice == '4') {
            if (ptr != nullptr) {
                cout << "Are you sure you want to exit without saving?" << endl;
                cout << "1) Yes 2) No" << endl;
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
