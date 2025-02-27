#include "UndoRedoController.h"

#include "gui/XournalView.h"

#include "Control.h"

UndoRedoController::UndoRedoController(Control* control): control(control) {}

UndoRedoController::~UndoRedoController() {
    this->control = nullptr;
    this->layer = nullptr;
    elements.clear();
}

void UndoRedoController::before() {
    EditSelection* selection = control->getWindow()->getXournal()->getSelection();
    if (selection != nullptr) {
        layer = selection->getSourceLayer();
        std::copy(selection->getElements().begin(), selection->getElements().end(), std::back_inserter(elements));
    }

    control->clearSelection();
}

void UndoRedoController::after() {
    // Restore selection, if any

    if (layer == nullptr) {
        // No layer - no selection
        return;
    }

    Document* doc = control->getDocument();

    PageRef page = control->getCurrentPage();
    size_t pageNo = doc->indexOf(page);
    XojPageView* view = control->getWindow()->getXournal()->getViewFor(pageNo);

    if (!view || !page) {
        return;
    }

    std::vector<Element*> visibleElements;
    for (Element* e: elements) {
        if (layer->indexOf(e) == -1) {
            // Element is gone - so it's not selectable
            continue;
        }

        visibleElements.push_back(e);
    }
    if (!visibleElements.empty()) {
        auto* selection = new EditSelection(control->getUndoRedoHandler(), visibleElements, view, page);
        control->getWindow()->getXournal()->setSelection(selection);
    }
}

void UndoRedoController::undo(Control* control) {
    UndoRedoController handler(control);
    handler.before();

    // Move out of text mode to allow textboxundo to work
    control->clearSelectionEndText();

    control->getUndoRedoHandler()->undo();

    handler.after();
}

void UndoRedoController::redo(Control* control) {
    UndoRedoController handler(control);
    handler.before();

    control->getUndoRedoHandler()->redo();

    handler.after();
}
