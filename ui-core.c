/*
 * User Interface helper library
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <err.h>
#include <errno.h>
#include <stdlib.h>

#include "ui-core.h"
#include "ui-event.h"

struct ui_session *ui_session_get (const char *server)
{
	struct ui_session *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		goto no_object;

	if ((o->d = XOpenDisplay (server)) == NULL)
		goto no_display;

	if (!ui_keymap_init (&o->keymap, o->d))
		goto no_keymap;

	if (!ui_evmap_init (&o->evmap))
		goto no_evmap;

	o->ref = 1;
	warnx ("session %p opened", o);
	return o;
no_evmap:
	ui_keymap_fini (&o->keymap);
no_keymap:
	XCloseDisplay (o->d);
no_display:
	free (o);
no_object:
	return NULL;
}

void ui_session_ref (struct ui_session *o)
{
	++o->ref;
}

void ui_session_unref (struct ui_session *o)
{
	if (o == NULL || --o->ref > 0)
		return;

	warnx ("session %p closed", o);

	ui_evmap_fini (&o->evmap);
	ui_keymap_fini (&o->keymap);
	XCloseDisplay (o->d);
	free (o);
}

int ui_bind (struct ui_window *w, const char *event,
	     ui_handler *fn, void *cookie)
{
	if (w == NULL || event == NULL || fn == NULL) {
		errno = EINVAL;
		return 0;
	}

	return ui_evmap_insert (&w->s->evmap, w, event, fn, cookie);
}

int ui_binds (struct ui_window *w, const struct ui_type *type, void *cookie)
{
	for (; type->event != NULL; ++type)
		if (!ui_bind (w, type->event, type->fn, cookie))
			return 0;

	return 1;
}

int ui_run (struct ui_session *o)
{
	XEvent e;

	for (;;) {
		XNextEvent (o->d, &e);

		if (ui_event_process (o, &e) < 0)
			break;
	}

	return 1;
}
