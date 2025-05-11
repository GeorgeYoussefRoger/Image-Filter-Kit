#include <iostream>
#include <string>
#include <stdexcept>
#include "Image_Class.h"
#include <limits>

using namespace std;

// Global pointer to hold the current image
Image* ptr = nullptr;

// Function prototypes
void mainmenu();
void applyFilter(Image& image);
void save_image();

// Apply filter functions
void invert_photo(Image& image) {
    // Implement inversion logic here
    for (int i=0;i<image.width;i++) {
       for (int j= 0 ;j<image.height;j++) {
          for(int k=0;k<image.channels;k++) {
            image(i,j,k)=255-image(i,j,k);
          }
       }
    }
}

void infrared(Image& img) {
    // Implement infrared effect logic here
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {

            img(i, j, 0) = 255;
            img(i, j, 1) = 255 - img(i, j, 1);
            img(i, j, 2) = 255 - img(i, j, 2);
        }
    }
}

void sunlight(Image& image) {
    // Implement sunlight effect logic here
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            image(i, j, 0) *= 1.1;
            image(i, j, 1) *= 1.0;
            image(i, j, 2) *= 0.8;
        }
    }
}

void blur(Image& image) {
    // Implement blur effect logic here
    Image img2 = image; // Make a copy to hold the original values
    int kernelSize = 15;
    int kernelRadius = kernelSize / 2;
    for (int i = kernelRadius; i < image.width - kernelRadius; i++) {
        for (int j = kernelRadius; j < image.height - kernelRadius; j++) {
            for (int k = 0; k < image.channels; k++) {
                int sum = 0;
                for (int m = -kernelRadius; m <= kernelRadius; m++) {
                    for (int n = -kernelRadius; n <= kernelRadius; n++) {
                        sum += img2(i + m, j + n, k);
                    }
                }
                image(i, j, k) = sum / (kernelSize * kernelSize);
            }
        }
    }
}


void gray(Image& image) {
    // Implement grayscale conversion logic here
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height; j++) {
            unsigned int avg = 0;
            for (int k = 0; k < image.channels; k++) {
                avg += image(i, j, k);
            }
            avg /= image.channels;
            for (int k = 0; k < image.channels; k++) {
                image(i, j, k) = avg;

            }
        }
    }
}

void frame(Image& img) {
    // Implement frame effect logic here
    string choice;
    cout<<"Choose:\n1) Normal frame\n2) Decorated frame";
    cin>>choice;
    if (choice=="1"){
        int red, green, blue;
        cout << "Enter the frame colors (red, green, blue):";
        if (!(cin >> red >> green >> blue)) {
            cout << "Invalid input. Please enter a number." << endl;
            // Clear the error state of cin and ignore the invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            frame(img);
        }

        int frame_size = 50;
        int new_width = img.width + 2 * frame_size;
        int new_height = img.height + 2 * frame_size;

        Image newimg(new_width, new_height);

        for (int i = 0; i < new_width; i++)
        {
            for (int j = 0; j < new_height; j++)
            {
                newimg(i, j, 0) = red;
                newimg(i, j, 1) = green;
                newimg(i, j, 2) = blue;
            }
        }

        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    newimg(i + frame_size, j + frame_size, k) = img(i, j, k);
                }
            }
        }
        img=newimg;
    }
    else if (choice=="2")
    {
        int frame_size = 50;
        int new_width = img.width + 2 * frame_size;
        int new_height = img.height + 2 * frame_size;

        Image newimg(new_width, new_height);

        for (int i = 0; i < new_width; i++)
        {
            for (int j = 0; j < new_height; j++)
            {
                for(int k=0;k<3;k++){
                    newimg(i,j,k)=255;

                }
            }
        }

        for (int i = 20; i < new_width-20; i++)
        {
            for (int j = 20; j < new_height-20; j++)
            {
                newimg(i, j, 0) = 0;
                newimg(i, j, 1) = 0;
                newimg(i, j, 2) = 255;
            }
        }


        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    newimg(i + frame_size, j + frame_size, k) = img(i, j, k);
                }
            }
        }
        img =newimg;
    }
    else
    {
        cout << "Invalid choice.\n";
        frame(img);
    }
}

