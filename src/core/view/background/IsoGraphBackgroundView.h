/*
 * Xournal++
 *
 * Class for isometric graph backgrounds
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "BaseIsometricBackgroundView.h"

class BackgroundConfig;

class xoj::view::IsoGraphBackgroundView: public BaseIsometricBackgroundView {
public:
    IsoGraphBackgroundView(double pageWidth, double pageHeight, Color backgroundColor, const BackgroundConfig& config);
    virtual ~IsoGraphBackgroundView() = default;

protected:
    virtual void paintGrid(cairo_t* cr, int cols, int rows, double xstep, double ystep, double xOffset,
                           double yOffset) const override;

protected:
    constexpr static double DEFAULT_LINE_WIDTH = 1.0;
};
