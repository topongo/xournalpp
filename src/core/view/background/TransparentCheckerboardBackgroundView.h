/*
 * Xournal++
 *
 * Class for transparent backgrounds (checkerboard pattern)
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "util/Color.h"

#include "BackgroundView.h"

class xoj::view::TransparentCheckerboardBackgroundView: public BackgroundView {
public:
    TransparentCheckerboardBackgroundView(double pageWidth, double pageHeight);
    virtual ~TransparentCheckerboardBackgroundView() noexcept;

    void draw(cairo_t* cr) const override;

private:
    cairo_pattern_t* pattern = nullptr;

protected:
    static cairo_pattern_t* createPattern();
    static constexpr int CHECKER_SIZE = 8;
    static constexpr Color DARK_GREY = Color(0x808080);
    static constexpr Color LIGHT_GREY = Color(0xc0c0c0);
};
