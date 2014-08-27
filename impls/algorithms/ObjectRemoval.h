#ifndef OBJECTREMOVAL_H
#define OBJECTREMOVAL_H

#include <istream>
#include <string>
#include <ostream>
#include "ImageConvert.h"
using namespace cv;

namespace algorithms {
    cv::Vec3b BGR2HSV(Vec3b HSVColor);
    cv::Vec3b HSV2BGR(Vec3b HSVColor);

    class ObjectRemoval{
        public:

            static const int LOW_HUE = 75;
            static const int HIGH_HUE = 130;

            static const int LOW_SAT =  100;
            static const int HIGH_SAT = 255;

            static const int LOW_VAL = 100;
            static const int HIGH_VAL = 255;

            static const long int THRESHOLD = 7500;

            void removeObject(cv::Mat src);
            cv::Mat identifyMask(cv::Mat img);
            void detectColorUsingTrackBar(std::string filename);
        private:
            /*!
             * \brief changeColor
             * \param image
             * \param mask
             * \param r
             * \param c
             * \param widthRec
             * \param heightRec
             */
            void changeColor(Mat image, Mat mask, int r, int c);
            void transparentObject(int minRow, int minCol,
                             int maxRow, int maxCol, Mat img, Mat mask);
            bool isOnBoundery(Mat mask, int r, int c);
            void Spiral( int minX, int minY, int maxX, int maxY, Mat img, Mat mask);
    };

}

#endif // OBJECTREMOVAL_H
