/*
 Image processing for morphometrics (IPM)
 Copyright (C) 2014  Nguyen Hoang Thao (hoangthao.ng@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/.
 */
#define QT_NO_PRINTER
#ifndef IMAGEVIEWER_H
 #define IMAGEVIEWER_H

 #include <QMainWindow>

#include <opencv2/opencv.hpp>

 class QAction;
 class QLabel;
 class QMenu;
 class QScrollArea;
 class QScrollBar;
 class QCloseEvent;
 class QContextMenuEvent;
 class QToolBar;
 class QDialog;

 class ImageViewer : public QMainWindow
 {
     Q_OBJECT

 public:
     ImageViewer();
     QString fileName;
     cv::Mat matImage;
     QImage qImage;
     void loadImage(QString fn);
     void loadImage(cv::Mat _matImage,QImage _qImage, QString tt);

     void addParameterPanel(QDialog *pnl, int x, int y);

     QAction *parameterAct;
     QDialog *parameterPanel;

     void activeFunction();
     void normalBlurAction(int kernelSize);
     void gaussianBlurAction(int kernelSize);
     void medianBlurAction(int kernelSize);
     void bilateralFilterAction(int sigmaColor);
     void convolutionFilterAction(int kernelSize);

     void erosionAction(int kernelSize, int typeEle);
	void dilationAction(int kernelSize, int typeEle);
	void morphoExtAction(int kernelSize, int type);

	void thresholdAction(int value, int type);
	void sobelAction(int kernelSize, int scale);
	void scharrAction(int scale);
	void laplaceAction(int kernelSize, int scale);
	void cannyAction(int kernelSize, int low, int ratio);

	void featureSurfAction(int minHessian, bool refresh=true);

 protected:
     void closeEvent(QCloseEvent *event);
     void contextMenuEvent(QContextMenuEvent *event);

 private slots:
     void open();
     void print();
     bool save();
     bool saveAs();
     void zoomIn();
     void zoomOut();
     void normalSize();
     void fitToWindow();
     void about();

     void parameterAction();
     void normalBlurAction();
     void gaussianBlurAction();
     void medianBlurAction();
     void bilateralFilterAction();
     void convolutionFilterAction();

     void erosionAction();
     void dilationAction();
     void morphoExtAction();

     void thresholdAction();
     void sobelAction();
     void scharrAction();
     void laplaceAction();
     void cannyAction();

     void houghLineAction();
     void houghCircleAction();
     void findContourAction();
     void convexHullAction();
     void bdBoxCircleAction();
     void bdRotEclipseAction();
     void imgMomentsAction();

     void harrisAction();
	 void shiTomasiAction();
	 void cornerDetectAction();

	 void featureSurfAction();
	 void featureSiftAction();
     void featureSiftActionWithMask();
	 void matchingBFAction();
	 void matchingFLANNAction();
	 void matchingHomographyAction();
	 void matchingTemplateAction();
	 void correctMorphAction();

	 void pyramidAction();
	 void remapAction();
	 void affineAction();
	 void histogramAction();
	 void equalizationAction();
	 void backprojectionAction();

	 void grayscaleMyImplAction();
	 void binaryMyImplAction();
	 void robertsMyImplAction();
	 void sobelMyImplAction();
	 void hitmissMyImplAction();
	 void otsuMyImplAction();
	 void growingMyImplAction();
	 void antMyImplAction();

     void objectRemovingAction();

 private:
     void createActions();
     void createMenus();
     void updateActions();
     void scaleImage(double factor);
     void adjustScrollBar(QScrollBar *scrollBar, double factor);
     void createToolBars();
     void createStatusBar();
     void readSettings();
     void writeSettings();

     QLabel *imageLabel;
     QScrollArea *scrollArea;
     double scaleFactor;

 #ifndef QT_NO_PRINTER
     QPrinter printer;
 #endif

     QAction *openAct;
     QAction *printAct;
     QAction *saveAct;
	 QAction *saveAsAct;
	 QAction *closeAct;
     QAction *exitAct;
     QAction *zoomInAct;
     QAction *zoomOutAct;
     QAction *normalSizeAct;
     QAction *fitToWindowAct;
     QAction *aboutAct;

     QAction *normalBlurAct;
     QAction *gaussianBlurAct;
     QAction *medianBlurAct;
     QAction *bilateralFilterAct;
     QAction *convolutionFilterAct;

     QAction *erosionAct;
	 QAction *dilationAct;
	 QAction *morphoExtAct;

	 QAction *thresholdAct;
	 QAction *sobelAct;
	 QAction *scharrAct;
	 QAction *laplaceAct;
	 QAction *cannyAct;

	 QAction *houghLineAct;
	 QAction *houghCircleAct;
	 QAction *imgMomentsAct;
	 QAction *findContourAct;
	 QAction *convexHullAct;
	 QAction *bdBoxCircleAct;
	 QAction *bdRotEclipseAct;

	 QAction *harrisAct;
	 QAction *shiTomasiAct;
	 QAction *cornerDetectAct;

	 QAction *featureSurfAct;
	 QAction *featureSiftAct;
     QAction *featureSiftActWithMask;
	 QAction *matchingBFAct;
	 QAction *matchingFLANNAct;
	 QAction *matchingHomographyAct;
	 QAction *matchingTemplateAct;

	 QAction *correctMorphAct;


	 QAction *pyramidAct;
	 QAction *remapAct;
	 QAction *affineAct;
	 QAction *histogramAct;
	 QAction *equalizationAct;
	 QAction *backprojectionAct;


	 QAction *grayscaleMyImplAct;
	 QAction *binaryMyImplAct;
	 QAction *robertsMyImplAct;
	 QAction *sobelMyImplAct;
	 QAction *hitmissMyImplAct;
	 QAction *otsuMyImplAct;
	 QAction *growingMyImplAct;
	 QAction *antMyImplAct;

     // action rectangle removing
     QAction *objectRemovingAct;


     QMenu *fileMenu;
     QMenu *viewMenu;
     QMenu *opencvMenu;
     QMenu *myimplMenu;
     QMenu *pluginsMenu;
     QMenu *helpMenu;

     QToolBar *fileToolBar;
     QToolBar *viewToolBar;
 };

 #endif
