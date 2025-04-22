/* See LICENSE file for copyright and license details. */

#include <X11/X.h>
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 0;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systrayiconsize = 12;
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 1;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray         = 1;        /* 0 means no systray */
static const int showbar             = 1;        /* 0 means no bar */
static const int topbar              = 1;        /* 0 means bottom bar */
static const int horizpadbar         = 16;       /* horizontal padding for statusbar */
static const int vertpadbar          = 32;       /* vertical padding for statusbar */
static const char *fonts[]           = { "monospace:size=11:antialias=true:hinting=true:style=Bold", "Symbols Nerd Font Mono:size=11:style=Regular:antialias=true:hinting=true" };
static const char dmenuvertpad[]     = "16";
static const char dmenulines[]       = "4";
static char normbgcolor[]            = "#222222";
static char normbordercolor[]        = "#444444";
static char normfgcolor[]            = "#bbbbbb";
static char selfgcolor[]             = "#eeeeee";
static char selbordercolor[]         = "#005577";
static char selbgcolor[]             = "#005577";
static char dormfgcolor[]            = "#444444";
static char dormbordercolor[]        = "#222222";
static char dormbgcolor[]            = "#222222";
static char termcol0[] = "#000000"; /* black   */
static char termcol1[] = "#ff0000"; /* red     */
static char termcol2[] = "#33ff00"; /* green   */
static char termcol3[] = "#ff0099"; /* yellow  */
static char termcol4[] = "#0066ff"; /* blue    */
static char termcol5[] = "#cc00ff"; /* magenta */
static char termcol6[] = "#00ffff"; /* cyan    */
static char termcol7[] = "#d0d0d0"; /* white   */
static char termcol8[]  = "#808080"; /* black   */
static char termcol9[]  = "#ff0000"; /* red     */
static char termcol10[] = "#33ff00"; /* green   */
static char termcol11[] = "#ff0099"; /* yellow  */
static char termcol12[] = "#0066ff"; /* blue    */
static char termcol13[] = "#cc00ff"; /* magenta */
static char termcol14[] = "#00ffff"; /* cyan    */
static char termcol15[] = "#ffffff"; /* white   */
static char *termcolor[] = {
  termcol0,
  termcol1,
  termcol2,
  termcol3,
  termcol4,
  termcol5,
  termcol6,
  termcol7,
  termcol8,
  termcol9,
  termcol10,
  termcol11,
  termcol12,
  termcol13,
  termcol14,
  termcol15,
};

static char *colors[][3] = {
  /*               fg           bg           border   */
  [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
  [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
  [SchemeDorm]  = { dormfgcolor,  dormbgcolor,  dormbordercolor  },
};

/* tagging */
static const char *tags[] = {
  "火",
  "木",
  "土",
  "金",
};

static const Rule rules[] = {
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL,       NULL,       NULL,       0,            False,       -1 },
  { "imv",      NULL,       NULL,       1,            True,        -1 }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "ロ",      tile },    /* first entry is default */
	{ "フ",      NULL },    /* no layout function means floating behavior */
	{ "ス",      monocle },
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
static const char *dmenucmd[] = { "dmenu_run", "-p", " ", "-vp", dmenuvertpad, "-c", "-l", dmenulines, NULL };

static const Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY,                       XK_space,  spawn,          SHCMD("$HOME/.local/share/desktop-scripts/dmenu-launch") },
  { MODKEY,                       XK_Escape, spawn,          SHCMD("$HOME/.local/share/desktop-scripts/dmenu-power") },
  { 0,                            XK_Print,  spawn,          SHCMD("$HOME/.local/share/desktop-scripts/dmenu-screenshot") },
  { MODKEY,                       XK_p,      spawn,          SHCMD("$HOME/.local/share/desktop-scripts/dmenu-screenshot") },
  { MODKEY|ShiftMask,             XK_space,  spawn,          SHCMD("$HOME/.local/share/desktop-scripts/dmenu-run") },
  { MODKEY,                       XK_Return, spawn,          SHCMD("$TERMINAL") },
  { MODKEY,                       XK_e,      spawn,          SHCMD("$VISUAL") },
  { MODKEY,                       XK_b,      spawn,          SHCMD("$BROWSER") },
  { MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
  { MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
  { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
  { MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
  { MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
  { MODKEY,                       XK_Tab,    view,           {0} },
  { MODKEY,                       XK_q,      killclient,     {0} },
  { MODKEY|Mod1Mask,              XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY|Mod1Mask,              XK_f,      setlayout,      {.v = &layouts[1]} },
  { MODKEY|Mod1Mask,              XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_f,      togglefullscr,  {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
  { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY|ShiftMask,             XK_c,      quit,           {1} },
  { MODKEY|ControlMask|ShiftMask, XK_q,      quit,           {0} },
  { 0,                            XF86XK_AudioMute,          spawn, SHCMD("$HOME/.local/share/desktop-scripts/volumectl mute")    },
  { 0,                            XF86XK_AudioMicMute,       spawn, SHCMD("$HOME/.local/share/desktop-scripts/microphone mute")    },
  { 0,                            XF86XK_AudioLowerVolume,   spawn, SHCMD("$HOME/.local/share/desktop-scripts/volumectl down")    },
  { 0,                            XF86XK_AudioRaiseVolume,   spawn, SHCMD("$HOME/.local/share/desktop-scripts/volumectl up")      },
  { 0,                            XF86XK_MonBrightnessDown,  spawn, SHCMD("$HOME/.local/share/desktop-scripts/backlightctl down") },
  { 0,                            XF86XK_MonBrightnessUp,    spawn, SHCMD("$HOME/.local/share/desktop-scripts/backlightctl up")   },
  { MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkStatusText,        0,              Button2,        spawn,          SHCMD("$TERMINAL") },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
