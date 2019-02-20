/*
 * User Interface Window
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <err.h>
#include <stdlib.h>

#include "ui-core.h"

static const long mask =
	KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
	EnterWindowMask | LeaveWindowMask | PointerMotionMask |
	ExposureMask | StructureNotifyMask | FocusChangeMask;

struct ui_window *
ui_window_create (struct ui_session *s, struct ui_window *parent,
		  int x, int y, int width, int height)
{
	struct ui_window *o;
	Window parent_id = parent == NULL ? DefaultRootWindow (s->d) :
					    parent->id;

	if ((o = malloc (sizeof (*o))) == NULL)
		goto no_object;

	o->id = XCreateSimpleWindow (s->d, parent_id, x, y, width, height,
				     0, 0, 0);
	if (o->id == None)
		goto no_window;

	XSelectInput (s->d, o->id, mask);
	XMapWindow (s->d, o->id); XFlush (s->d);  /* NOTE: temporary! */
	ui_session_ref (s);
	o->s = s;
	warnx ("window %p (%lx) created", o, o->id);
	return o;
no_window:
	free (o);
no_object:
	return NULL;
}

void ui_window_destroy (struct ui_window *o)
{
	if (o == NULL)
		return;

	ui_evmap_purge (&o->s->evmap, o->id);
	XDestroyWindow (o->s->d, o->id);
	ui_session_unref (o->s);
	warnx ("window %p (%lx) destroyed", o, o->id);
	free (o);
}
