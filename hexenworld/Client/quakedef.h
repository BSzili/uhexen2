/*
	quakedef.h
	primary header for client

	$Header: /home/ozzie/Download/0000/uhexen2/hexenworld/Client/quakedef.h,v 1.25 2006-07-17 14:48:41 sezero Exp $
*/

#include <sys/types.h>
#include <math.h>
#include <string.h>
#if !defined(_WIN32)
// for strcasecmp and strncasecmp
#include <strings.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "bothdefs.h"

#include "common.h"
#include "bspfile.h"
#include "sys.h"
#include "zone.h"
#include "mathlib.h"
#include "cvar.h"

#include "protocol.h"
#include "net.h"

#include "cmd.h"
#include "crc.h"

#include "console.h"
#include "vid.h"
#include "wad.h"
#include "draw.h"
#include "render.h"
#include "view.h"
#include "screen.h"
#include "sbar.h"
#include "sound.h"
#include "cdaudio.h"
#include "mididef.h"

#include "../Server/pr_comp.h"			// defs shared with qcc
#include "../Server/progdefs.h"			// generated by program cdefs
#include "pr_strng.h"
#include "cl_effect.h"
#include "client.h"

#ifdef GLQUAKE
// gl_model.h is included from within glquake.h
#include "glquake.h"
#else
#include "model.h"
#include "d_iface.h"
#endif
#include "pmove.h"

#include "input.h"
#include "keys.h"
#include "menu.h"

//=============================================================================

// the host system specifies the base of the directory tree, the
// command line parms passed to the program, and the amount of memory
// available for the program to use

typedef struct
{
	char	*basedir;
	char	*userdir;		// userspace directory on UNIX platforms
	int	argc;
	char	**argv;
	void	*membase;
	int	memsize;
} quakeparms_t;


//=============================================================================

//
// host
//
extern	quakeparms_t	host_parms;

extern	cvar_t		sys_ticrate;
extern	cvar_t		sys_nostdout;
extern	cvar_t		developer;

extern	cvar_t		password;
extern	cvar_t		talksounds;

extern	qboolean	host_initialized;	// true if into command execution
extern	double		host_frametime;
extern	byte		*host_basepal;
extern	byte		*host_colormap;
extern	int		host_framecount;	// incremented every frame, never reset
extern	double		realtime;		// not bounded in any way, changed at
						// start of every frame, never reset

void Host_InitCommands (void);
void Host_Init (quakeparms_t *parms);
void Host_Shutdown(void);
void Host_Error (char *error, ...);
void Host_EndGame (char *message, ...);
void Host_Frame (float time);
void Host_Quit_f (void);
void Host_ClientCommands (char *fmt, ...);
void Host_ShutdownServer (qboolean crash);
void Host_WriteConfiguration (char *fname);

extern	qboolean	msg_suppress_1;	// suppresses resolution and cache size console output
					//  a fullscreen DIB focus gain/loss

extern	qboolean	noclip_anglehack;

extern	unsigned int	defLosses;	// Defenders losses in Siege
extern	unsigned int	attLosses;	// Attackers Losses in Siege

extern	int		cl_keyholder;
extern	int		cl_doc;

