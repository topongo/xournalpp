/*
 * Xournal++
 *
 * Displays a single page
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <mutex>
#include <vector>

#include <cairo.h>

#include "gui/inputdevices/PositionInputData.h"
#include "model/PageListener.h"
#include "model/PageRef.h"
#include "model/Stroke.h"
#include "model/TexImage.h"
#include "util/Range.h"

#include "Layout.h"
#include "Redrawable.h"

class EditSelection;
class EraseHandler;
class InputHandler;
class SearchControl;
class Selection;
class PdfElemSelection;
class Settings;
class Text;
class TextEditor;
class VerticalToolHandler;
class XournalView;

class XojPageView: public Redrawable, public PageListener {
public:
    XojPageView(XournalView* xournal, const PageRef& page);
    ~XojPageView() override;

public:
    void updatePageSize(double width, double height);

    void rerenderPage() override;
    void rerenderRect(double x, double y, double width, double height) override;

    void repaintPage() override;
    void repaintArea(double x1, double y1, double x2, double y2) override;

    void setSelected(bool selected);

    void setIsVisible(bool visible);

    bool isSelected() const;

    void endText();

    bool searchTextOnPage(std::string& text, int* occures, double* top);

    bool onKeyPressEvent(GdkEventKey* event);
    bool onKeyReleaseEvent(GdkEventKey* event);

    bool cut();
    bool copy();
    bool paste();

    bool actionDelete();

    void deleteViewBuffer() override;

    /**
     * Returns whether this PageView contains the
     * given point on the display
     */
    bool containsPoint(int x, int y, bool local = false) const;

    /**
     * Returns Row assigned in current layout
     */
    int getMappedRow() const;

    /**
     * Returns Column assigned in current layout
     */
    int getMappedCol() const;

    GdkRGBA getSelectionColor() override;
    int getBufferPixels();

    /**
     * 0 if currently visible
     * -1 if no image is saved (never visible or cleanup)
     * else the time in Seconds
     */
    int getLastVisibleTime();
    TextEditor* getTextEditor();

    /**
     * Returns a reference to the XojPage belonging to
     * this PageView
     */
    const PageRef getPage();

    XournalView* getXournal();

    /**
     * Returns the width of this PageView
     */
    double getWidth() const;

    /**
     * Returns the height of this XojPageView
     */
    double getHeight() const;

    /**
     * Returns the width of this XojPageView as displayed
     * on the display taking into account the current zoom
     */
    int getDisplayWidth() const;
    double getDisplayWidthDouble() const;
    /**
     * Returns the height of this XojPageView as displayed
     * on the display taking into account the current zoom
     */
    int getDisplayHeight() const;
    double getDisplayHeightDouble() const;

    /**
     * Returns the x coordinate of this XojPageView with
     * respect to the display
     */
    int getX() const override;

    /**
     * Returns the y coordinate of this XojPageView with
     * respect to the display
     */
    int getY() const override;

    TexImage* getSelectedTex();
    Text* getSelectedText();

    xoj::util::Rectangle<double> getRect() const;

public:  // event handler
    bool onButtonPressEvent(const PositionInputData& pos);
    bool onButtonReleaseEvent(const PositionInputData& pos);
    bool onButtonDoublePressEvent(const PositionInputData& pos);
    bool onButtonTriplePressEvent(const PositionInputData& pos);
    bool onMotionNotifyEvent(const PositionInputData& pos);
    void onMotionCancelEvent();

    /**
     * This event fires after onButtonPressEvent and also
     * if no input sequence is actively running and a stylus button was pressed
     */
    bool onButtonClickEvent(const PositionInputData& pos);

    /**
     * This method actually repaints the XojPageView, triggering
     * a rerender call if necessary
     */
    bool paintPage(cairo_t* cr, GdkRectangle* rect);

    /**
     * Does the painting, called in synchronized block
     */
    void paintPageSync(cairo_t* cr, GdkRectangle* rect);

public:  // listener
    void rectChanged(xoj::util::Rectangle<double>& rect) override;
    void rangeChanged(Range& range) override;
    void pageChanged() override;
    void elementChanged(Element* elem) override;

private:
    void handleScrollEvent(GdkEventButton* event);

    void startText(double x, double y);

    void addRerenderRect(double x, double y, double width, double height);

    void drawLoadingPage(cairo_t* cr);

    void setX(int x);
    void setY(int y);

    void setMappedRowCol(int row, int col);  // row, column assigned by mapper during layout.

    /**
     * Shows the floating toolbox at the location of an input event
     */
    void showFloatingToolbox(const PositionInputData& pos);

    /**
     * Shows the PDF toolbox at the location of an input event
     */
    void showPdfToolbox(const PositionInputData& pos);


private:
    PageRef page;
    XournalView* xournal = nullptr;
    Settings* settings = nullptr;
    EraseHandler* eraser = nullptr;
    InputHandler* inputHandler = nullptr;

    /**
     * The selected (while selection)
     */
    Selection* selection = nullptr;

    /**
     * The text editor View
     */
    TextEditor* textEditor = nullptr;

    /**
     * For keeping old text changes to undo!
     */
    Text* oldtext;

    bool selected = false;

    cairo_surface_t* crBuffer = nullptr;

    bool inEraser = false;

    /**
     * Vertical Space
     */
    VerticalToolHandler* verticalSpace{};

    /**
     * Search handling
     */
    SearchControl* search = nullptr;

    /**
     * Unixtimestam when the page was last time in the visible area
     */
    int lastVisibleTime = -1;

    std::mutex repaintRectMutex;
    std::vector<xoj::util::Rectangle<double>> rerenderRects;
    bool rerenderComplete = false;

    std::mutex drawingMutex;

    int dispX{};  // position on display - set in Layout::layoutPages
    int dispY{};


    int mappedRow{};
    int mappedCol{};


    friend class RenderJob;
    friend class InputHandler;
    friend class BaseSelectObject;
    friend class SelectObject;
    friend class PlayObject;
    friend class PdfFloatingToolbox;
    // only function allowed to setX(), setY(), setMappedRowCol():
    friend void Layout::layoutPages(int width, int height);
};
