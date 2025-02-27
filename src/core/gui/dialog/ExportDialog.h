/*
 * Xournal++
 *
 * Dialog with export settings
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <cstddef>
#include <vector>

#include "control/jobs/ImageExport.h"
#include "control/settings/Settings.h"
#include "gui/GladeGui.h"
#include "util/PageRange.h"

class ExportDialog: public GladeGui {
public:
    ExportDialog(GladeSearchpath* gladeSearchPath);

public:
    void show(GtkWindow* parent) override;
    void initPages(size_t current, size_t count);
    bool isConfirmed() const;
    PageRangeVector getRange();
    bool progressiveMode();
    ExportBackgroundType getBackgroundType();

    /**
     * @brief Reads the quality parameter from the dialog
     *
     * @return The selected quality parameter
     */
    RasterImageQualityParameter getPngQualityParameter();

    /**
     * @brief Hides the quality settings
     */
    void removeQualitySetting();

    /**
     * @brief Show "progressive mode" checkbox and hide quality settings
     * (both cannot be shown at the same time)
     */
    void showProgressiveMode();

    /**
     * @brief Handler for changes in combobox cbQuality
     */
    static void selectQualityCriterion(GtkComboBox* comboBox, ExportDialog* self);

private:
    size_t currentPage = 0;
    size_t pageCount = 0;

    bool confirmed = false;
};
