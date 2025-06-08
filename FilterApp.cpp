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
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0;
            for (int k = 0; k < image.channels; ++k) {
                avg += image(i, j, k); 
            }
            avg /= image.channels;
            for (int k = 0; k < image.channels; ++k) {
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
        cout << "The images are of different sizes. What to do?\n";
        cout << "1. Resize to the biggest width and height\n";
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

            Image resized1(max_width, max_height);
            for (int i = 0; i < max_width; ++i) {
                for (int j = 0; j < max_height; ++j) {
                    for (int k = 0; k < image.channels; ++k) {
                        float width = image.width;
                        float height = image.height;
                        float scale_row = width / max_width;
                        float scale_column = height / max_height;
                        resized1.setPixel(i, j, k, image.getPixel(round(i * scale_row), round(j * scale_column), k));
                    }
                }
            }

            Image resized2(max_width, max_height);
            for (int i = 0; i < max_width; ++i) {
                for (int j = 0; j < max_height; ++j) {
                    for (int k = 0; k < 3; ++k) {
                        float width = image2.width;
                        float height = image2.height;
                        float scale_row = width / max_width;
                        float scale_column = height / max_height;
                        resized2.setPixel(i, j, k, image2.getPixel(round(i * scale_row), round(j * scale_column), k));
                    }
                }
            }

            Image merged(max_width, max_height);
            for (int i = 0; i < max_width; ++i) {
                for (int j = 0; j < max_height; ++j) {
                    for (int k = 0; k < image.channels; ++k) {
                        unsigned char pixel1 = resized1.getPixel(i, j, k);
                        unsigned char pixel2 = resized2.getPixel(i, j, k);
                        merged.setPixel(i, j, k, max(pixel1, pixel2));
                    }
                }
            }
            image = merged; 
        }
        else {
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
    cout << "Choose:\n1) Flip Horizontally\n2) Flip Vertically\n=>";
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
                    unsigned char tmp = image(i, j, k);
                    image(i, j, k) = image(image.width - 1 - i, j, k);
                    image(image.width - 1 - i, j, k) = tmp;
                }
            }
        }
    }
    else {
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height / 2; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    unsigned char tmp = image(i, j, k);
                    image(i, j, k) = image(i, image.height - 1 - j, k);
                    image(i, image.height - 1 - j, k) = tmp;
                }
            }
        }
    }
}

void rotate(Image& image) {
    cout << "Choose:\n1) 90 degree\n2) 180 degree\n3) 270 degree\n=>";
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
    else {
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
    while (!(cin >> x >> y && x >= 0 && y >= 0 && x < image.width && y < image.height)) {
        cout << "Invalid input. Please enter valid image bounds: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    unsigned int w, h;
    cout << "Enter crop width and height: ";
    while (!(cin >> w >> h && w > 0 && h > 0 && x + w <= image.width && y + h <= image.height)) {
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
    unsigned int new_width = image.width + 2 * frame_size;
    unsigned int new_height = image.height + 2 * frame_size;
    Image framed(new_width, new_height);

    cout << "Choose:\n1) Simple frame\n2) Decorated frame\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2'))) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (choice == '1') {
        unsigned int r, g, b;
        cout << "Enter the frame colors (red, green, blue) from 0-255 for each: ";
        while (!(cin >> r >> g >> b && r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)) {
            cout << "Invalid input. Please enter valid numbers: ";
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
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    framed(i + frame_size, j + frame_size, k) = image(i, j, k);
                }
            }
        }
        image = framed;
    }
    else {
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
        for (int i = frame_size; i < new_width - frame_size; ++i) {
            for (int j = frame_size; j < new_height - frame_size; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    framed(i, j, k) = image(i - frame_size, j - frame_size, k);
                }
            }
        }
        image = framed;
    }
}

void edges(Image& image) {
    gray(image);

    int horizontal[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
    int vertical[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};
    Image magnitude(image.width, image.height);
    
    for (int i = 1; i < image.width - 1; ++i) {
        for (int j = 1; j < image.height - 1; ++j) {
            unsigned int magnitudeX = 0, magnitudeY = 0;
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    magnitudeX += horizontal[x + 1][y + 1] * image(i + x, j + y, 0);
                    magnitudeY += vertical[x + 1][y + 1] * image(i + x, j + y, 0);
                }
            }
            magnitude(i, j, 0) = sqrt(magnitudeX * magnitudeX + magnitudeY * magnitudeY);
        }
    }
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            for (int k = 0; k < image.channels; ++k) {
            image.setPixel(i, j, k, magnitude(i, j, 0) > 127 ? 0 : 255);
            }
        }
    }
}

void resize(Image& image) {
    cout << "Choose:\n1) Resize by dimensions\n2) Resize by ratio\n=>";
    char choice;
    while (!(cin >> choice && (choice == '1' || choice == '2'))) {
        cout << "Invalid choice. Enter 1 or 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (choice == '1') {
        float w, h;
        cout << "Enter width then height: " << endl;
        while (!(cin >> w >> h)) {
            cout << "Invalid input. Please enter valid numbers: " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        Image resized(w, h);
        float width = image.width;
        float height = image.height;
        float scale_row = width / w;
        float scale_column = height / h;
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    resized(i, j, k) = image(round(i * scale_row), round(j * scale_column), k);
                }
            }
        }
        image = resized;
    }
    else {
        float ratio_width;
        float ratio_height;
        cout << "Enter width ratio then height ratio: " << endl;
        while (!(cin >> ratio_width >> ratio_height)) {
            cout << "Invalid input. Please enter valid numbers: " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        unsigned int w, h;
        w = image.width * ratio_width;
        h = image.height * ratio_height;
        float f_width = image.width / w;
        float f_height = image.height / h;
        Image resized(w, h);
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
                for (int k = 0; k < image.channels; ++k) {
                    resized(i, j, k) = image(round(i * f_width), round(j * f_height), k);
                }
            }
        }
        image = resized;
    }
}

