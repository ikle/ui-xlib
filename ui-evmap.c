/*
 * User Interface Event map
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <err.h>
#include <stdlib.h>
#include <string.h>

#include <data/hash.h>

#include "ui-core.h"
#include "ui-evmap.h"

struct event {
	struct ui_window *w;
	const char *name;
	ui_handler *fn;
	void *cookie;
};

static struct event *event_alloc (struct ui_window *w, const char *name,
				  ui_handler *fn, void *cookie)
{
	struct event *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	o->w      = w;
	o->name   = name;
	o->fn     = fn;
	o->cookie = cookie;
	return o;
}

static int event_eq (const void *a, const void *b)
{
	const struct event *p = a;
	const struct event *q = b;

	return p->w->id == q->w->id && strcmp (p->name, q->name) == 0;
}

static size_t event_hash (const void *o)
{
	const struct event *p = o;
	size_t h;

	h = hash (0, &p->w->id, sizeof (p->w->id));
	return hash (h, p->name, strlen (p->name));
}

static const struct data_type event_type = {
	.free	= free,
	.eq	= event_eq,
	.hash	= event_hash,
};

int ui_evmap_init (struct ui_evmap *o)
{
	return ht_init (&o->ht, &event_type);
}

void ui_evmap_fini (struct ui_evmap *o)
{
	ht_fini (&o->ht);
}

int ui_evmap_insert (struct ui_evmap *o, struct ui_window *w,
		     const char *event, ui_handler *fn, void *cookie)
{
	struct event *e;

	if ((e = event_alloc (w, event, fn, cookie)) == NULL)
		goto no_event;

	if (!ht_insert (&o->ht, e, 1))
		goto no_insert;

	warnx ("mapped %p for %lx, %s", fn, w->id, event);
	return 1;
no_insert:
	event_type.free (e);
no_event:
	return 0;
}

int ui_evmap_process (struct ui_evmap *o, Window w,
		      const char *event, const void *arg)
{
	struct ui_window fw = { NULL, w };
	struct event fe = { &fw, event }, *e;

	warnx ("%lx: process %s", w, event);

	if ((e = ht_lookup (&o->ht, &fe)) == NULL)
		return 0;

	warnx ("handler %p found", e->fn);

	return e->fn (e->w, arg, e->cookie);
}

void ui_evmap_purge (struct ui_evmap *o, Window w)
{
	size_t i;
	struct event *e;

	ht_foreach (i, e, &o->ht)
		if (e->w->id == w) {
			warnx ("unmaped %p from %lx, %s", e->fn, w, e->name);
			o->ht.table[i] = NULL;
		}
}
