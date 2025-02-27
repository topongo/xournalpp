#include "LayerView.h"

#include "model/Layer.h"

#include "DebugShowRepaintBounds.h"

using xoj::util::Rectangle;
using namespace xoj::view;

LayerView::LayerView(const Layer* layer): layer(layer) {}

const Layer* LayerView::getLayer() const { return layer; }

void LayerView::draw(const Context& ctx) const {
    IF_DEBUG_REPAINT(int drawn = 0; int notDrawn = 0;);

    // Get the bounds of the mask, in page coordinates
    double minX;
    double maxX;
    double minY;
    double maxY;
    cairo_clip_extents(ctx.cr, &minX, &minY, &maxX, &maxY);

    for (auto& e: layer->getElements()) {

        IF_DEBUG_REPAINT({
            auto cr = ctx.cr;
            cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
            cairo_set_source_rgb(cr, 0, 1, 0);
            cairo_set_line_width(cr, 1);
            cairo_rectangle(cr, e->getX(), e->getY(), e->getElementWidth(), e->getElementHeight());
            cairo_stroke(cr);
        });

        if (e->intersectsArea(minX, minY, maxX - minX, maxY - minY)) {
            ElementView::createFromElement(e)->draw(ctx);
            IF_DEBUG_REPAINT(drawn++;);
        }
        IF_DEBUG_REPAINT(else { notDrawn++; });
    }
    IF_DEBUG_REPAINT(g_message("DBG:LayerView::draw: draw %i / not draw %i", drawn, notDrawn););
}
