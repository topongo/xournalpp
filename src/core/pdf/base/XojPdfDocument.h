/*
 * Xournal++
 *
 * PDF Document Container
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>
#include <vector>

#include "pdf/base/XojPdfDocumentInterface.h"

#include "XojPdfBookmarkIterator.h"
#include "XojPdfDocumentInterface.h"
#include "XojPdfPage.h"


class XojPdfDocument: XojPdfDocumentInterface {
public:
    XojPdfDocument();
    XojPdfDocument(const XojPdfDocument& doc);
    ~XojPdfDocument() override;

public:
    XojPdfDocument& operator=(const XojPdfDocument& doc);
    bool operator==(XojPdfDocument& doc) const;
    void assign(XojPdfDocumentInterface* doc) override;
    bool equals(XojPdfDocumentInterface* doc) const override;

public:
    bool save(fs::path const& file, GError** error) const override;
    bool load(fs::path const& file, std::string password, GError** error) override;
    bool load(gpointer data, gsize length, std::string password, GError** error) override;
    bool isLoaded() const override;

    XojPdfPageSPtr getPage(size_t page) const override;
    size_t getPageCount() const override;
    XojPdfBookmarkIterator* getContentsIter() const override;

private:
    XojPdfDocumentInterface* doc;
};
