/* Hamza Mughal's patch of dwm */
#include <X11/XF86keysym.h>

static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */

static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */

static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

static const int horizpadbar        = 5;        /* horizontal padding for statusbar */
static const int vertpadbar         = 10;        /* vertical padding for statusbar */
static const int vertpad            = 0;       /* vertical padding of bar */
static const int sidepad            = 0;       /* horizontal padding of bar */

static const char *fonts[]          = { "Jetbrains Mono:size=11", "Joypixels:size=11", "Amiri:size=11", "Font Awesome:size=11" };
static const char dmenufont[]       = "SF UI Display:size=11";

static const char col_gray1[]       = "#282c34";
static const char col_gray2[]       = "#343a45";
static const char col_gray3[]       = "#d7dce2";
static const char col_gray4[]       = "#ffd580";
static const char col_cyan[]        = "#171c28";

static const unsigned int baralpha = 0xff;
static const unsigned int borderalpha = OPAQUE;

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           -1 },
	{ "Telegram",     NULL,       NULL,       1 << 8,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.52; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",    tile },
	{ "><>",    NULL },
	{ "[M]",    monocle },
	{ "|M|",    centeredmaster },
	{ ">M>",    centeredfloatingmaster },
	{ "HHH",    grid },
 	{ "[@]",    spiral },
	{ "[\\]",   dwindle },
	{ "TTT",    bstack },
	{ "===",    bstackhoriz },
	{ "H[]",    deck },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

static const char *dmenucmd[] = { "dmenu_run", "-l", "10", "-g", "8", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *browsercmd[]  = { "brave", NULL };
static const char *privatebrowsercmd[]  = { "brave", "--incognito", NULL };
static const char *filemanagercmd[]  = { "alacritty", "-e", "ranger", NULL };
static const char *mailmanagercmd[]  = { "alacritty", "-e", "neomutt", NULL };

/* commands spawned when clicking statusbar, the mouse button pressed is exported as BUTTON */
static char *statuscmds[] = { "notify-send Mouse$BUTTON" };
static char *statuscmd[] = { "/bin/sh", "-c", NULL, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */

	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },

    /* application shortcuts */
	{ MODKEY,                       XK_slash,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
	{ MODKEY|ControlMask,           XK_b,      spawn,          {.v = privatebrowsercmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = filemanagercmd } },
	{ MODKEY,                       XK_grave,  spawn,          {.v = mailmanagercmd } },
	{ MODKEY|ShiftMask,             XK_x,      spawn,          SHCMD("prompt 'Do you really want to shutdown?' 'poweroff'") },

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.02} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.02} },

	/* gaps */
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },

	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod1Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod1Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */

	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ Mod1Mask,                       XK_Tab,    view,           {0} },

	/* { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /1* tile *1/ */
	/* { MODKEY|ShiftMask,             XK_n,      setlayout,      {.v = &layouts[1]} }, /1* null *1/ */
	/* { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} }, /1* monocle *1/ */
	/* { MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} }, /1* centered master *1/ */
	/* { MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} }, /1* center floating *1/ */
	/* { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[5]} }, /1* grid *1/ */
	/* { MODKEY,                       XK_r,      setlayout,      {.v = &layouts[6]} }, /1* spiral *1/ */
	/* { MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[7]} }, /1* dwindle *1/ */
	/* { MODKEY,                       XK_s,      setlayout,      {.v = &layouts[8]} }, /1* bottom stack *1/ */
	/* { MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[9]} }, /1* bottom stack horizontal *1/ */
	/* { MODKEY,                       XK_a,      setlayout,      {.v = &layouts[10]} }, /1* deck *1/ */

	/* layouts */
	{ MODKEY|ControlMask,           XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	/* tagkeys */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

    /* brightness control */
    { 0, XF86XK_MonBrightnessUp,    spawn,     SHCMD("xbacklight -inc 5 && pkill -RTMIN+11 dwmblocks") },
    { 0, XF86XK_MonBrightnessDown,  spawn,     SHCMD("xbacklight -dec 5 && pkill -RTMIN+11 dwmblocks") },

    /* audio control */
    { 0, XF86XK_AudioLowerVolume,   spawn,     SHCMD("pactl set-sink-volume 0 -5% && pkill -RTMIN+10 dwmblocks") },
    { 0, XF86XK_AudioRaiseVolume,   spawn,     SHCMD("pactl set-sink-volume 0 +5% && pkill -RTMIN+10 dwmblocks") },
    { 0, XF86XK_AudioMute,          spawn,     SHCMD("pactl set-sink-mute 0 toggle && pkill -RTMIN+10 dwmblocks") },

    /* screenshots */
    { 0, XK_Print,                  spawn,     SHCMD("screenshot") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = statuscmd } },
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = statuscmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
