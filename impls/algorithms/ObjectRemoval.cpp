#include "ObjectRemoval.h"

using namespace cv;
using namespace std;
using namespace algorithms;

/*!
 * \brief ObjectRemoval::changeColor
 * \param image
 * \param mask: array bit 0 and 1. The selected object now is in white color, the others is black
 * \param r
 * \param c
 * \param widthRec
 * \param heightRec
 */

void ObjectRemoval::changeColor(Mat image, Mat mask, int r, int c){
    int color = mask.at<uchar>(r,c) ; //color at mask(r,c): black or white
    Vec3b replacingColor;
    int offset, lowerRange, upperRange;

    if(color == 255){ //pixel at (r,c) is white
        offset = 5;
        int brushSize = offset;
        replacingColor = image.at<Vec3b>(r-(offset), c);
        lowerRange = r-brushSize < 0 ? 0 : r-brushSize;
        upperRange = r+brushSize > mask.rows ? mask.rows : r+brushSize;

        for (int i = lowerRange; i< upperRange; ++i)
            image.at<Vec3b>(i,c) = replacingColor;
        mask.at<uchar>(r,c) = 0;
    }
}

void ObjectRemoval::transparentObject(int minRow, int minCol,
                                int maxRow, int maxCol, Mat img, Mat mask){
    for (int r = minRow; r< maxRow; ++r)
        for (int c = minCol; c < maxCol; ++c )
            changeColor(img,mask,r,c);
}



Mat ObjectRemoval::identifyMask(Mat img){
    cv::Mat src, hsvSrc, mask;

    src = img;

    //convert image from BGR to HSV
    cv::cvtColor(src,hsvSrc,cv::COLOR_BGR2HSV);

    //identify a mask of the rectangle based on the color range
    cv::inRange(hsvSrc,cv::Scalar(LOW_HUE,LOW_SAT,LOW_VAL),cv::Scalar(LOW_HUE,HIGH_SAT,HIGH_VAL),mask);

    // start scaning through the source image to detect the rectangle object based on
    //the color range (range of blue) and an accepted threshold.
    bool start = false;
    long int prev = 0;
    long int current = 0;

    for(int i = LOW_HUE; i < HIGH_HUE; i++){
            //cout<<"current - prev  = "<< current - prev <<endl;
        if (current - prev > THRESHOLD){
            start = true;
        }
        if (start && (current - prev < THRESHOLD*0.5)){
            //cout<<"Break when Hue = "<< i-1 <<endl;

            cv::inRange(hsvSrc,cv::Scalar(LOW_HUE,LOW_SAT,LOW_VAL),
                        cv::Scalar(i-1,HIGH_SAT,HIGH_VAL),mask);
            break;
        }
        cv::inRange(hsvSrc,cv::Scalar(LOW_HUE,LOW_SAT,LOW_VAL),
                    cv::Scalar(i,HIGH_SAT,HIGH_VAL),mask);
        prev = current;
        current = countNonZero(mask);
        //cout<<"Hue = "<< i <<endl;

    } //exit for

    //cout<<"after finish for loop: current - prev  = "<< current - prev <<endl;
    //cv::inRange(hsvSrc,cv::Scalar(lowH,lowS,lowV),cv::Scalar(highH,highS,highV),mask);

    cv::Size structureSize = cv::Size(20, 20);
    //morphological opening (remove small objects from the foreground)
    cv::erode(mask, mask,
              cv:: getStructuringElement(cv::MORPH_ELLIPSE, structureSize) );
    cv::dilate( mask, mask,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, structureSize) );

    //morphological closing (fill small holes in the foreground)
    cv::dilate( mask, mask,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, structureSize) );
    cv::erode(mask, mask,
              cv::getStructuringElement(cv::MORPH_ELLIPSE, structureSize) );

    imshow("mask", mask);
    return mask;
}

/*!
 * \brief ObjectRemoval::removeObject
 * \param img
 */
void ObjectRemoval::removeObject(Mat src){

    Mat mask = identifyMask(src);
    imshow("mask",mask);

    /*identify four point of rectangle*/
    int minR = mask.rows , minC = mask.cols, maxR =0 , maxC =0;
    for(int i = 0 ; i < mask.rows; ++i)
        for(int j = 0; j < mask.cols; ++j){
            if(mask.at<uchar>(i,j) == 255){
                if (i < minR) minR = i;
                if (i > maxR) maxR = i;
                if (j < minC) minC = j;
                if (j > maxC) maxC = j;
            }
        }
    // transparent the rectangle into the background's color
   transparentObject(minR, minC, maxR, maxC, src, mask);
}


