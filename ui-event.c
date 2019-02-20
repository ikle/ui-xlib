/*
 * User Interface Events
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <err.h>
#include <stdio.h>

#include "ui-core.h"
#include "ui-event.h"

#define EVENT_MAX  32

static int ui_key_process (struct ui_session *o, XEvent *ev)
{
	XKeyEvent *e = &ev->xkey;
	const char *group, *name;
	char event[EVENT_MAX];
	int count;

	switch (e->type) {
	case KeyPress:		group = "key-on"; 	break;
	case KeyRelease:	group = "key-off";	break;
	default:		return 0;
	}

	if ((name = ui_keymap_lookup (&o->keymap, e->keycode)) == NULL)
		return 0;

	count = snprintf (event, sizeof (event), "%s %s%s%s%s", group,
			  (e->state & ControlMask) != 0 ? "C-" : "",
			  (e->state & Mod1Mask)    != 0 ? "M-" : "",
			  (e->state & ShiftMask)   != 0 ? "S-" : "",
			  name);
	if (count >= sizeof (event)) {
		warnx ("event buffer overflow, requested %d bytes", count);
		return 0;
	}

	return ui_evmap_process (&o->evmap, e->window, event, NULL);
}

int ui_event_process (struct ui_session *o, XEvent *e)
{
	int ret;
	
	if ((ret = ui_key_process (o, e)) != 0)
		return ret;

	return 0;
}
