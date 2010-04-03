/*=====================================================================

PIXHAWK Micro Air Vehicle Flying Robotics Toolkit

(c) 2009, 2010 PIXHAWK PROJECT  <http://pixhawk.ethz.ch>

This file is part of the PIXHAWK project

    PIXHAWK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PIXHAWK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PIXHAWK. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
 * @file
 *   @brief Definition of Head Down Display (HDD)
 *
 *   @author Lorenz Meier <mavteam@student.ethz.ch>
 *
 */

#ifndef HDDISPLAY_H
#define HDDISPLAY_H

#include <QtGui/QWidget>
#include <QColor>
#include <QTimer>
#include <QFontDatabase>
#include <cmath>

#include "UASInterface.h"

namespace Ui {
    class HDDisplay;
}

class HDDisplay : public QWidget {
    Q_OBJECT
public:
    HDDisplay(QWidget *parent = 0);
    ~HDDisplay();

public slots:
    /** @brief Update a HDD value */
    void updateValue(UASInterface* uas, QString name, double value, quint64 msec);
    void start();
    void stop();
    void setActiveUAS(UASInterface* uas);

    protected slots:
    void paintGL();

protected:
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent * event);
    float refLineWidthToPen(float line);
    float refToScreenX(float x);
    float refToScreenY(float y);
    void rotatePolygonClockWiseRad(QPolygonF& p, float angle, QPointF origin);
    void drawPolygon(QPolygonF refPolygon, QPainter* painter);
    void drawLine(float refX1, float refY1, float refX2, float refY2, float width, const QColor& color, QPainter* painter);
    void drawEllipse(float refX, float refY, float radiusX, float radiusY, float startDeg, float endDeg, float lineWidth, const QColor& color, QPainter* painter);
    void drawCircle(float refX, float refY, float radius, float startDeg, float endDeg, float lineWidth, const QColor& color, QPainter* painter);

    void drawChangeRateStrip(float xRef, float yRef, float height, float minRate, float maxRate, float value, QPainter* painter);
    void drawChangeIndicatorGauge(float xRef, float yRef, float radius, float expectedMaxChange, float value, const QColor& color, QPainter* painter, bool solid=true);
    void drawGauge(float xRef, float yRef, float radius, float min, float max, const QString name, float value, const QColor& color, QPainter* painter, QPair<float, float> goodRange, QPair<float, float> criticalRange, bool solid=true);
    void drawSystemIndicator(float xRef, float yRef, int maxNum, float maxWidth, float maxHeight, QPainter* painter);
    void paintText(QString text, QColor color, float fontSize, float refX, float refY, QPainter* painter);

    UASInterface* uas; ///< The uas currently monitored
    QMap<QString, float> values; ///< The variables this HUD displays
    QMap<QString, float> valuesDot; ///< First derivative of the variable
    QMap<QString, float> valuesMean; ///< Mean since system startup for this variable
    QMap<QString, int> valuesCount; ///< Number of values received so far
    QMap<QString, quint64> lastUpdate; ///< The last update time for this variable
    double scalingFactor; ///< Factor used to scale all absolute values to screen coordinates
    float xCenterOffset, yCenterOffset; ///< Offset from center of window in mm coordinates
    float vwidth; ///< Virtual width of this window, 200 mm per default. This allows to hardcode positions and aspect ratios. This virtual image plane is then scaled to the window size.
    float vheight; ///< Virtual height of this window, 150 mm per default

    int xCenter; ///< Center of the HUD instrument in pixel coordinates. Allows to off-center the whole instrument in its OpenGL window, e.g. to fit another instrument
    int yCenter; ///< Center of the HUD instrument in pixel coordinates. Allows to off-center the whole instrument in its OpenGL window, e.g. to fit another instrument

    // HUD colors
    QColor backgroundColor;    ///< Background color
    QColor defaultColor;       ///< Color for most HUD elements, e.g. pitch lines, center cross, change rate gauges
    QColor setPointColor;      ///< Color for the current control set point, e.g. yaw desired
    QColor warningColor;       ///< Color for warning messages
    QColor criticalColor;      ///< Color for caution messages
    QColor infoColor;          ///< Color for normal/default messages
    QColor fuelColor;          ///< Current color for the fuel message, can be info, warning or critical color

    // Blink rates
    int warningBlinkRate;      ///< Blink rate of warning messages, will be rounded to the refresh rate

    QTimer* refreshTimer;      ///< The main timer, controls the update rate
    QPainter* hudPainter;
    QFont font;                ///< The HUD font, per default the free Bitstream Vera SANS, which is very close to actual HUD fonts
    QFontDatabase fontDatabase;///< Font database, only used to load the TrueType font file (the HUD font is directly loaded from file rather than from the system)
    bool hardwareAcceleration; ///< Enable hardware acceleration

    float strongStrokeWidth;   ///< Strong line stroke width, used throughout the HUD
    float normalStrokeWidth;   ///< Normal line stroke width, used throughout the HUD
    float fineStrokeWidth;     ///< Fine line stroke width, used throughout the HUD

private:
    Ui::HDDisplay *m_ui;
};

#endif // HDDISPLAY_H
