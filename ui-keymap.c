/*
 * User Interface Keymap
 *
 * Copyright (c) 2019 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <errno.h>
#include <stdlib.h>
#include <X11/keysym.h>
#include <data/hash.h>

#include "ui-keymap.h"

struct keypair {
	unsigned long key;
	const char *name;
};

static const struct keypair keymap[] = {
	{ XK_BackSpace,		"backspace"	},
	{ XK_Tab,		"tab"		},
	{ XK_Return,		"enter"		},
	{ XK_Escape,		"escape"	},
	{ XK_space,		"space"		},
	{ XK_Up,		"up"		},
	{ XK_Down,		"down"		},
	{ XK_Left,		"left"		},
	{ XK_Right,		"right"		},
	{ XK_Home,		"home"		},
	{ XK_End,		"end"		},
	{ XK_Prior,		"pg-up"		},
	{ XK_Next,		"pg-down"	},
	{ XK_Insert,		"insert"	},
	{ XK_Delete,		"delete"	},

	{ XK_0,			"0"		},
	{ XK_1,			"1"		},
	{ XK_2,			"2"		},
	{ XK_3,			"3"		},
	{ XK_4,			"4"		},
	{ XK_5,			"5"		},
	{ XK_6,			"6"		},
	{ XK_7,			"7"		},
	{ XK_8,			"8"		},
	{ XK_9,			"9"		},

	{ XK_a,			"a"		},
	{ XK_b,			"b"		},
	{ XK_c,			"c"		},
	{ XK_d,			"d"		},
	{ XK_e,			"e"		},
	{ XK_f,			"f"		},
	{ XK_g,			"g"		},
	{ XK_h,			"h"		},
	{ XK_i,			"i"		},
	{ XK_j,			"j"		},
	{ XK_k,			"k"		},
	{ XK_l,			"l"		},
	{ XK_m,			"m"		},
	{ XK_n,			"n"		},
	{ XK_o,			"o"		},
	{ XK_p,			"p"		},
	{ XK_q,			"q"		},
	{ XK_r,			"r"		},
	{ XK_s,			"s"		},
	{ XK_t,			"t"		},
	{ XK_u,			"u"		},
	{ XK_v,			"v"		},
	{ XK_w,			"w"		},
	{ XK_x,			"x"		},
	{ XK_y,			"y"		},
	{ XK_z,			"z"		},

	{ XK_F1,		"F1"		},
	{ XK_F2,		"F2"		},
	{ XK_F3,		"F3"		},
	{ XK_F4,		"F4"		},
	{ XK_F5,		"F5"		},
	{ XK_F6,		"F6"		},
	{ XK_F7,		"F7"		},
	{ XK_F8,		"F8"		},
	{ XK_F9,		"F9"		},
	{ XK_F10,		"F10"		},
	{ XK_F11,		"F11"		},
	{ XK_F11,		"F12"		},

	{ XK_KP_Tab,		"tab"		},
	{ XK_KP_Enter,		"enter"		},
	{ XK_KP_Space,		"space"		},
	{ XK_KP_Up,		"up"		},
	{ XK_KP_Down,		"down"		},
	{ XK_KP_Left,		"left"		},
	{ XK_KP_Right,		"right"		},
	{ XK_KP_Home,		"home"		},
	{ XK_KP_End,		"end"		},
	{ XK_KP_Prior,		"pg-up"		},
	{ XK_KP_Next,		"pg-down"	},
	{ XK_KP_Insert,		"insert"	},
	{ XK_KP_Delete,		"delete"	},

	{ XK_KP_0,		"0"		},
	{ XK_KP_1,		"1"		},
	{ XK_KP_2,		"2"		},
	{ XK_KP_3,		"3"		},
	{ XK_KP_4,		"4"		},
	{ XK_KP_5,		"5"		},
	{ XK_KP_6,		"6"		},
	{ XK_KP_7,		"7"		},
	{ XK_KP_8,		"8"		},
	{ XK_KP_9,		"9"		},

	{},
};

static struct keypair *keypair_alloc (unsigned long key, const char *name)
{
	struct keypair *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	o->key  = key;
	o->name = name;
	return o;
}

static int keypair_eq (const void *a, const void *b)
{
	const struct keypair *p = a;
	const struct keypair *q = b;

	return p->key == q->key;
}

static size_t keypair_hash (const void *o)
{
	const struct keypair *p = o;

	return hash (0, &p->key, sizeof (p->key));
}

static const struct data_type keypair_type = {
	.free	= free,
	.eq	= keypair_eq,
	.hash	= keypair_hash,
};

int ui_keymap_init (struct ui_keymap *o, Display *d)
{
	const struct keypair *p;
	unsigned keycode;
	struct keypair *q;

	if (!ht_init (&o->ht, &keypair_type))
		return 0;

	for (p = keymap; p->key != 0; ++p)
		if ((keycode = XKeysymToKeycode (d, p->key)) != 0) {
			if ((q = keypair_alloc (keycode, p->name)) == NULL)
				goto no_keypair;

			if (!ht_insert (&o->ht, q, 0)) {
				keypair_type.free (q);

				if (errno != EEXIST)
					goto no_insert;
			}
		}

	return 1;
no_insert:
no_keypair:
	ht_fini (&o->ht);
	return 0;
}

void ui_keymap_fini (struct ui_keymap *o)
{
	ht_fini (&o->ht);
}

const char *ui_keymap_lookup (struct ui_keymap *o, unsigned keycode)
{
	const struct keypair *p;
	struct keypair fake = { keycode, NULL };

	if ((p = ht_lookup (&o->ht, &fake)) == NULL)
		return NULL;

	return p->name;
}
