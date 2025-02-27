#include "GraphBackgroundView.h"

#include <cmath>

#include "model/BackgroundConfig.h"
#include "util/Util.h"

using namespace background_config_strings;
using namespace xoj::view;

GraphBackgroundView::GraphBackgroundView(double pageWidth, double pageHeight, Color backgroundColor,
                                         const BackgroundConfig& config):
        OneColorBackgroundView(pageWidth, pageHeight, backgroundColor, config, DEFAULT_LINE_WIDTH, DEFAULT_LINE_COLOR,
                               ALT_DEFAULT_LINE_COLOR) {

    config.loadValue(CFG_RASTER, squareSize);
    config.loadValue(CFG_MARGIN, margin);

    if (int roundToGrid = 0; margin > 0.0 && config.loadValue(CFG_ROUND_MARGIN, roundToGrid) && roundToGrid) {
        roundUpMargin = true;
    }
}

void GraphBackgroundView::draw(cairo_t* cr) const {
    // Paint the background color
    PlainBackgroundView::draw(cr);

    // Get the bounds of the mask, in page coordinates
    double minX;
    double maxX;
    double minY;
    double maxY;
    cairo_clip_extents(cr, &minX, &minY, &maxX, &maxY);

    if (margin > 0.0) {
        minX = std::max(minX, margin);
        maxX = std::min(maxX, pageWidth - margin);
        minY = std::max(minY, margin);
        maxY = std::min(maxY, pageHeight - margin);
    }

    //  Add a 0.5 * lineWidth padding in case the line is just outside the mask but its thickness still makes it
    //  (partially) visible
    const double halfLineWidth = 0.5 * lineWidth;
    auto [indexMinX, indexMaxX] =
            getIndexBounds(minX - halfLineWidth, maxX + halfLineWidth, squareSize, squareSize, pageWidth);
    auto [indexMinY, indexMaxY] =
            getIndexBounds(minY - halfLineWidth, maxY + halfLineWidth, squareSize, squareSize, pageHeight);

    if (roundUpMargin) {
        minX = minX == margin ? indexMinX * squareSize : minX;
        maxX = maxX == pageWidth - margin ? indexMaxX * squareSize : maxX;
        minY = minY == margin ? indexMinY * squareSize : minY;
        maxY = maxY == pageHeight - margin ? indexMaxY * squareSize : maxY;
    }

    for (int i = indexMinX; i <= indexMaxX; ++i) {
        cairo_move_to(cr, i * squareSize, minY);
        cairo_line_to(cr, i * squareSize, maxY);
    }
    for (int i = indexMinY; i <= indexMaxY; ++i) {
        cairo_move_to(cr, minX, i * squareSize);
        cairo_line_to(cr, maxX, i * squareSize);
    }

    cairo_save(cr);
    Util::cairo_set_source_rgbi(cr, foregroundColor);
    cairo_set_line_width(cr, lineWidth);
    cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
    cairo_stroke(cr);
    cairo_restore(cr);
}
