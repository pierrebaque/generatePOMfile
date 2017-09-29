#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace cv;

int min(int a, int b) {
    if(a < b) return(a);
    return(b);
}

int max(int a, int b) {
    if(a > b) return(a);
    return(b);
}

int main(int, char** argv)
{
    // Load intrinsic calibration
    cv::FileStorage fs(argv[3], cv::FileStorage::READ);
    Mat cameraMatrix, distortionCoefficients;
    fs["camera_matrix"] >>cameraMatrix;
    fs["distortion_coefficients"] >> distortionCoefficients;

    // Load extrinsic  calibration
    cv::FileStorage fs_ext(argv[2], cv::FileStorage::READ);
    cv::Vec3d rvec, tvec;
    fs_ext["rvec"] >>rvec;
    fs_ext["tvec"] >> tvec;
    // I divide by 100 to take into account units difference
    tvec = tvec /100;

    // Define the ground
    std::vector<cv::Point3f> world;
    std::vector<cv::Point3f> world_right;
    std::vector<cv::Point3f> world_behind;
    std::vector<cv::Point3f> world_up;

    float height = 18;
    float width = 8;
    int nb_height = 10;
    int nb_width = 10;
    float man_ray = 0.3;
    float man_height = 1.8;
    int reduction = 1;
    float origineX = 0.0;
    float origineY = 0.0;

    string constants_filename = argv[6];
    std::ifstream inFile(constants_filename.c_str());
    std::string line;
    int nb_points = 0;
    while(getline(inFile, line))
    {
        istringstream iss(line);
        vector<string> words;
        do
        {
            string sub;
            iss >> sub;
            words.push_back(sub);
        } while (iss);
        if(!words[0].compare("HEIGHT")) height = stof(words[1]);
        if(!words[0].compare("WIDTH")) width = stof(words[1]);
        if(!words[0].compare("NB_HEIGHT")) nb_height = stoi(words[1]);
        if(!words[0].compare("NB_WIDTH")) nb_width = stoi(words[1]);
        if(!words[0].compare("MAN_RAY")) man_ray = stof(words[1]);
        if(!words[0].compare("MAN_HEIGHT")) man_height = stof(words[1]);
        if(!words[0].compare("REDUCTION")) reduction = stoi(words[1]);
        if(!words[0].compare("ORIGINE_X")) origineX = stof(words[1]);
        if(!words[0].compare("ORIGINE_Y")) origineY = stof(words[1]);
    }

    inFile.close();

    for(int i = 0; i < nb_height; i++) {
        for(int j = 0; j < nb_width; j++) {
            world.push_back(cv::Point3f(origineX + (float) j * width / (float) nb_width, origineY + (float) i * height / (float) nb_height, 0.));
            world_right.push_back(cv::Point3f((float) j * width / (float) nb_width + man_ray, (float) i * height / (float) nb_height, 0.));
            world_behind.push_back(cv::Point3f((float) j * width / (float) nb_width, (float) i * height / (float) nb_height + man_ray, 0.));
            world_up.push_back(cv::Point3f((float) j * width / (float) nb_width, (float) i * height / (float) nb_height, man_height));
        }
    }

    // Define the points, 8 by position
    std::vector<cv::Point3f> points;
    for(int i = 0; i < (int) world.size(); i++) {
        float x = world[i].x;
        float y = world[i].y;
        float z = world[i].z;
        points.push_back(cv::Point3f(x + man_ray, y + man_ray, z));
        points.push_back(cv::Point3f(x + man_ray, y - man_ray, z));
        points.push_back(cv::Point3f(x - man_ray, y + man_ray, z));
        points.push_back(cv::Point3f(x - man_ray, y - man_ray, z));
        points.push_back(cv::Point3f(x + man_ray, y + man_ray, z + man_height));
        points.push_back(cv::Point3f(x + man_ray, y - man_ray, z + man_height));
        points.push_back(cv::Point3f(x - man_ray, y + man_ray, z + man_height));
        points.push_back(cv::Point3f(x - man_ray, y - man_ray, z + man_height));
    }

    //Open the image
	Mat img = imread(argv[1]);

    // Apply the projections
    std::vector<cv::Point2f> projections;
    std::vector<cv::Point2f> right;
    std::vector<cv::Point2f> behind;
    std::vector<cv::Point2f> up;
    projectPoints(world, rvec, tvec, cameraMatrix, distortionCoefficients, projections);
    projectPoints(world_right, rvec, tvec, cameraMatrix, distortionCoefficients, right);
    projectPoints(world_behind, rvec, tvec, cameraMatrix, distortionCoefficients, behind);
    projectPoints(world_up, rvec, tvec, cameraMatrix, distortionCoefficients, up);
    std::vector<cv::Point2f> points_projections;
    projectPoints(points, rvec, tvec, cameraMatrix, distortionCoefficients, points_projections);

    // Resize the projections
    float resize_factor = 1.0;
    for(int i = 0; i < (int) projections.size(); i++){
        projections[i].x = projections[i].x/resize_factor;
        projections[i].y = projections[i].y/resize_factor;
        right[i].x = right[i].x/resize_factor;
        right[i].y = right[i].y/resize_factor;
        behind[i].x = behind[i].x/resize_factor;
        behind[i].y = behind[i].y/resize_factor;
        up[i].x = up[i].x/resize_factor;
        up[i].y = up[i].y/resize_factor;
        points_projections[i].x = points_projections[i].x/resize_factor;
        points_projections[i].y = points_projections[i].y/resize_factor;
    }

    // Check the position of people on camera
    /*
    for(int i = 0; i < (int) projections.size(); i++) {
        arrowedLine(img, projections[i], right[i], cv::Scalar(255, 0, 0), 2);
        arrowedLine(img, projections[i], behind[i], cv::Scalar(255, 0, 0), 2);
        arrowedLine(img, projections[i], up[i], cv::Scalar(255, 0, 0), 2);
    }
    */

    // Compute the rectangles
    std::vector<std::vector<int> > rectangles((int) world.size(), vector<int>(4));
    for(int i = 0; i < (int) world.size(); i++) {
        int xMin, yMin, xMax, yMax;
        xMin = (int) points_projections[8 * i].x;
        yMin = (int) points_projections[8 * i].y;
        xMax = (int) points_projections[8 * i].x;
        yMax = (int) points_projections[8 * i].y;
        std::vector<pair<int, int> > vec(8);
        for(int j = 0; j < 8; j++) {
            int x, y;
            x = (int) points_projections[8 * i + j].x;
            y = (int) points_projections[8 * i + j].y;
            xMin = min(xMin, x);
            yMin = min(yMin, y);
            xMax = max(xMax, x);
            yMax = max(yMax, y);
            vec[j] = make_pair(y, x);
        }
        sort(vec.begin(), vec.end());
        int xMid = 0, yMid = 0;
        for (int j = 4; j < 8; j++) {
            xMid += vec[j].second;
            yMid += vec[j].first;
        }
        xMid /= 4;
        yMid /= 4;
        rectangles[i][0] = xMin;
        rectangles[i][1] = yMin;
        rectangles[i][2] = xMax;
        rectangles[i][3] = yMid;
        
        //Test
        if (yMid - yMin < xMax - xMin) {
            //std::cerr << " Error size" << std::endl;
            for(int j = 0; j < 8; j++) {
                int x, y;
                x = (int) points_projections[8 * i + j].x;
                y = (int) points_projections[8 * i + j].y;
                //std::cerr << "x"<< x << " y " << y << std::endl;
            }
        }
    }
   //std::cerr << " End mimax" << std::endl;
    // Check the position of rectangles
    for(int i = 0; i < (int) rectangles.size(); i++) {
        bool visible = true;
        if(rectangles[i][0]  >= img.cols - 2 || rectangles[i][1]  >= img.rows - 2 || rectangles[i][2]  <= 1 || rectangles[i][3]  <= 1) visible = false;
        if(rectangles[i][3] - rectangles[i][1] < rectangles[i][2] - rectangles[i][0]) visible = false; //Vertical bug
        if(rectangles[i][2] - rectangles[i][0] > img.cols / 3) visible = false;
        if (visible) {
            // Crop the rectangles when too big Actually, don't do it.
//            if(rectangles[i][0] < 0 ) rectangles[i][0] = 0;
//            if(rectangles[i][1] < 0 ) rectangles[i][1] = 0;
//            if(rectangles[i][2] > img.cols - 1 ) rectangles[i][2] = img.cols - 1;
//            if(rectangles[i][3] > img.rows - 1 ) rectangles[i][3] = img.rows - 1;
            
            
            float xMin, yMin, xMax, yMax;
            xMin = (float) rectangles[i][0];
            yMin = (float) rectangles[i][1];
            xMax = (float) rectangles[i][2];
            yMax = (float) rectangles[i][3];
            cv::Point2f p1 = cv::Point2f(xMin, yMin);
            cv::Point2f p2 = cv::Point2f(xMin, yMax);
            cv::Point2f p3 = cv::Point2f(xMax, yMax);
            cv::Point2f p4 = cv::Point2f(xMax, yMin);
            arrowedLine(img, p1, p2, cv::Scalar(0, 255, 0));
            arrowedLine(img, p2, p3, cv::Scalar(0, 255, 0));
            arrowedLine(img, p3, p4, cv::Scalar(0, 255, 0));
            arrowedLine(img, p4, p1, cv::Scalar(0, 255, 0));
        }
    }

    std::string camera_number = argv[4];

    for(int i = 0; i < (int) rectangles.size(); i++) {
        std::cout << "RECTANGLE " << camera_number << " " << i;
        bool visible = true;
        if(rectangles[i][0]  >= img.cols - 2 || rectangles[i][1]  >= img.rows - 2 || rectangles[i][2]  <= 1 || rectangles[i][3]  <= 1) visible = false;
        if(rectangles[i][3] - rectangles[i][1] < rectangles[i][2] - rectangles[i][0]) visible = false;
        if(rectangles[i][2] - rectangles[i][0] > img.cols / 3) visible = false;

        if(visible) {
            for(int j = 0; j < 4; j++) {
                std::cout << " " << (int) (rectangles[i][j] / reduction);
            }
            std::cout << std::endl;
        }
        else {
            std::cout << " notvisible" << std::endl;
        }
    }



    if(strcmp(argv[5], "0")) {

        //namedWindow("My Window",  0);
		//resizeWindow("My Window",img.size().width,img.size().height);

		//show the image
		//imshow("My Window", img);
        imwrite("./images/c" + string(argv[4]) + ".jpg", img);

      // Wait until user press some key
		//waitKey(0);

    }


    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
