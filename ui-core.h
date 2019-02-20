/*
 * User Interface Core
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef UI_CORE_H
#define UI_CORE_H  1

#include <X11/Xlib.h>
#include <data/ht.h>

#include "ui.h"
#include "ui-keymap.h"
#include "ui-evmap.h"

struct ui_session {
	size_t ref;
	Display *d;
	struct ui_keymap keymap;
	struct ui_evmap  evmap;
};

struct ui_window {
	struct ui_session *s;
	Window id;
};

#endif  /* UI_CORE_H */