void menu_rotation(Image& img) {
    // Implement image rotation logic here
    cout << "Choose:\n1) 90 degree\n2) 180 degree\n3) -90 degree\n";
    Image img2;
    string choice;
    cin >> choice;
    if (choice == "1") {
        Image img2(img.height, img.width);
        for (int i = 0; i < img.height; i++) {
            for (int j = 0; j < img.width; j++) {
                for (int k = 0; k < img.channels; k++) {
                    img2(i, j, k) = img(j, i, k);
                }
            }
        }
        img = img2;
    }
    else if (choice == "2") {
        Image img2(img.width, img.height);
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                for (int k = 0; k < img.channels; k++) {
                    img2(i, j, k) = img(img.width - 1 - i, img.height - 1 - j, k);

                }
            }
        }
        img = img2;
    }
    else if (choice == "3") {
        Image img2(img.height, img.width);
        for (int i = img.height - 1; i >= 0; i--) {
            for (int j = img.width - 1; j >= 0; j--) {
                for (int k = 0; k < img.channels; k++) {
                    img2(i, j, k) = img(j, i, k);
                }
            }
        }
        img = img2;
    }
    else {
        cout << "Invalid choice.\n";
        menu_rotation(img);
    }
}

void merge(Image& img) {
    cout << "Please Enter The Name of the Second Image with (.png, .jpeg, .jpg, .bmp):";
    string photo2;
    cin >> photo2;
    Image img2(photo2);
    while (true) {
        try {
            img2.loadNewImage(photo2);
            break;
        }
        catch (invalid_argument) {
            cout << "Invalid Photo" << endl;
            merge(img);
        }
    }
    cout << "Choose:\n";
    cout << "1. Resize to the biggest width and height\n";
    cout << "2. Merge the common area of the smaller width and height\n";
    string choice;
    cin >> choice;
    if (choice == "1") {
        int new_width = (img.width > img2.width) ? img.width : img2.width;
        int new_height = (img.height > img2.height) ? img.height : img2.height;
        Image resized_image1(new_width, new_height);
        Image resized_image2(new_width, new_height);

        // Resize img1
        for (int i = 0; i < new_width; i++) {
            for (int j = 0; j < new_height; j++) {
                int orig_width = i * img.width / new_width;
                int orig_height = j * img.height / new_height;
                for (int k = 0; k < img.channels; k++) {
                    resized_image1.setPixel(i, j, k, img.getPixel(orig_width, orig_height, k));
                }
            }
        }

        // Resize img2
        for (int i = 0; i < new_width; i++) {
            for (int j = 0; j < new_height; j++) {
                int orig_width = i * img2.width / new_width;
                int orig_height = j * img2.height / new_height;
                for (int k = 0; k < img2.channels; k++) {
                    resized_image2.setPixel(i, j, k, img2.getPixel(orig_width, orig_height, k));
                }
            }
        }
        // Merge the resized images
        Image merged_image(new_width, new_height);
        for (int i = 0; i < new_width; i++) {
            for (int j = 0; j < new_height; j++) {
                for (int k = 0; k < img.channels; k++) {
                    int pixel1 = resized_image1.getPixel(i, j, k);
                    int pixel2 = resized_image2.getPixel(i, j, k);
                    merged_image.setPixel(i, j, k, max(pixel1, pixel2));
                }
            }
        }
        img = merged_image;
    } else if (choice == "2") {
        // Determine the common area
        int common_width = (img.width = img2.width) ? img.width : (img.height = img2.height) ? img.height : img2.height;
        int common_height = (img.height = img2.height) ? img.height : (img.width = img2.width) ? img.width : img2.width;

        // Merge the common area
        Image merged_image(common_width, common_height);
        for (int i = 0; i < common_width; i++) {
            for (int j = 0; j < common_height; j++) {
                for (int k = 0; k < img.channels; k++) {
                    int pixel1 = img.getPixel(i, j, k);
                    int pixel2 = img2.getPixel(i, j, k);
                    merged_image.setPixel(i, j, k, max(pixel1, pixel2));
                }
            }
        }
        img = merged_image;
    }
    else {
        cout << "Invalid Choice.\n";
        merge(img);
    }
}

