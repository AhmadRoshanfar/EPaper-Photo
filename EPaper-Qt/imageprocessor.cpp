#include "imageprocessor.h"

QList<QColor> currentPalette = {QColor(0, 0, 0),       // Black
                                QColor(255, 255, 255), // White
                                QColor(0, 255, 0),     // Green
                                QColor(0, 0, 255),     // Blue
                                QColor(255, 0, 0),     // Red
                                QColor(255, 255, 0)};  // Yellow

ImageProcessor::ImageProcessor(QObject *parent)
    : QObject(parent)
{
}

float ImageProcessor::getColorError(int r, int g, int b, const QColor &stdCol)
{
    float dr = r - stdCol.red();
    float dg = g - stdCol.green();
    float db = b - stdCol.blue();
    return dr * dr + dg * dg + db * db;
}

int ImageProcessor::getNearestColorIndex(int r, int g, int b)
{
    int bestIndex = 0;
    float minError = getColorError(r, g, b, currentPalette[0]);

    for (int i = 1; i < currentPalette.size(); ++i) {
        float currentError = getColorError(r, g, b, currentPalette[i]);
        if (currentError < minError) {
            minError = currentError;
            bestIndex = i;
        }
    }

    return bestIndex;
}

QVector3D ImageProcessor::addErrorValue(
    const QVector3D &color, float r, float g, float b, float factor)
{
    return QVector3D(color.x() + (r * factor) / 32.0f,
                     color.y() + (g * factor) / 32.0f,
                     color.z() + (b * factor) / 32.0f);
}

void ImageProcessor::applyDithering(QImage &img) {
    img = img.convertToFormat(QImage::Format_RGB888);
    const int w = img.width();
    const int h = img.height();

    // Two-row error buffer for Floyd-Steinberg dithering
    std::vector<std::vector<QVector3D>> errArr(2, std::vector<QVector3D>(w, QVector3D(0, 0, 0)));
    int aInd = 0, bInd = 1;

    for (int j = 0; j < h; ++j) {
        // Swap error buffer rows
        std::swap(aInd, bInd);

        // Clear the next row's errors
        for (int i = 0; i < w; ++i) {
            errArr[bInd][i] = QVector3D(0, 0, 0);
        }

        for (int i = 0; i < w; ++i) {
            QColor pixelColor = img.pixelColor(i, j);
            QVector3D oldError = errArr[aInd][i];

            // Apply accumulated error
            float r = std::clamp(pixelColor.red() + oldError.x(), 0.0f, 255.0f);
            float g = std::clamp(pixelColor.green() + oldError.y(), 0.0f, 255.0f);
            float b = std::clamp(pixelColor.blue() + oldError.z(), 0.0f, 255.0f);

            // Find nearest palette color
            QColor nearestColor = currentPalette[getNearestColorIndex(r, g, b)];
            img.setPixelColor(i, j, nearestColor);

            // Calculate quantization error
            float errR = r - nearestColor.red();
            float errG = g - nearestColor.green();
            float errB = b - nearestColor.blue();

            // Distribute error using Floyd-Steinberg weights (matches JavaScript logic)
            if (i == 0) {
                // Left edge
                if (i + 1 < w) {
                    errArr[bInd][i] = addErrorValue(errArr[bInd][i], errR, errG, errB, 7.0f);
                    errArr[bInd][i + 1] = addErrorValue(errArr[bInd][i + 1], errR, errG, errB, 2.0f);
                    errArr[aInd][i + 1] = addErrorValue(errArr[aInd][i + 1], errR, errG, errB, 7.0f);
                }
            } else if (i == w - 1) {
                // Right edge
                errArr[bInd][i - 1] = addErrorValue(errArr[bInd][i - 1], errR, errG, errB, 7.0f);
                errArr[bInd][i] = addErrorValue(errArr[bInd][i], errR, errG, errB, 9.0f);
            } else {
                // Middle pixels - standard Floyd-Steinberg distribution
                errArr[bInd][i - 1] = addErrorValue(errArr[bInd][i - 1], errR, errG, errB, 3.0f);
                errArr[bInd][i] = addErrorValue(errArr[bInd][i], errR, errG, errB, 5.0f);
                errArr[bInd][i + 1] = addErrorValue(errArr[bInd][i + 1], errR, errG, errB, 1.0f);
                errArr[aInd][i + 1] = addErrorValue(errArr[aInd][i + 1], errR, errG, errB, 7.0f);
            }
        }
    }
}
