/*
 * User Interface Events
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef UI_EVENT_H
#define UI_EVENT_H  1

#include <X11/Xlib.h>

int ui_event_process (struct ui_session *o, XEvent *e);

#endif  /* UI_EVENT_H */