void brightness(Image& img) {
    float percentage;
    cout << "Enter the percentage to adjust brightness (-50 to 50):";
    if (!(cin >> percentage)) {
        cout << "Invalid input. Please enter a number." << endl;
        // Clear the error state of cin and ignore the invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        brightness(img);
    }
    if (percentage >= -50 && percentage <= 50) {
        float factor = 1 + (percentage / 100);
        for (int i = 0; i < img.width; i++) {
            for (int j = 0; j < img.height; j++) {
                for (int k = 0; k < img.channels; k++) {
                    // Calculate index of current pixel in the image array
                    int index = (j * img.width + i) * img.channels + k;
                    // Calculate new pixel value
                    int new_value = img.imageData[index] * factor;
                    // Make the value between [0, 255]
                    img.imageData[index] = (new_value < 0) ? 0 : ((new_value > 255) ? 255 : new_value);
                }
            }
        }
    }
    else {
        cout << "Invalid Value.\n";
        brightness(img);
    }
}

void edges(Image& img) {
    // Converting the image to black and white
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            unsigned  int avg = 0;
            for (int k = 0; k < 3; k++) {
                avg += img(i, j, k); // Accumulate pixel values
            }
            avg /= 3; // Calculate average

            // Set all channels to the average value
            img(i, j, 0) = avg;
            img(i, j, 1) = avg;
            img(i, j, 2) = avg;

            if (avg > 127) {
                img(i, j, 0) = img(i, j, 1) = img(i, j, 2) = 225;
            }
            else {
                img(i, j, 0) = img(i, j, 1) = img(i, j, 2) = 0;
            }
        }
    }

    // Applying Sobel edge detection method
    int horizontal[3][3] = {{-1,0,1},
                            {-2,0,2},
                            {-1,0,1}};

    int vertical[3][3] = {{-1,-2,-1},
                          {0,0,0},
                          {1,2,1}};

    // Create temporary image to store gradient magnitudes
    Image magnitude(img.width, img.height);

    // Apply edges operator to calculate gradient magnitudes
    for (int i = 1; i < img.width - 1; i++) {
        for (int j = 1; j < img.height - 1; j++) {
            // Calculate gradient in x and y direction
            int magnitudeX = 0, magnitudeY = 0;
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    magnitudeX += horizontal[x + 1][y + 1] * img(i + x, j + y, 0);
                    magnitudeY += vertical[x + 1][y + 1] * img(i + x, j + y, 0);
                }
            }
            // Store gradient magnitude in temporary image
            magnitude(i, j, 0) = sqrt(magnitudeX * magnitudeX + magnitudeY * magnitudeY);
        }
    }
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            // If gradient magnitude is not zero set as black (0), else white (255)
            img(i, j, 0) = magnitude(i, j, 0) ? 0 : 255;
            img(i, j, 1) = img(i, j, 0); // Set other channels the same
            img(i, j, 2) = img(i, j, 0); // Set other channels the same
        }
    }
}

void flip(Image& img) {
    cout << "Choose:\n1) flip horizontally\n2) flip vertically\n";
    string choice;
    cin >> choice;
    if (choice == "1") {
        for (int i = 0; i < img.width / 2; ++i) {
            for (int j = 0; j < img.height; ++j) {
                for (int k = 0; k < 3; ++k) {

                    unsigned int avg = img(i, j, k);
                    img(i, j, k) = img(img.width - 1 - i, j, k);
                    img(img.width - 1 - i, j, k) = avg;
                }
            }
        }
    }
    else if (choice == "2") {
        for (int i = 0; i < img.width; ++i) {
            for (int j = 0; j < img.height / 2; ++j) {
                for (int k = 0; k < 3; ++k) {

                    unsigned int avg = img(i, j, k);
                    img(i, j, k) = img(i, img.height - 1 - j, k);
                    img(i, img.height - 1 - j, k) = avg;
                }
            }
        }
    }
    else {
        cout << "Invalid Choice.\n";
        flip(img);
    }
}

