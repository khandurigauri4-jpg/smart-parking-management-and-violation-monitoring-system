#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

using namespace std;
using namespace cv;


class Vehicle
{
protected:
    string vehicleNumber;

public:
    Vehicle(string number)
    {
        vehicleNumber = number;
    }

    virtual void displayVehicleType()
    {
        cout << "Vehicle Type: General Vehicle" << endl;
    }

    string getVehicleNumber()
    {
        return vehicleNumber;
    }

    virtual ~Vehicle()
    {
    }
};


class TwoWheeler : public Vehicle
{
public:

    TwoWheeler(string number) : Vehicle(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Category: Two Wheeler" << endl;
    }
};


class FourWheeler : public Vehicle
{
public:

    FourWheeler(string number) : Vehicle(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Category: Four Wheeler" << endl;
    }
};


class Bike : public TwoWheeler
{
public:

    Bike(string number) : TwoWheeler(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Type: Bike" << endl;
        cout << "Vehicle Category: Two Wheeler" << endl;
    }
};



class Scooty : public TwoWheeler
{
public:

    Scooty(string number) : TwoWheeler(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Type: Scooty" << endl;
        cout << "Vehicle Category: Two Wheeler" << endl;
    }
};


class Car : public FourWheeler
{
public:

    Car(string number) : FourWheeler(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Type: Car" << endl;
        cout << "Vehicle Category: Four Wheeler" << endl;
    }
};



class Truck : public FourWheeler
{
public:

    Truck(string number) : FourWheeler(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Type: Truck" << endl;
        cout << "Vehicle Category: Four Wheeler" << endl;
    }
};



class ParkingSlot
{
private:

    int slotNumber;
    bool available;
    string vehicleNumber;

public:

    ParkingSlot(int number)
    {
        slotNumber = number;
        available = true;
        vehicleNumber = "";
    }

    bool isAvailable()
    {
        return available;
    }

    void parkVehicle(string number)
    {
        if (available)
        {
            vehicleNumber = number;
            available = false;

            cout << "Vehicle automatically assigned to Slot "
                 << slotNumber << endl;
        }
        else
        {
            cout << "Slot " << slotNumber
                 << " is already occupied." << endl;
        }
    }

    void removeVehicle()
    {
        if (!available)
        {
            vehicleNumber = "";
            available = true;

            cout << "Vehicle removed from Slot "
                 << slotNumber << endl;
        }
    }

    void displaySlot()
    {
        cout << "\nSlot Number : " << slotNumber << endl;

        if (available)
        {
            cout << "Status      : Available" << endl;
        }
        else
        {
            cout << "Status      : Occupied" << endl;
            cout << "Vehicle No. : " << vehicleNumber << endl;
        }
    }

    int getSlotNumber()
    {
        return slotNumber;
    }
};


class ParkingManagement
{
private:

    vector<ParkingSlot> slots;

public:

    ParkingManagement(int totalSlots)
    {
        for (int i = 1; i <= totalSlots; i++)
        {
            slots.push_back(ParkingSlot(i));
        }
    }

    int assignSlot(string vehicleNumber)
    {
        for (int i = 0; i < slots.size(); i++)
        {
            if (slots[i].isAvailable())
            {
                slots[i].parkVehicle(vehicleNumber);

                return slots[i].getSlotNumber();
            }
        }

        return -1;
    }

    void displayAllSlots()
    {
        
        cout << "          PARKING SLOT STATUS" << endl;
        

        for (int i = 0; i < slots.size(); i++)
        {
            slots[i].displaySlot();
        }
    }
};


// OCR CLASS


class PlateOCR
{
private:

    tesseract::TessBaseAPI ocr;

public:

    bool initialize()
    {
        if (ocr.Init(NULL, "eng", tesseract::OEM_LSTM_ONLY))
        {
            cout << "Tesseract initialization failed." << endl;
            return false;
        }

        ocr.SetPageSegMode(tesseract::PSM_SINGLE_LINE);

        return true;
    }


    string readNumberPlate(Mat plateImage)
    {
        Mat gray;
        Mat thresholdImage;

        // Convert to grayscale
        cvtColor(plateImage, gray, COLOR_BGR2GRAY);

        // Remove noise
        GaussianBlur(
            gray,
            gray,
            Size(3, 3),
            0
        );

        // Convert image to black and white
        threshold(
            gray,
            thresholdImage,
            0,
            255,
            THRESH_BINARY + THRESH_OTSU
        );

        // Give processed image to OCR
        ocr.SetImage(
            thresholdImage.data,
            thresholdImage.cols,
            thresholdImage.rows,
            1,
            thresholdImage.step
        );

        string result = ocr.GetUTF8Text();

        return result;
    }


    ~PlateOCR()
    {
        ocr.End();
    }
};


// LICENSE PLATE DETECTOR


class PlateDetector
{
public:

