#include "RenderJob.h"

#include <cmath>

#include "control/Control.h"
#include "control/ToolHandler.h"
#include "gui/PageView.h"
#include "gui/XournalView.h"
#include "model/Document.h"
#include "util/Rectangle.h"
#include "util/Util.h"
#include "view/DocumentView.h"

using xoj::util::Rectangle;

RenderJob::RenderJob(XojPageView* view): view(view) {}

auto RenderJob::getSource() -> void* { return this->view; }

void RenderJob::rerenderRectangle(Rectangle<double> const& rect) {
    double zoom = view->xournal->getZoom();
    Document* doc = view->xournal->getDocument();

    /**
     * Make sure the mask is big enough
     * The +1 covers examples like rect.x = 0.4, rect.width = 1 and zoom = 1
     * We need a mask of width 2 pixels for that...
     **/
    auto x = int(std::floor(rect.x * zoom));
    auto y = int(std::floor(rect.y * zoom));
    auto width = int(std::ceil(rect.width * zoom)) + 1;
    auto height = int(std::ceil(rect.height * zoom)) + 1;

    cairo_surface_t* rectBuffer = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t* crRect = cairo_create(rectBuffer);
    cairo_translate(crRect, -x, -y);
    cairo_scale(crRect, zoom, zoom);

    DocumentView v;
    Control* control = view->getXournal()->getControl();
    v.setMarkAudioStroke(control->getToolHandler()->getToolType() == TOOL_PLAY_OBJECT);
    v.setPdfCache(view->xournal->getCache());

    doc->lock();
    v.drawPage(view->page, crRect, false);
    doc->unlock();

    cairo_destroy(crRect);

    view->drawingMutex.lock();

    cairo_t* crPageBuffer = cairo_create(view->crBuffer);

    cairo_set_operator(crPageBuffer, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_surface(crPageBuffer, rectBuffer, x, y);
    cairo_rectangle(crPageBuffer, x, y, width, height);
    cairo_fill(crPageBuffer);

    cairo_destroy(crPageBuffer);

    cairo_surface_destroy(rectBuffer);

    view->drawingMutex.unlock();
}

void RenderJob::run() {
    double zoom = this->view->xournal->getZoom();

    this->view->repaintRectMutex.lock();

    bool rerenderComplete = this->view->rerenderComplete;
    auto rerenderRects = std::move(this->view->rerenderRects);

    this->view->rerenderComplete = false;

    this->view->repaintRectMutex.unlock();

    int dpiScaleFactor = this->view->xournal->getDpiScaleFactor();

    if (rerenderComplete || dpiScaleFactor > 1) {
        Document* doc = this->view->xournal->getDocument();

        int dispWidth = this->view->getDisplayWidth();
        int dispHeight = this->view->getDisplayHeight();

        dispWidth *= dpiScaleFactor;
        dispHeight *= dpiScaleFactor;
        zoom *= dpiScaleFactor;

        cairo_surface_t* crBuffer = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, dispWidth, dispHeight);
        cairo_t* cr2 = cairo_create(crBuffer);
        cairo_scale(cr2, zoom, zoom);

        doc->lock();

        Control* control = view->getXournal()->getControl();
        DocumentView localView;
        localView.setMarkAudioStroke(control->getToolHandler()->getToolType() == TOOL_PLAY_OBJECT);
        localView.setPdfCache(this->view->xournal->getCache());
        localView.drawPage(this->view->page, cr2, false);

        cairo_destroy(cr2);

        this->view->drawingMutex.lock();

        if (this->view->crBuffer) {
            cairo_surface_destroy(this->view->crBuffer);
        }
        this->view->crBuffer = crBuffer;

        this->view->drawingMutex.unlock();
        doc->unlock();
    } else {
        for (Rectangle<double> const& rect: rerenderRects) { rerenderRectangle(rect); }
    }

    // Schedule a repaint of the widget
    repaintWidget(this->view->getXournal()->getWidget());
}

/**
 * Repaint the widget in UI Thread
 */
void RenderJob::repaintWidget(GtkWidget* widget) {
    // "this" is not needed, "widget" is in
    // the closure, therefore no sync needed
    // Because of this the argument "widget" is needed
    Util::execInUiThread([=]() { gtk_widget_queue_draw(widget); });
}

auto RenderJob::getType() -> JobType { return JOB_TYPE_RENDER; }
