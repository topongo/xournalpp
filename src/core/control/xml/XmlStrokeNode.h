/*
 * Xournal++
 *
 * XML Writer helper class
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "model/Point.h"

#include "XmlNode.h"

class XmlStrokeNode: public XmlNode {
public:
    XmlStrokeNode(const char* tag);
    virtual ~XmlStrokeNode();

public:
    void setWidth(double width, const double* widths, int widthsLength);

    void writeOut(OutputStream* out) override;

private:
    Point* points;
    int pointsLength;

    double width;

    double* widths;
    int widthsLength;
};
