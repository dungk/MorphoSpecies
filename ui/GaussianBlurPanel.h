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

#ifndef GAUSSIANBLURPANEL_H_
#define GAUSSIANBLURPANEL_H_

#include <QDialog>
#include <QWidget>
#include <QSlider>

namespace ui {

class GaussianBlurPanel : public QDialog
{
    Q_OBJECT

public:
    static const int DF_KERNEL_SIZE=0;
    GaussianBlurPanel(QWidget *parent);

private slots:
	void on_sliKernelSize_valueChanged(int value);
private:
    QSlider *sliKernelSize;
};

} /* namespace ui */
#endif /* GAUSSIANBLURPANEL_H_ */