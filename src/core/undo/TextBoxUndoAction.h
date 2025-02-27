/*
 * Xournal++
 *
 * Undo action for insert (write text, draw stroke...)
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "UndoAction.h"

class Element;
class Layer;
class XojPage;

class TextBoxUndoAction: public UndoAction {
public:
    TextBoxUndoAction(const PageRef& page, Layer* layer, Element* element, Element* oldelement);
    ~TextBoxUndoAction() override;

public:
    bool undo(Control* control) override;
    bool redo(Control* control) override;

    std::string getText() override;

private:
    Layer* layer;
    Element* element;
    Element* oldelement;
};
