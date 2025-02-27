/*
 * Xournal++
 *
 * A stroke which is temporary used if you erase a part
 * This class remembers which sections of a stroke have not yet been erased, until the eraser sequence is concluded
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <memory>
#include <mutex>

#include <gtk/gtk.h>

#include "model/PathParameter.h"
#include "model/Stroke.h"
#include "util/Rectangle.h"
#include "util/UnionOfIntervals.h"

#include "config-debug.h"

class Range;
struct PaddedBox;

class ErasableStroke {
public:
    ErasableStroke(const Stroke& stroke);
    ~ErasableStroke();

    /**
     * @brief Type for subsections of a stroke
     */
    using SubSection = Interval<PathParameter>;

public:
    /**
     * @brief Starts erasing the stroke, with the already computed intersection parameters
     * @param intersectionParameters Vector of even length containing the parameters of the sections to be erased
     * @param range Range (destined to rerendering) that will be widened around the eraser box
     * @param view Redrawable to which we pass areas that need rerendering
     * view is only used when the stroke is from the highlighter tool.
     * The rerendered areas correspond to where the stroke overlaps itself after being split in two (or more)
     */
    void beginErasure(const IntersectionParametersContainer& intersectionParameters, Range& range);

    /**
     * @brief Erase the stroke
     * @param box PaddedBox of the eraser
     * @param range Range (destined to rerendering) that will be widened around the eraser box
     * @param view Redrawable to which we pass areas that need rerendering
     * view is only used when the stroke is from the highlighter tool.
     * The rerendered areas correspond to where the stroke overlaps itself after being split in two (or more)
     */
    void erase(const PaddedBox& box, Range& range);

    /**
     * @brief Get the resulting strokes (if any) once the erasing is finished
     * @return A vector of pointers to newly created strokes (owned by the caller).
     * The resulting strokes correspond to what's left of the original stroke
     */
    std::vector<std::unique_ptr<Stroke>> getStrokes() const;

    /**
     * @brief Get a clone of the data, in the form of a vector of sections
     * @return The clone
     */
    std::vector<ErasableStroke::SubSection> getRemainingSubSectionsVector() const;

    /**
     * @return true if the stroke is closed (i.e. the first and last knots are very close), false otherwise.
     */
    bool isClosedStroke() const;

    /**
     * @brief Get the bounding box of a subsection.
     * The bounding box is either pulled from cache or computed and added to cache
     * @return The bounding box.
     * NB: The bounding boxes obtained here are imprecise: they do not take pressure into account but use the stroke's
     * width instead.
     */
    xoj::util::Rectangle<double> getSubSectionBoundingBox(const SubSection& section) const;

protected:
    /**
     * @brief Given a vector of subsections, compute (coarsely) where those subsections overlap.
     * @param subsections The input subsections
     * @return Vector of disjoint rectangles. Altogether, those rectangles contains all the overlaps of the subsections
     */
    void addOverlapsToRange(const std::vector<SubSection>& subsections, Range& range);

public:
    /**
     * @brief Reference to the stroke being erased
     */
    const Stroke& stroke;

protected:
    /**
     * @brief Parameters for the subsections that have not (yet) been erased
     * Protected by the associated mutex
     */
    UnionOfIntervals<PathParameter> remainingSections{};
    mutable std::mutex sectionsMutex;

    /**
     * @brief Cache for the bounding boxes of the remaining subsections
     * Usually pretty small (< 10): std::vector is faster than std::map
     * Protected by the associated mutex
     */
    mutable std::vector<std::pair<SubSection, xoj::util::Rectangle<double>>> boundingBoxes;
    mutable std::mutex boxesMutex;

    /**
     * @brief Whether or not the last iteration erased anything
     */
    bool changesAtLastIteration = false;

    /**
     * @brief Whether the stroke is closed (i.e. the first and last knots are very close)
     */
    bool closedStroke;
    static constexpr double CLOSED_STROKE_DISTANCE = 0.3;

#ifdef DEBUG_ERASABLE_STROKE_BOXES
public:
    mutable cairo_surface_t* surfDebug = nullptr;
    mutable cairo_t* crDebug = nullptr;

    static void paintDebugRect(const xoj::util::Rectangle<double>& rect, char color, cairo_t* cr);
#endif

public:
    /**
     * Binary tree used for searching for overlaps between subsections
     */
    class OverlapTree;
};
