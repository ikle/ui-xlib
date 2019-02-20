/*
 * User Interface Keymap
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef UI_KEYMAP_H
#define UI_KEYMAP_H  1

#include <X11/Xlib.h>
#include <data/ht.h>

#include "ui.h"

struct ui_keymap {
	struct ht ht;
};

int ui_keymap_init (struct ui_keymap *o, Display *d);
void ui_keymap_fini (struct ui_keymap *o);

const char *ui_keymap_lookup (struct ui_keymap *o, unsigned keycode);

#endif  /* UI_KEYMAP_H */
