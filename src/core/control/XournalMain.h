/*
 * Xournal++
 *
 * Xournal main entry, commandline parser
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <config.h>
#include <glib.h>

#include "filesystem.h"

class GladeSearchpath;
class Control;

namespace XournalMain {
auto run(int argc, char** argv) -> int;
}
