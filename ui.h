/*
 * User Interface helper library
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef UI_H
#define UI_H  1

struct ui_session *ui_session_get (const char *server);
void ui_session_ref (struct ui_session *o);
void ui_session_unref (struct ui_session *o);

struct ui_window *
ui_window_create (struct ui_session *s, struct ui_window *parent,
		  int x, int y, int width, int height);
void ui_window_destroy (struct ui_window *o);

typedef int ui_handler (struct ui_window *w, const void *arg, void *cookie);

int ui_bind (struct ui_window *w, const char *event,
	     ui_handler *fn, void *cookie);

struct ui_type {
	const char *event;
	ui_handler *fn;
};

int ui_binds (struct ui_window *w, const struct ui_type *type, void *cookie);

int ui_run (struct ui_session *o);

#endif  /* UI_H */
