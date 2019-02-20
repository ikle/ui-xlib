/*
 * User Interface Event map
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef UI_EVMAP_H
#define UI_EVMAP_H  1

#include <X11/Xlib.h>
#include <data/ht.h>

#include "ui.h"

struct ui_evmap {
	struct ht ht;
};

int  ui_evmap_init (struct ui_evmap *o);
void ui_evmap_fini (struct ui_evmap *o);

int ui_evmap_insert (struct ui_evmap *o, struct ui_window *w,
		     const char *event, ui_handler *fn, void *cookie);

int ui_evmap_process (struct ui_evmap *o, Window w,
		      const char *event, const void *arg);

void ui_evmap_purge (struct ui_evmap *o, Window w);

#endif  /* UI_EVMAP_H */
