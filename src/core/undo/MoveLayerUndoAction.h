/*
 * Xournal++
 *
 * Undo action for insert  layer
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>
#include <vector>

#include "UndoAction.h"


class Layer;
class LayerController;

class MoveLayerUndoAction: public UndoAction {
public:
    MoveLayerUndoAction(LayerController* layerController, const PageRef& page, Layer* layer, Layer::Index oldLayerPos,
                        Layer::Index newLayerPos);
    ~MoveLayerUndoAction() override;

public:
    bool undo(Control* control) override;
    bool redo(Control* control) override;

    std::string getText() override;

private:
    Layer::Index oldLayerPos;
    Layer::Index newLayerPos;
    LayerController* layerController;
    Layer* layer;
};
