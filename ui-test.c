/*
 * User Interface library test
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <err.h>
#include <locale.h>
#include <stdio.h>

#include "ui.h"

static int on_up (struct ui_window *w, const void *arg, void *cookie)
{
	warnx ("key up pressed");
	return 1;
}

static int on_down (struct ui_window *w, const void *arg, void *cookie)
{
	warnx ("key down pressed");
	return 1;
}

static int on_escape (struct ui_window *w, const void *arg, void *cookie)
{
	warnx ("key escape pressed");
	return -1;
}

static struct ui_type test[] = {
	{ "key-on up",		on_up		},
	{ "key-on down",	on_down		},
	{ "key-on escape",	on_escape	},
	{ },
};

int main (int argc, char *argv[])
{
	struct ui_session *s;
	struct ui_window *w;

	setlocale (LC_ALL, "");

	if ((s = ui_session_get (NULL)) == NULL)
		errx (1, "cannot open session");

	if ((w = ui_window_create (s, NULL, 0, 0, 320, 240)) == NULL)
		errx (1, "cannot create window");

	if (!ui_binds (w, test, NULL))
		errx (1, "cannot bind to window");

	ui_run (s);

	ui_session_unref (s);
	ui_window_destroy (w);
	return 0;
}
