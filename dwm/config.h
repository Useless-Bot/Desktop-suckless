/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx	    = 2;        /* border pixel of windows */
static unsigned int snap	    = 32;       /* snap pixel */
static const unsigned int gappih    = 5;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 15;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar		    = 1;        /* 0 means no bar */
static int topbar		    = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {"Iosevka:size=11" };
static const char dmenufont[]       = "Iosevka:size=11";
static char normbgcolor[]           = "#4a5463";
static char normbordercolor[]       = "#99ccff";
static char normfgcolor[]           = "#ffffff";
static char selfgcolor[]            = "#e7e7e7";
static char selbordercolor[]        = "#88aadd";
static char selbgcolor[]            = "#d67540";	/* Colored bar */
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 9,	    0,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol	arrange function */
	{ "[\\]",	dwindle }, /* first entry is default */ 
	{ "[@]",	spiral },
	{ "[]=",	tile },    /* first entry is default */
	{ "[M]",	monocle },
	{ "[@]",	spiral },
	{ "[\\]",	dwindle },
	{ "H[]",	deck },
	{ "TTT",	bstack },
	{ "===",	bstackhoriz },
	{ "HHH",	grid },
	{ "###",	nrowgrid },
	{ "---",	horizgrid },
	{ ":::",	gaplessgrid },
	{ "|M|",	centeredmaster },
	{ ">M>",	centeredfloatingmaster },
	{ "><>",	NULL },    /* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *webbrowser[] = { "firefox", NULL };
static const char *discord[] = { "discord", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	FLOAT,   &mfact },
};

#include "shiftview.c"

static Key keys[] = {
	/* modifier	                  key        	function        argument */
	{ MODKEY,			XK_d,      	spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,		XK_d,		spawn,		{.v = discord } } ,
	{ MODKEY,			XK_Return, 	spawn,          {.v = termcmd } },
	{ MODKEY,			XK_w,		spawn,		{.v = webbrowser } },
	{ MODKEY,                       XK_quoteleft,	togglebar,      {0} },
	{ MODKEY,                       XK_k,      	focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_j,      	focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,		XK_equal,      	incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_minus,      	incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      	setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      	setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, 	zoom,           {0} },
	{ MODKEY|Mod4Mask,              XK_u,      	incrgaps,       {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_u,      	incrgaps,       {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_i,      	incrigaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_i,      	incrigaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_o,      	incrogaps,      {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_o,      	incrogaps,      {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_p,      	incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_p,      	incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_bracketleft, incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_bracketleft, incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_bracketright,incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_bracketright,incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_backslash,   incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_backslash,   incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod4Mask,              XK_y,      	togglegaps,     {0} },
	{ MODKEY|Mod4Mask|ShiftMask,    XK_y,      	defaultgaps,    {0} },
	{ MODKEY,			XK_b,		shiftview,	{.i = -1 } }, 
	{ MODKEY,			XK_n,		shiftview,	{.i = +1 } },	
	{ MODKEY,                       XK_Tab,    	view,           {0} },
	{ MODKEY|ShiftMask,		XK_q,     	killclient,     {0} },
	{ MODKEY|Mod4Mask,		XK_1,     	setlayout,      {.v = &layouts[0]} },
	{ MODKEY|Mod4Mask,		XK_2,     	setlayout,      {.v = &layouts[1]} },
	{ MODKEY|Mod4Mask,		XK_3,     	setlayout,      {.v = &layouts[2]} },
	{ MODKEY|Mod4Mask,		XK_4,		setlayout,	{.v = &layouts[3]} },	 
	{ MODKEY|Mod4Mask,		XK_5,		setlayout,	{.v = &layouts[4]} },
	{ MODKEY,                       XK_space, 	setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_f,		togglefloating, {0} },
	{ MODKEY,                       XK_0,     	view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,     	tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma, 	focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,	focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma, 	tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,	tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,           	           0)
	TAGKEYS(                        XK_2,           	           1)
	TAGKEYS(                        XK_3,           	           2)
	TAGKEYS(                        XK_4,           	           3)
	TAGKEYS(                        XK_5,           	           4)
	TAGKEYS(                        XK_6,           	           5)
	TAGKEYS(                        XK_7,           	           6)
	TAGKEYS(                        XK_8,           	           7)
	TAGKEYS(                        XK_9,           	           8)
	{ MODKEY|ShiftMask,             XK_Escape,     	 quit,	           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

