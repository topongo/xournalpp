#include "BackgroundView.h"

#include "control/PdfCache.h"
#include "model/BackgroundConfig.h"
#include "model/Document.h"

#include "DottedBackgroundView.h"
#include "GraphBackgroundView.h"
#include "ImageBackgroundView.h"
#include "IsoDottedBackgroundView.h"
#include "IsoGraphBackgroundView.h"
#include "LinedBackgroundView.h"
#include "PdfBackgroundView.h"
#include "PlainBackgroundView.h"
#include "RuledBackgroundView.h"
#include "StavesBackgroundView.h"
#include "TransparentCheckerboardBackgroundView.h"

using namespace xoj::view;

auto BackgroundView::createRuled(double width, double height, Color backgroundColor, const PageType& pt,
                                 double lineWidthFactor) -> std::unique_ptr<BackgroundView> {
    std::unique_ptr<OneColorBackgroundView> res;
    switch (pt.format) {
        case PageTypeFormat::Plain:
            return std::make_unique<PlainBackgroundView>(width, height, backgroundColor);
        case PageTypeFormat::Ruled:
            res = std::make_unique<RuledBackgroundView>(width, height, backgroundColor, pt.config);
            break;
        case PageTypeFormat::Lined:
            res = std::make_unique<LinedBackgroundView>(width, height, backgroundColor, pt.config);
            break;
        case PageTypeFormat::Graph:
            res = std::make_unique<GraphBackgroundView>(width, height, backgroundColor, pt.config);
            break;
        case PageTypeFormat::Staves:
            res = std::make_unique<StavesBackgroundView>(width, height, backgroundColor, pt.config);
            break;
        case PageTypeFormat::Dotted:
            res = std::make_unique<DottedBackgroundView>(width, height, backgroundColor, pt.config);
            break;
        case PageTypeFormat::IsoGraph:
            res = std::make_unique<IsoGraphBackgroundView>(width, height, backgroundColor, pt.config);
            break;
        case PageTypeFormat::IsoDotted:
            res = std::make_unique<IsoDottedBackgroundView>(width, height, backgroundColor, pt.config);
            break;
        default:
            g_warning("BackgroundView::createForPage unknowntype: %d", static_cast<int>(pt.format));
            return nullptr;
    }
    // In page template previews, the line width is multiplied to make the lines actually visible.
    res->multiplyLineWidth(lineWidthFactor);
    return res;
}

auto BackgroundView::createForPage(PageRef page, BackgroundFlags bgFlags, PdfCache* pdfCache)
        -> std::unique_ptr<BackgroundView> {
    const double width = page->getWidth();
    const double height = page->getHeight();
    if (!page->isLayerVisible(0)) {
        return std::make_unique<TransparentCheckerboardBackgroundView>(width, height);
    }

    const auto pt = page->getBackgroundType();
    if (pt.isSpecial()) {
        switch (pt.format) {
            case PageTypeFormat::Image:
                if (bgFlags.showImage) {
                    return std::make_unique<ImageBackgroundView>(width, height, page->getBackgroundImage());
                }
                break;
            case PageTypeFormat::Pdf:
                if (bgFlags.showPDF) {
                    return std::make_unique<PdfBackgroundView>(width, height, page->getPdfPageNr(), pdfCache);
                }
                break;
            case PageTypeFormat::Copy:
                g_warning("BackgroundView::createForPage for 'Copy' page type");
                return nullptr;
            default:
                g_warning("BackgroundView::createForPage unknown type: %d\n", static_cast<int>(pt.format));
                return nullptr;
        }
    } else {
        if (bgFlags.showRuling) {
            return createRuled(width, height, page->getBackgroundColor(), pt);
        }
    }
    // In case the flags tell us to hide the background, create a dummy view.
    return std::make_unique<BackgroundView>(width, height);
}