void black_white(Image& img) {
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            unsigned  int avg = 0; // Initialize average value

            for (int k = 0; k < 3; ++k) {
                avg += img(i, j, k); // Accumulate pixel values
            }

            avg /= 3; // Calculate average

            // Set all channels to the average value
            img(i, j, 0) = avg;
            img(i, j, 1) = avg;
            img(i, j, 2) = avg;

            if(avg >127){
                img(i, j, 0) = img(i, j, 1) = img(i, j, 2) = 225;
            }
            else
                img(i, j, 0) = img(i, j, 1) = img(i, j, 2) = 0;
        }
    }
}

void resize(Image& img) {
    cout << "Choose:\n1) resize by dimensions\n2) resize by ratio\n";
    string choice;
    cin >> choice;
    if (choice == "1") {
        float w, h;
        cout << "Enter width then height " << endl;
        // Check if the input is a number
        if (!(cin >> w >> h)) {
            cout << "Invalid input. Please enter a number." << endl;
            // Clear the error state of cin and ignore the invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            resize(img);
        }
        Image newimg(w, h);
        float width = img.width;
        float height = img.height;
        float newwidth = w;
        float newheight = h;
        float sr = width / w;
        float sc = height / h;

        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
                for (int k = 0; k < 3; ++k) {
                    newimg(i, j, k) = img(round(i * sr), round(j * sc), k);
                }
            }
        }
        img = newimg;
    }else if (choice == "2") {
        float ratio_width;
        float ratio_height;
        cout << "Enter ratio_height then ratio_width " << endl;
        if (!(cin >> ratio_height>>ratio_width)) {
            cout << "Invalid input. Please enter a number." << endl;
            // Clear the error state of cin and ignore the invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            resize(img);
        }
        int  w,h;
         w = img.width * ratio_width;
         h = img.height * ratio_height;
        float f_width = img.width/w;
        float f_height = img.height/h;
        Image newimg(w, h);
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
                for (int k = 0; k < 3; ++k) {
                    newimg(i, j, k) = img(round(i*f_width), round(j*f_height), k);
                }
            }
        }
        img = newimg;
    }
}

void crop(Image& img) {
    cout << "image width = " << img.width;
    cout << "image height = " << img.height;
    cout << "\nPlease enter start point dimensions:";
    int x, y;
    // Check if the input is a number
    if (!(cin >> x >> y)) {
        cout << "Invalid input. Please enter a number." << endl;
        // Clear the error state of cin and ignore the invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        crop(img);
    }
    cout << "\nPlease enter the dimensions of area to cut:";
    int w, h;
    // Check if the input is a number
    if (!(cin >> w >> h)) {
        cout << "Invalid input. Please enter a number." << endl;
        // Clear the error state of cin and ignore the invalid input
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        crop(img);
    }
    Image cropped(w, h);
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            for(int k = 0; k < 3; k++){
                cropped(i, j, k) = img(w + i, h + j, k);
            }
        }
    }
    img=cropped;
}


void purple(Image& img) {
    for (int i = 0; i < img.width; ++i) {
        for (int j = 0; j < img.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                img(i, j, 0) = img(i, j, 0);
                img(i, j, 1) = img(i, j, 1)*0.83;
                img(i, j, 2) = img(i, j, 2);
            }
        }
    }
}

