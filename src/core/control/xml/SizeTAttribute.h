/*
 * Xournal++
 *
 * XML Writer helper class
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include "Attribute.h"

class SizeTAttribute: public XMLAttribute {
public:
    SizeTAttribute(const char* name, size_t value);
    ~SizeTAttribute() override;

public:
    void writeOut(OutputStream* out) override;

private:
    size_t value;
};
