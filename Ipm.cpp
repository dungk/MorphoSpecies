#include <QApplication>

#include "ImageViewer.h"
#include "ImageConvert.h"
#include "impls/algorithms/ObjectRemoval.h"
#include <istream>
#include <QtCore/QDebug>
#include <string>
#include <ostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ImageViewer imageViewer;

#if defined(Q_OS_SYMBIAN)
    imageViewer.showMaximized();
#else
     imageViewer.show();
#endif
    return app.exec();
}