    Mat detectPlate(Mat image)
    {
        Mat gray;
        Mat edgeImage;

        // Convert image to grayscale
        cvtColor(
            image,
            gray,
            COLOR_BGR2GRAY
        );

        // Reduce noise
        GaussianBlur(
            gray,
            gray,
            Size(5, 5),
            0
        );

        // Detect edges
        Canny(
            gray,
            edgeImage,
            100,
            200
        );

        vector<vector<Point>> contours;

        findContours(
            edgeImage,
            contours,
            RETR_LIST,
            CHAIN_APPROX_SIMPLE
        );

        double bestArea = 0;
        Rect bestRectangle;

        for (int i = 0; i < contours.size(); i++)
        {
            Rect rectangle = boundingRect(contours[i]);

            double width = rectangle.width;
            double height = rectangle.height;

            if (height == 0)
                continue;

            double ratio = width / height;

            double area = width * height;

            // Typical license plates are wider than tall
            if (
                ratio > 2.0 &&
                ratio < 6.0 &&
                area > bestArea &&
                area > 1000
            )
            {
                bestArea = area;
                bestRectangle = rectangle;
            }
        }

        if (bestArea == 0)
        {
            return Mat();
        }

        // Draw detected plate
        rectangle(
            image,
            bestRectangle,
            Scalar(0, 255, 0),
            2
        );

        // Return plate image
        return image(bestRectangle).clone();
    }
};


// CLEAN OCR RESULT


string cleanPlateNumber(string text)
{
    string result = "";

    for (char c : text)
    {
        if (
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9')
        )
        {
            result += toupper(c);
        }
    }

    return result;
}


// MAIN


int main()
{

    cout << "       SMART PARKING SYSTEM" << endl;


    cout << "\nStarting camera..." << endl;

    // OPEN CAMERA
  

    VideoCapture camera(0);

    if (!camera.isOpened())
    {
        cout << "Camera could not be opened." << endl;
        return 1;
    }

    cout << "Camera started successfully." << endl;

    cout << "\nPress SPACE to capture vehicle image." << endl;
    cout << "Press ESC to exit." << endl;


    Mat frame;
    Mat capturedImage;


    // CAPTURE IMAGE


    while (true)
    {
        camera >> frame;

        if (frame.empty())
        {
            cout << "Could not capture image." << endl;
            break;
        }

        imshow(
            "Smart Parking Camera",
            frame
        );

        char key = waitKey(30);

        if (key == 27)
        {
            cout << "\nProgram stopped." << endl;

            camera.release();
            destroyAllWindows();

            return 0;
        }

        if (key == ' ')
        {
            capturedImage = frame.clone();

            cout << "\nImage captured successfully." << endl;

            break;
        }
    }


    camera.release();
    destroyAllWindows();


    // OPENCV
  


    cout << "        OPENCV PROCESSING" << endl;
   

    cout << "Processing captured image..." << endl;


    PlateDetector detector;

    Mat plateImage =
        detector.detectPlate(capturedImage);


    if (plateImage.empty())
    {
        cout << "\nLicense plate could not be detected." << endl;

        return 1;
    }


    cout << "License plate detected successfully." << endl;


    imshow(
        "Detected License Plate",
        plateImage
    );

    waitKey(1000);
    destroyAllWindows();



    // OCR


    cout << "  OCR" << endl;


    cout << "Reading license plate..." << endl;


    PlateOCR ocr;


    if (!ocr.initialize())
    {
        return 1;
    }


    string ocrResult =
        ocr.readNumberPlate(plateImage);


    string vehicleNumber =
        cleanPlateNumber(ocrResult);


    cout << "\nOCR Result: "
         << vehicleNumber << endl;


    if (vehicleNumber.empty())
    {
        cout << "\nCould not read vehicle number." << endl;

        return 1;
    }


    // VEHICLE OBJECT


    cout << "        VEHICLE IDENTIFICATION" << endl;



    cout << "Vehicle Number: "
         << vehicleNumber << endl;


    /*
        At this stage OCR has automatically provided
        the vehicle number.

        Vehicle type recognition requires another
        computer-vision/ML model.

        For this prototype we create a FourWheeler
        object after successful plate recognition.
    */


    Vehicle* vehicle =
        new Car(vehicleNumber);


    vehicle->displayVehicleType();

    // PARKING MANAGEMENT

    cout << "        PARKING MANAGEMENT" << endl;



    ParkingManagement parking(10);


    cout << "\nFinding available parking slot..." << endl;


    int assignedSlot =
        parking.assignSlot(
            vehicle->getVehicleNumber()
        );


    if (assignedSlot == -1)
    {
        cout << "\nNo parking slot available." << endl;
    }
    else
    {
        cout << "\n" << endl;

        cout << "Parking Successful!" << endl;

        cout << "Vehicle Number : "
             << vehicle->getVehicleNumber()
             << endl;

        cout << "Assigned Slot  : "
             << assignedSlot
             << endl;

    }


    // --------------------------------------------------------
    // DISPLAY PARKING
    // --------------------------------------------------------

    parking.displayAllSlots();


    // --------------------------------------------------------
    // CLEAN UP
    // --------------------------------------------------------

    delete vehicle;


    cout << "\n========================================" << endl;
    cout << "       PARKING PROCESS COMPLETED" << endl;
    cout << "========================================" << endl;


    return 0;
}