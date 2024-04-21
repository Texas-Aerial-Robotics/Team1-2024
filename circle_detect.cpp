/*
 * WIP
 * compile command: g++ circle_detect.cpp -o circle_detect $(pkg-config --cflags --libs opencv4) -std=c++11
 *  
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // Open the default camera
    VideoCapture cap(0);

    // Check if camera opened successfully
    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while (true) {
        Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        // Convert to HSV color space
        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // Define the range of green color in HSV
        Scalar lower_green(25, 80, 80);
        Scalar upper_green(50, 255, 255);

        // Threshold the HSV image to get only green colors
        Mat mask;
        inRange(hsv, lower_green, upper_green, mask);

        // Apply Gaussian blur to reduce noise
        GaussianBlur(mask, mask, Size(9, 9), 2);

        // Use Hough Circle Transform to detect circles
        vector<Vec3f> circles;
        HoughCircles(mask, circles, HOUGH_GRADIENT, 1, 300, 50, 30, 0, 0);

        // If circles are detected, draw them
        for (size_t i = 0; i < circles.size(); i++) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // Draw the outer circle
            circle(frame, center, radius, Scalar(0, 255, 0), 2);
            // Draw the center of the circle
            circle(frame, center, 2, Scalar(0, 0, 255), 3);
        }

        // Display the resulting frame
        imshow("Frame", frame);
        imshow("Mask", mask);

        // Press Esc to exit
        if (waitKey(1) == 27)
            break;
    }

    // When everything done, release the video capture object
    cap.release();
    // Closes all the frames
    destroyAllWindows();

    return 0;
}