/* Code reserved
 *
bool ObjectRemoval::isOnBoundery(Mat mask, int r, int c){
    int color = mask.at<uchar>(r,c) ;
    if(r == 0 || c ==0 || r == mask.rows -1 || c == mask.cols -1) return color == 255;
    int colorLeft = mask.at<uchar>(r,c -1) ;
    int colorRight = mask.at<uchar>(r,c +1) ;
    int colorAbove = mask.at<uchar>(r -1,c) ;
    int colorBelow = mask.at<uchar>(r +1,c) ;

    return (color == 255) && ((colorLeft == 0) || colorRight == 0 ||
                              colorAbove == 0 || colorBelow ==0);

}

void ObjectRemoval::Spiral( int minX, int minY, int maxX, int maxY, Mat img, Mat mask){
    int minR, minC, maxR, maxC, i, j, width, height;
    width = maxY - minY + 1;
    height = maxX - minX + 1;

    minR = minX;
    minC = minY;
    maxR = maxX;
    maxC = maxY;
    i= minX;
    j= minY;


    for( int id =0; id< width*height; id++){
        //modify color
      //  changeColor(img,mask,i,j,width,height);
        cout<< "(" << i << ","<< j <<")"<<endl;

        if((i == minR) &&(j < maxC)) j++;
        else if((i < maxR) &&(j == maxC)) i++;
        else if((i == maxR) &&(j > minC)) j--;
        else if((i > minR+1) &&(j == minC)) i--;

        if ((i == minR+1) && (j == minC)) {
            ++minR;
            cout<<"MinR ++ \r\n";
            --maxC;
            cout<<"MaxC -- \r\n";
            --maxR;
            cout<<"MaxR -- \r\n";
            ++minC;
            cout<<"MinC ++ \r\n";
        }
    }
}


void ObjectRemoval:: detectColorUsingTrackBar(string filename){

    cv::namedWindow("Control", CV_WINDOW_AUTOSIZE);

    int iLowH = 0;//75;
    int iHighH = 360;// 130;

    int iLowS = 0;
    int iHighS = 255;

    int iLowV = 0;
    int iHighV = 255;

    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 360); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    while (true)
    {
        if (cv::waitKey(30) == 32)
        {
            cv::Mat imgOriginal;
            imgOriginal= cv::imread(filename, CV_LOAD_IMAGE_COLOR);

            if (!imgOriginal.data) //if not success, break loop
            {
                std::cout << "Cannot read image" << endl;
                break;
            }
            cv::Mat imgHSV, background_mask, object_mask,outImg;

            //Convert the captured frame from BGR to HSV
            cv::cvtColor(imgOriginal, imgHSV,cv:: COLOR_BGR2HSV);
            //Threshold the image
            cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), background_mask);

            //morphological opening (remove small objects from the foreground)
            cv::erode(background_mask, background_mask, cv:: getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)) );
            cv::dilate( background_mask, background_mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)) );

            //morphological closing (fill small holes in the foreground)
            cv::dilate( background_mask, background_mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)) );
            cv::erode(background_mask, background_mask, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20, 20)) );

            cv::bitwise_not(background_mask,object_mask);
            cv::bitwise_and(imgOriginal,imgOriginal,outImg,background_mask);

            cv::imshow("Original", outImg);

            int numOfNoneZero1 = cv::countNonZero( background_mask);
            std:: cout << "" << numOfNoneZero1 << std::endl;

        }
        if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            std::cout << "esc key is pressed by user" << std::endl;
            break;
        }
    }
}

Vec3b algorithms::BGR2HSV(Vec3b BGRColor){
    const float FLOAT_TO_BYTE = 255.0f;
    const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;
    float fR, fG, fB;
    float fH, fS, fV;
    int bR, bG, bB;
    bB = BGRColor[0];
    bG = BGRColor[1];
    bR = BGRColor[2];

    // Convert from 8-bit integers to floats.
    fR = bR * BYTE_TO_FLOAT;
    fG = bG * BYTE_TO_FLOAT;
    fB = bB * BYTE_TO_FLOAT;

    // Convert from RGB to HSV, using float ranges 0.0 to 1.0.
    float fDelta;
    float fMin, fMax;
    int iMax;
    // Get the min and max, but use integer comparisons for slight speedup.
    if (bB < bG) {
        if (bB < bR) {
            fMin = fB;
            if (bR > bG) {
                iMax = bR;
                fMax = fR;
            }
            else {
                iMax = bG;
                fMax = fG;
            }
        }
        else {
            fMin = fR;
            fMax = fG;
            iMax = bG;
        }
    }
    else {
        if (bG < bR) {
            fMin = fG;
            if (bB > bR) {
                fMax = fB;
                iMax = bB;
            }
            else {
                fMax = fR;
                iMax = bR;
            }
        }
        else {
            fMin = fR;
            fMax = fB;
            iMax = bB;
        }
    }
    fDelta = fMax - fMin;
    fV = fMax;				// Value (Brightness).
    if (iMax != 0) {			// Make sure it's not pure black.
        fS = fDelta / fMax;		// Saturation.
        float ANGLE_TO_UNIT = 1.0f / (6.0f * fDelta);	// Make the Hues between 0.0 to 1.0 instead of 6.0
        if (iMax == bR) {		// between yellow and magenta.
            fH = (fG - fB) * ANGLE_TO_UNIT;
        }
        else if (iMax == bG) {		// between cyan and yellow.
            fH = (2.0f/6.0f) + ( fB - fR ) * ANGLE_TO_UNIT;
        }
        else {				// between magenta and cyan.
            fH = (4.0f/6.0f) + ( fR - fG ) * ANGLE_TO_UNIT;
        }
        // Wrap outlier Hues around the circle.
        if (fH < 0.0f)
            fH += 1.0f;
        if (fH >= 1.0f)
            fH -= 1.0f;
    }
    else {
        // color is pure Black.
        fS = 0;
        fH = 0;	// undefined hue
    }

    // Convert from floats to 8-bit integers.
    int bH = (int)(0.5f + fH * 255.0f);
    int bS = (int)(0.5f + fS * 255.0f);
    int bV = (int)(0.5f + fV * 255.0f);

    // Clip the values to make sure it fits within the 8bits.
    if (bH > 255)
        bH = 255;
    if (bH < 0)
        bH = 0;
    if (bS > 255)
        bS = 255;
    if (bS < 0)
        bS = 0;
    if (bV > 255)
        bV = 255;
    if (bV < 0)
        bV = 0;

    return Vec3b(bH,bS,bV);
}

Vec3b algorithms::HSV2BGR(Vec3b HSVColor){
    const float FLOAT_TO_BYTE = 255.0f;
    const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;
    float fR, fG, fB;
    float fH, fS, fV;
    int bH, bS, bV;
    bH = HSVColor[0];
    bS = HSVColor[1];
    bV = HSVColor[2];
    // Convert from 8-bit integers to floats
    fH = (float)bH * BYTE_TO_FLOAT;
    fS = (float)bS * BYTE_TO_FLOAT;
    fV = (float)bV * BYTE_TO_FLOAT;

    // Convert from HSV to RGB, using float ranges 0.0 to 1.0
    int iI;
    float fI, fF, p, q, t;

    if( bS == 0 ) {
        // achromatic (grey)
        fR = fG = fB = fV;
    }
    else {
        // If Hue == 1.0, then wrap it around the circle to 0.0
        if (fH >= 1.0f)
            fH = 0.0f;

        fH *= 6.0;			// sector 0 to 5
        fI = floor( fH );		// integer part of h (0,1,2,3,4,5 or 6)
        iI = (int) fH;			//		"		"		"		"
        fF = fH - fI;			// factorial part of h (0 to 1)

        p = fV * ( 1.0f - fS );
        q = fV * ( 1.0f - fS * fF );
        t = fV * ( 1.0f - fS * ( 1.0f - fF ) );

        switch( iI ) {
        case 0:
            fR = fV;
            fG = t;
            fB = p;
            break;
        case 1:
            fR = q;
            fG = fV;
            fB = p;
            break;
        case 2:
            fR = p;
            fG = fV;
            fB = t;
            break;
        case 3:
            fR = p;
            fG = q;
            fB = fV;
            break;
        case 4:
            fR = t;
            fG = p;
            fB = fV;
            break;
        default:		// case 5 (or 6):
            fR = fV;
            fG = p;
            fB = q;
            break;
        }
    }

    // Convert from floats to 8-bit integers
    int bR = (int)(fR * FLOAT_TO_BYTE);
    int bG = (int)(fG * FLOAT_TO_BYTE);
    int bB = (int)(fB * FLOAT_TO_BYTE);

    // Clip the values to make sure it fits within the 8bits.
    if (bR > 255)
        bR = 255;
    if (bR < 0)
        bR = 0;
    if (bG > 255)
        bG = 255;
    if (bG < 0)
        bG = 0;
    if (bB > 255)
        bB = 255;
    if (bB < 0)
        bB = 0;

    return Vec3b(bB,bG,bR);
}

void ObjectRemoval::transparentObject(int minRow, int minCol,
                                int maxRow, int maxCol, Mat img, Mat mask){
    int color, offset = 5, lowR, highR;
    Vec3b replacingColor;
    for (int r = minRow; r< maxRow; ++r)
        for (int c = minCol; c < maxCol; ++c ){
            color = mask.at<uchar>(r,c);
            lowR = r - offset;
            highR = r + offset;
            if(color == 255){
                if ((minRow == 0) && (minCol ==0)){
                    if ( maxCol == mask.cols)
                    {
                        replacingColor = img.at<Vec3b>(maxRow + (offset),minCol);
                        lowR = r;
                        highR = r + maxRow + offset;
                    }
                    replacingColor = img.at<Vec3b>(minRow, maxCol + (offset));
                    lowR = c;
                    highR = c + maxCol + offset;
                }

                if ((maxRow == mask.rows) && (maxCol == mask.cols)){
                    if (minCol ==0)
                    {
                        replacingColor = img.at<Vec3b>(minRow - (offset), minCol);
                        lowR = r - offset;
                        highR = r + offset;
                    }
                    replacingColor = img.at<Vec3b>(minRow,minCol - (offset));
                    lowR = c - offset;
                    highR = c + offset;
                }

                for (int i = lowR; i< highR; ++i)
                    img.at<Vec3b>(i,c) = replacingColor;
                mask.at<uchar>(r,c) = 0;
            }

        }
}*/

