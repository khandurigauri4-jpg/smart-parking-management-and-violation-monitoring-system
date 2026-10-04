#include <iostream>
#include <string>
#include <cctype>

#include <opencv2/opencv.hpp>
#include <opencv2/geometry/2d.hpp>
#include <tesseract/baseapi.h>

using namespace std;
using namespace cv;

int main()
{
    // Open webcam
    VideoCapture camera(0);

    if (!camera.isOpened())
    {
        cout << "Error: Camera could not be opened." << endl;
        return 1;
    }

    cout << "Camera started successfully." << endl;
    cout << "Press SPACE to capture an image." << endl;
    cout << "Press ESC to exit." << endl;

    Mat frame;

    while (true)
    {
        // Get image from camera
        camera >> frame;

        if (frame.empty())
        {
            cout << "Error: Could not capture image." << endl;
            break;
        }

        // Show camera
        imshow("Smart Parking - Camera", frame);

        char key = waitKey(1);

        // ESC = exit
        if (key == 27)
        {
            break;
        }

        // SPACE = capture
        if (key == 32)
        {
            cout << "\nImage captured." << endl;

            // Convert image to grayscale
            Mat gray;
            cvtColor(frame, gray, COLOR_BGR2GRAY);

            // Reduce noise
            Mat blurred;
            GaussianBlur(gray, blurred, Size(5, 5), 0);

            // Detect edges
            Mat edges;
            Canny(blurred, edges, 100, 200);

            // Find contours
            vector<vector<Point>> contours;

            findContours(
                edges,
                contours,
                RETR_LIST,
                CHAIN_APPROX_SIMPLE
            );

            Rect plate;
            bool plateFound = false;

            // Check possible license plate areas
            for (size_t i = 0; i < contours.size(); i++)
            {
                Rect rect = boundingRect(contours[i]);

                float width = rect.width;
                float height = rect.height;

                if (height == 0)
                    continue;

                float ratio = width / height;

                // Normal license plate is wider than it is tall
                if (ratio >= 2.0 && ratio <= 6.0 &&
                    rect.area() > 1000)
                {
                    plate = rect;
                    plateFound = true;
                    break;
                }
            }

            if (!plateFound)
            {
                cout << "License plate not detected." << endl;
                continue;
            }

            // Crop license plate
            Mat plateImage = frame(plate);

            imshow("Detected License Plate", plateImage);

            cout << "License plate detected." << endl;

            // OCR


            tesseract::TessBaseAPI ocr;

            if (ocr.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY) != 0)
            {
                cout << "Error: Tesseract could not start." << endl;
                continue;
            }

            // Convert plate to grayscale
            Mat plateGray;
            cvtColor(plateImage, plateGray, COLOR_BGR2GRAY);

            // Improve image for OCR
            Mat plateThreshold;

            threshold(
                plateGray,
                plateThreshold,
                0,
                255,
                THRESH_BINARY + THRESH_OTSU
            );

            imshow("Plate for OCR", plateThreshold);

            // Give image to Tesseract
            ocr.SetImage(
                plateThreshold.data,
                plateThreshold.cols,
                plateThreshold.rows,
                1,
                plateThreshold.step
            );

            // Get recognized text
            string result = ocr.GetUTF8Text();

            // Remove unnecessary spaces/newlines
            string cleanNumber = "";

            for (char c : result)
            {
                if (isalnum(c))
                {
                    cleanNumber += toupper(c);
                }
            }

            cout << "Vehicle Number: " << cleanNumber << endl;

            ocr.End();


        }
    }

    camera.release();
    destroyAllWindows();

    return 0;
}