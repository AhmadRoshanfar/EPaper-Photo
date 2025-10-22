// imageprocessor.h
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QColor>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QObject>
#include <QPainter>
#include <QStandardPaths>
#include <QVector3D>
#include <QtCore/qurl.h>
#include <QtMath>

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessor(QObject *parent = nullptr);

    void applyDithering(QImage &img);

public slots:

private:
    // Enhanced color matching methods
    float getColorError(int r, int g, int b, const QColor &stdCol);
    int getNearestColorIndex(int r, int g, int b);
    QVector3D addErrorValue(const QVector3D &color, float r, float g, float b, float factor);
};

#endif // IMAGEPROCESSOR_H