void blur(Image& image) {
    Image blurred = image;
    unsigned int kernelSize = 15;
    unsigned int kernelRadius = kernelSize / 2;
    for (unsigned int i = kernelRadius; i < image.width - kernelRadius; ++i) {
        for (unsigned int j = kernelRadius; j < image.height - kernelRadius; ++j) {
            for (unsigned int k = 0; k < image.channels; ++k) {
                unsigned int sum = 0;
                for (int m = -kernelRadius; m <= kernelRadius; ++m) {
                    for (int n = -kernelRadius; n <= kernelRadius; ++n) {
                        sum += blurred(i + m, j + n, k);
                    }
                }
                image(i, j, k) = sum / (kernelSize * kernelSize);
            }
        }
    }
}

void sunlight(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image(i, j, 0) *= 1.1;
            image(i, j, 1) *= 1.0;
            image(i, j, 2) *= 0.8;
        }
    }
}

void purple(Image& image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            image.setPixel(i, j, 1, image(i, j, 1) * 0.83);
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
    cout << "1) Gray\n2) Black & White\n3) Invert\n4) Merge\n5) Flip\n";
    cout << "6) Rotate\n7) Brightness\n8) Crop\n9) Frame\n10) Edge detection\n";
    cout << "11) Resize\n12) Blur\n13) Sunlight\n14) Purple\n15) Infrared\n==>";
    char choice;
    while (!(cin >> choice && (choice >= '1' && choice <= '15'))) {
        cout << "Invalid choice. Enter a number between 1 and 15: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    if (choice == '1') {
        gray(image);
    }
    else if (choice == '2') {
        black_white(image);
    }
    else if (choice == '3') {
        invert(image);
    }
    else if(choice == '4') {
        merge(image);
    }
    else if(choice == '5') {
        flip(image);
    }
    else if (choice == '6') {
        rotate(image);
    }
    else if(choice == '7') {
        brightness(image);
    }
    else if(choice == '8') {
        crop(image);
    }
    else if(choice == '9') {
        frame(image);
    }
    else if(choice == '10') {
        edges(image);
    }
    else if (choice == '11') {
        resize(image);
    }
    else if (choice == '12') {
        blur(image);
    }
    else if (choice == '13') {
        sunlight(image);
    }
    else if (choice == '14') {
        purple(image);
    }
    else {
        infrared(image);
    }
    cout << "Filter applied successfully" << endl;
    ptr = &image;
}

void saving() {
    string saved;
    bool running = true;
    if (ptr == nullptr) {
        cout << "No image to save. Please load an image first." << endl;
        main_menu();
    }
    while (running) {
        cout << "Enter the name of the image to save: ";
        cin >> saved;
        if (saved.size() < 5) {
            cout << "Please enter a valid file name: ";
            continue;
        }
        string ext = saved.substr(saved.size() - 4);
        string ext_long = saved.substr(saved.size() - 5);
        if (ext == ".jpg" || ext == ".png" || ext == ".bmp" || ext_long == ".jpeg") {
            ptr->saveImage(saved);
            cout << "Image saved as " << saved << endl;
            running = false;
            ptr = nullptr;
        }
        else {
            cout << "Invalid file format. Please use .jpg, .jpeg, .png, or .bmp." << endl;
            saving();
        }
    }
}

void main_menu() {
    Image image;
    string img;
    while (true) {
        cout << "Choose an option:" << endl;
        cout << "1. Load image" << endl;
        cout << "2. Save image" << endl;
        cout << "3. Apply another filter to the image" << endl;
        cout << "4. Exit\n==>";
        string choice;
        while (true) {
        cin >> choice;
        if (choice == "1" || choice == "2" || choice == "3" || choice == "4") break;
        cout << "Invalid choice. Enter a number between 1 and 4: ";
        }
        if (choice == "1") {
            if(ptr != nullptr) {
                cout << "There is a loaded image already" << endl;
                cout << "Choose\n1) Ignore the old image and work on a new one\n2) Return to the main menu" << endl;
                string c;
                while (true) {
                    cin >> c;
                    if (c == "1" || c == "2") break;
                    cout << "Invalid choice. Enter 1 or 2: ";
                }
                if (c == "1") {}
                else if (c == "2") {
                    main_menu();
                }
            }
            cout << "Enter the name of the image file to load: ";
            cin >> img;
            try {
                image.loadNewImage(img);
                filter_menu(image);
            }
            catch (const invalid_argument& e) {
                cout << "Invalid photo. please load the right image" << endl;
            }
        }
        else if (choice == "2") {
            saving();
        }
        else if (choice == "3") {
            if (ptr == nullptr) {
                cout << "There is no image loaded." << endl;
                main_menu();
            }
            else {
                filter_menu(image);
            }
        }
        else if (choice == "4") {
            if (ptr != nullptr) {
                cout << "Are you sure you want to exit without saving?" << endl;
                cout << "1) Yes 2) No" << endl;
                string c;
                while (true) {
                    cin >> c;
                    if (c == "1" || c == "2") break;
                    cout << "Invalid choice. Enter 1 or 2: ";
                }
                if (c == "1") {
                    exit(0);
                }
                else if (c == "2") {
                    saving();
                    main_menu();
                }
            }
            else {
                exit(0);
            }
        }
    }
}

int main() {
    cout << "----- Welcome to the Image Filter App -----\n";
    main_menu();
    return 0;
}
