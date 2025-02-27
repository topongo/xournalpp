/*
 * Xournal++
 *
 * Draw stroke
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <cstdint>

#include "View.h"

class Stroke;

class xoj::view::StrokeView: public xoj::view::ElementView {
public:
    StrokeView(const Stroke* s);
    ~StrokeView() = default;

public:
    /**
     * @brief Paint the given stroke.
     * @param dontRenderEditingStroke If true, and if the stroke is currently being (partially) erased, then render only
     * the not-yet-erased parts. (Typically set to true, except for previews and export jobs)
     * @param markAudioStroke If true, the stroke is faded out if it has no audio playback attached.
     * @param noColor If true, paint as if on a colorblind mask (only the alpha values are painted).
     */
    void draw(const Context& ctx) const override;

private:
    inline void pathToCairo(cairo_t* cr) const;

    /**
     * No pressure sensitivity, one line is drawn
     */
    void drawNoPressure(cairo_t* cr) const;

    /**
     * Draw a stroke with pressure, for this multiple
     * lines with different widths needs to be drawn
     */
    void drawWithPressure(cairo_t* cr) const;

private:
    const Stroke* s;

public:
    static constexpr double OPACITY_HIGHLIGHTER = 0.47;
    static constexpr double MINIMAL_ALPHA = 0.04;

    //  Must match the enum StrokeCapStyle in Stroke.h
    static constexpr cairo_line_cap_t CAIRO_LINE_CAP[] = {CAIRO_LINE_CAP_ROUND, CAIRO_LINE_CAP_BUTT,
                                                          CAIRO_LINE_CAP_SQUARE};
};
