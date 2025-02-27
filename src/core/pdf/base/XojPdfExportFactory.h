/*
 * Xournal++
 *
 * PDF Document Export Abstraction Interface
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>
#include <vector>

#include "XojPdfExport.h"


class Document;
class ProgressListener;

class XojPdfExportFactory {
private:
    XojPdfExportFactory();
    virtual ~XojPdfExportFactory();

public:
    static std::unique_ptr<XojPdfExport> createExport(Document* doc, ProgressListener* listener);

private:
};
