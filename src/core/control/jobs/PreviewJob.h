/*
 * Xournal++
 *
 * A job which handles preview repaint
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>
#include <vector>

#include <gtk/gtk.h>

#include "Job.h"


class SidebarPreviewBaseEntry;
class Document;

/**
 * @brief A Job which renders a SidebarPreviewPage
 */
class PreviewJob: public Job {
public:
    PreviewJob(SidebarPreviewBaseEntry* sidebar);

protected:
    void onDelete() override;
    ~PreviewJob() override;

public:
    void* getSource() override;

    void run() override;

    JobType getType() override;

private:
    void initGraphics();
    void clipToPage();
    void drawBorder();
    void finishPaint();
    void drawPage();

private:
    /**
     * Graphics buffer
     */
    cairo_surface_t* crBuffer = nullptr;

    /**
     * Graphics drawing
     */
    cairo_t* cr2 = nullptr;

    /**
     * Zoom factor
     */
    double zoom = 0;

    /**
     * Sidebar preview
     */
    SidebarPreviewBaseEntry* sidebarPreview = nullptr;
};
