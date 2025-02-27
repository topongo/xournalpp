/*
 * Xournal++
 *
 * PDF Document Container Interface
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>
#include <vector>

#include "XojPdfBookmarkIterator.h"
#include "XojPdfPage.h"
#include "filesystem.h"

class XojPdfDocumentInterface {
public:
    XojPdfDocumentInterface();
    virtual ~XojPdfDocumentInterface();

public:
    virtual void assign(XojPdfDocumentInterface* doc) = 0;
    virtual bool equals(XojPdfDocumentInterface* doc) const = 0;

public:
    virtual bool save(fs::path const& file, GError** error) const = 0;
    virtual bool load(fs::path const& file, std::string password, GError** error) = 0;
    virtual bool load(gpointer data, gsize length, std::string password, GError** error) = 0;
    virtual bool isLoaded() const = 0;

    virtual XojPdfPageSPtr getPage(size_t page) const = 0;
    virtual size_t getPageCount() const = 0;
    virtual XojPdfBookmarkIterator* getContentsIter() const = 0;

private:
};
