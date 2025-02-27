/*
 * Xournal++
 *
 * Element that is audio enabled
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>
#include <vector>

#include "util/serializing/ObjectInputStream.h"
#include "util/serializing/ObjectOutputStream.h"

#include "Element.h"
#include "filesystem.h"


class AudioElement: public Element {
protected:
    AudioElement(ElementType type);

public:
    ~AudioElement() override;

    void setTimestamp(size_t timestamp);
    size_t getTimestamp() const;

    void setAudioFilename(fs::path fn);
    auto getAudioFilename() const -> fs::path const&;

    virtual bool intersects(double x, double y, double halfSize) const = 0;
    virtual bool intersects(double x, double y, double halfSize, double* gap) const = 0;

protected:
    void serialize(ObjectOutputStream& out) const override;
    void readSerialized(ObjectInputStream& in) override;

    void cloneAudioData(const AudioElement* other);

private:
    // Stroke timestamp, to match it to the audio stream
    size_t timestamp = 0;
    fs::path audioFilename{};
};
