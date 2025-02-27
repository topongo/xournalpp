/*
 * Xournal++
 *
 * Base class for Background selection dialog
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>
#include <vector>

#include "gui/GladeGui.h"


class Document;
class Settings;
class BaseElementView;

class BackgroundSelectDialogBase: public GladeGui {
public:
    BackgroundSelectDialogBase(GladeSearchpath* gladeSearchPath, Document* doc, Settings* settings,
                               const std::string& glade, const std::string& mainWnd);
    ~BackgroundSelectDialogBase() override;

public:
    Settings* getSettings();
    void show(GtkWindow* parent) override;
    virtual void setSelected(int selected);

protected:
    void layout();

private:
    static void sizeAllocate(GtkWidget* widget, GtkRequisition* requisition, BackgroundSelectDialogBase* dlg);

private:
protected:
    Settings* settings = nullptr;
    GtkWidget* scrollPreview = nullptr;
    GtkWidget* layoutContainer = nullptr;

    Document* doc = nullptr;

    /**
     * Selection confirmed
     */
    bool confirmed = false;

    /**
     * Selected image, none if negative
     */
    int selected = -1;

    /**
     * To check if the size has real changed
     */
    int lastWidth = 0;

    /**
     * Elements to display
     */
    std::vector<BaseElementView*> elements;
};
