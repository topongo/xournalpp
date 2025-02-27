#include "control/DeviceListHelper.h"

#include <algorithm>
#include <utility>
#include <vector>

#include "util/GListView.h"
#include "util/i18n.h"


void storeNewUnlistedDevice(std::vector<InputDevice>& deviceList, GdkDevice* device) {
    // This could potentially be problematic with systems having a multitude of input devices as it searches linearily
    auto it = std::find(deviceList.begin(), deviceList.end(), InputDevice(device));
    if (it != deviceList.end()) {
        // Device is already known but source may be unknown
        it->updateType(gdk_device_get_source(device));
        return;
    }

    deviceList.emplace_back(device);
}

void addDevicesToList(std::vector<InputDevice>& deviceList, GListView<GdkDevice> devView, bool ignoreTouchDevices) {
    for (GdkDevice& dev: devView) {
        if (GDK_SOURCE_KEYBOARD == gdk_device_get_source(&dev)) {
            continue;
        }
        if (gdk_device_get_vendor_id(&dev) == nullptr && gdk_device_get_product_id(&dev) == nullptr) {
            continue;
        }
        if (ignoreTouchDevices && GDK_SOURCE_TOUCHSCREEN == gdk_device_get_source(&dev)) {
            continue;
        }
        storeNewUnlistedDevice(deviceList, &dev);
    }
}

auto DeviceListHelper::getDeviceList(Settings* settings, bool ignoreTouchDevices) -> std::vector<InputDevice> {
    std::vector<InputDevice> deviceList = settings->getKnownInputDevices();

    if (ignoreTouchDevices) {
        deviceList.erase(
                std::remove_if(deviceList.begin(), deviceList.end(),
                               [](const InputDevice& device) { return device.getSource() == GDK_SOURCE_TOUCHSCREEN; }),
                deviceList.end());
    }

    GdkDisplay* display = gdk_display_get_default();
    GdkSeat* defaultSeat = gdk_display_get_default_seat(display);
    GdkDevice* pointer = gdk_seat_get_pointer(defaultSeat);
    GdkSeat* pointerSeat = gdk_device_get_seat(pointer);
    auto* pointerSlaves = gdk_seat_get_slaves(pointerSeat, GDK_SEAT_CAPABILITY_ALL_POINTING);
    addDevicesToList(deviceList, pointerSlaves, ignoreTouchDevices);
    g_list_free(pointerSlaves);

    if (deviceList.empty()) {
        g_warning("No device found. Is Xournal++ running in debugger / Eclipse...?\n"
                  "Probably this is the reason for not finding devices!\n");
    }

    return deviceList;
}

InputDeviceClass DeviceListHelper::getSourceMapping(GdkInputSource source, Settings* settings) {
    auto deviceList = DeviceListHelper::getDeviceList(settings);

    for (InputDevice const& inputDevice: deviceList) {
        InputDeviceClass deviceClass =
                InputEvents::translateDeviceType(inputDevice.getName(), inputDevice.getSource(), settings);

        if (inputDevice.getSource() == source) {
            return deviceClass;
        }
    }

    return InputDeviceClass::INPUT_DEVICE_IGNORE;
}

InputDevice::InputDevice(GdkDevice* device): name(gdk_device_get_name(device)), source(gdk_device_get_source(device)) {}

InputDevice::InputDevice(std::string name, GdkInputSource source): name(std::move(name)), source(source) {}

auto InputDevice::getName() const -> std::string { return this->name; }

auto InputDevice::getSource() const -> GdkInputSource { return this->source; }

void InputDevice::updateType(GdkInputSource newSource) { this->source = newSource; }

auto InputDevice::getType() const -> std::string {
    switch (source) {
        case GDK_SOURCE_MOUSE:
            return _("mouse");
        case GDK_SOURCE_PEN:
            return _("pen");
        case GDK_SOURCE_ERASER:
            return _("eraser");
        case GDK_SOURCE_CURSOR:
            return _("cursor");
        case GDK_SOURCE_KEYBOARD:
            // not used: filtered above
            return _("keyboard");
        case GDK_SOURCE_TOUCHSCREEN:
            return _("touchscreen");
        case GDK_SOURCE_TOUCHPAD:
            return _("touchpad");
#if (GDK_MAJOR_VERSION >= 3 && GDK_MINOR_VERSION >= 22)
        case GDK_SOURCE_TRACKPOINT:
            return _("trackpoint");
        case GDK_SOURCE_TABLET_PAD:
            return _("tablet pad");
#endif
    }

    return "";
}

auto InputDevice::operator==(const InputDevice& inputDevice) const -> bool {
    return this->getName() == inputDevice.getName();
}