// Function to apply selected filter
void applyFilter(Image& image) {
    cout << "Choose what filter you want to apply:\n";
    cout << "1) Blur\n2) Grayscale\n3) Frame\n4) Rotation\n5) Invert\n";
    cout << "6) Infrared\n7) Sunlight\n8) Merge\n9) Brightness\n10) Edge detection\n";
    cout << "11) Flip\n12) Black & White\n13) Resize\n14) Crop\n15) Purple\n==>";
    string choice;
    cin >> choice;

    if (choice == "1") {
        blur(image);
    }
    else if (choice == "2") {
        gray(image);
    }
    else if(choice == "3"){
        frame(image);
    }
    else if(choice == "4"){
        menu_rotation(image);
    }
    else if(choice == "5"){
        invert_photo(image);
    }
    else if (choice == "6"){
        infrared(image);
    }
    else if(choice == "7"){
        sunlight(image);
    }
    else if(choice == "8"){
        merge(image);
    }
    else if(choice == "9"){
        brightness(image);
    }
    else if(choice == "10"){
        edges(image);
    }
    else if (choice == "11") {
        flip(image);
    }
    else if (choice == "12") {
        black_white(image);
    }
    else if (choice == "13") {
        resize(image);
    }
    else if (choice == "14") {
        crop(image);
    }
    else if (choice == "15") {
        purple(image);
    }
    else{
        cout<<"Enter valid choice"<<endl;
        applyFilter(image);
    }
    cout << "Filter Applied successfully" << endl;
    ptr=&image;
}

// Main menu function
void mainmenu() {
    Image image; // Create an instance of the Image class
    string photo;

    bool running = true;
    while (running) {
        cout << "Choose an option:" << endl;
        cout << "1. load image" << endl;
        cout << "2. Save image" << endl;
        cout << "3. Apply another filter to image" << endl;
        cout << "4. Exit" << endl;
        string choice;
        cin >> choice;

        if (choice == "1") {
            // Load image from file
            if(ptr!=nullptr){
                cout<<"Their is a loaded photo already"<<endl;
                cout<<"Choose 1. ignore the old image and work on new one\n"<<"return to main menu"<<endl;
                string ans;
                cin>>ans;
                if(ans=="1"){
                    
                }
                else if(ans=="2"){
                    mainmenu();
                }
            }
            
            cout << "Enter the name of the image file to load:";
            cin >> photo;

            try {
                image.loadNewImage(photo); // Assuming Image class has a method to load image
                applyFilter(image);
            }
            catch (const invalid_argument& e) {
                cout << "Invalid photo. please load the right image" << endl;
            }
        }
        else if (choice == "2") {
            save_image(); // Save the current image
        }
        else if (choice == "3") {
            if(ptr==nullptr){
                cout << "Enter another choice" << endl;
                mainmenu();
            }
            applyFilter(image);
            
        }
        else if (choice=="4"){
        if(ptr!=nullptr){
                cout<<"Are you Sure you want to exit Without saving"<<endl;
                cout<<"Choose 1. to exit 2. to save image"<<endl;
                string ch;
                cin>>ch;
                if(ch=="1"){
                    exit(0);
                }
                else if(ch=="2"){
                    save_image();
                    mainmenu();
                }

            }
            exit(0);
        }
        else {
            cout << "Invalid option, please choose again." << endl;
        }
        }
    
    }


// Function to save the image
void save_image() {
    string newimage;
    bool x = true;
    if (ptr == nullptr) {
        cout << "No image to save. Please load an image first." << endl;
        mainmenu();
    }
    while (x) {
        cout << "Enter the name of the image to save:";
        cin >> newimage;
        if (newimage.size() < 5) {
            cout << "Please enter a valid file name:";
            continue;
        }
        string ext = newimage.substr(newimage.size() - 4);
        string ext_long = newimage.substr(newimage.size() - 5);
        if (ext == ".jpg" || ext == ".png" || ext == ".bmp" || ext_long == ".jpeg") {
            ptr->saveImage(newimage);
            cout << "Image saved as " << newimage << endl;
            x = false;
            ptr=nullptr;
        }
        else {
            cout << "Enter a valid name:";
        }
    }
}


int main() {
    cout << "----- Welcome to Image Filter App -----\n";
    mainmenu();
    return 0;
}
