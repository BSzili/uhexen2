/*
	glquake.h
	common glquake header

	$Id: glquake.h,v 1.49 2006-06-03 16:30:20 sezero Exp $
*/


#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
//#include <GL/glu.h>

#ifdef __MORPHOS__
#include <proto/tinygl.h>
#endif

#include "gl_opt.h"
#include "gl_func.h"

#ifndef	GLX_3DFX_WINDOW_MODE_MESA
#define	GLX_3DFX_WINDOW_MODE_MESA		0x1
#endif

#ifndef	GLX_3DFX_FULLSCREEN_MODE_MESA
#define	GLX_3DFX_FULLSCREEN_MODE_MESA		0x2
#endif

#ifndef	GL_TEXTURE0_ARB
#define	GL_TEXTURE0_ARB				0x84C0
#define	GL_TEXTURE1_ARB				0x84C1
#define	GL_TEXTURE2_ARB				0x84C2
#define	GL_TEXTURE3_ARB				0x84C3
#define	GL_TEXTURE4_ARB				0x84C4
#define	GL_TEXTURE5_ARB				0x84C5

#define	GL_ACTIVE_TEXTURE_ARB			0x84E0
#define	GL_CLIENT_ACTIVE_TEXTURE_ARB		0x84E1
#define	GL_MAX_TEXTURE_UNITS_ARB		0x84E2
#endif

#ifndef	GL_MULTISAMPLE_ARB
#define	GL_MULTISAMPLE_ARB			0x809D
#endif

#ifndef	GL_SHARED_TEXTURE_PALETTE_EXT
#define	GL_SHARED_TEXTURE_PALETTE_EXT		0x81FB
#endif

#ifndef GL_COLOR_INDEX8_EXT
#define GL_COLOR_INDEX8_EXT			0x80E5
#endif

// the GL_Bind macro
#define GL_Bind(texnum) {\
	if (currenttexture != (texnum)) {\
		currenttexture = (texnum);\
		glBindTexture_fp(GL_TEXTURE_2D, currenttexture);\
	}\
}

// defs for palettized textures
#define INVERSE_PAL_R_BITS 6
#define INVERSE_PAL_G_BITS 6
#define INVERSE_PAL_B_BITS 6
#define INVERSE_PAL_TOTAL_BITS	( INVERSE_PAL_R_BITS + INVERSE_PAL_G_BITS + INVERSE_PAL_B_BITS )

// misc. common glquake defines
#define MAX_GLTEXTURES		2048
#define MAX_EXTRA_TEXTURES	156   // 255-100+1
#define	MAX_CACHED_PICS		256
#define	MAX_LIGHTMAPS		64

#define	gl_solid_format		3
#define	gl_alpha_format		4

// types for textures

typedef struct
{
	int		texnum;
	float	sl, tl, sh, th;
} glpic_t;

typedef struct cachepic_s
{
	char		name[MAX_QPATH];
	qpic_t		pic;
	byte		padding[32];	// for appended glpic
} cachepic_t;

typedef struct
{
	unsigned int	texnum;
	char	identifier[64];
	int		width, height;
	qboolean	mipmap;
//	unsigned short	crc;
	unsigned long	hash;
} gltexture_t;


extern	int	texture_extension_number;
extern	int	gl_filter_min;
extern	int	gl_filter_max;
extern	float	gldepthmin, gldepthmax;
extern	int	glx, gly, glwidth, glheight;
extern	int	gl_extra_textures[MAX_EXTRA_TEXTURES];   // generic textures for models

void GL_BeginRendering (int *x, int *y, int *width, int *height);
void GL_EndRendering (void);

int GL_LoadTexture (char *identifier, int width, int height, byte *data, qboolean mipmap, qboolean alpha, int mode, qboolean rgba);
int GL_LoadPicTexture (qpic_t *pic);
int M_DrawBigCharacter (int x, int y, int num, int numNext);
void GL_BuildLightmaps (void);
void GL_SetupLightmapFmt (qboolean check_cmdline);
void GL_MakeAliasModelDisplayLists (model_t *m, aliashdr_t *hdr);
void GL_Set2D (void);
void GL_SubdivideSurface (msurface_t *fa);
#ifdef QUAKE2
void R_LoadSkys (void);
void R_DrawSkyBox (void);
void R_ClearSkyBox (void);
#endif
//void EmitSkyPolys (msurface_t *fa);
void EmitWaterPolys (msurface_t *fa);
void EmitBothSkyLayers (msurface_t *fa);
qboolean R_CullBox (vec3_t mins, vec3_t maxs);
void R_DrawBrushModel (entity_t *e, qboolean Translucent);
void R_DrawSkyChain (msurface_t *s);
void R_DrawWorld (void);
void R_DrawWaterSurfaces (void);
void R_RenderBrushPoly (msurface_t *fa, qboolean override);
void R_RenderDlights (void);
void R_RotateForEntity (entity_t *e);
void R_MarkLights (dlight_t *light, int bit, mnode_t *node);
void R_AnimateLight(void);
int R_LightPoint (vec3_t p);
int *R_LightPointColour (vec3_t p);
void R_StoreEfrags (efrag_t **ppefrag);
void R_InitParticles (void);
void R_ClearParticles (void);
void R_DrawParticles (void);


// r_local.h -- private refresh defs

#define ALIAS_BASE_SIZE_RATIO		(1.0 / 11.0)
					// normalizing factor so player model works
					// out to about 1 pixel per triangle
#define	MAX_SKIN_HEIGHT		480

#define TILE_SIZE		128	// size of textures generated by R_GenTiledSurf

#define SKYSHIFT		7
#define	SKYSIZE			(1 << SKYSHIFT)
#define SKYMASK			(SKYSIZE - 1)

#define BACKFACE_EPSILON	0.01


typedef struct surfcache_s
{
	struct surfcache_s	*next;
	struct surfcache_s 	**owner;	// NULL is an empty chunk of memory
	int			lightadj[MAXLIGHTMAPS]; // checked for strobe flush
	int			dlight;
	int			size;		// including header
	unsigned		width;
	unsigned		height;		// DEBUG only needed for debug
	float			mipscale;
	struct texture_s	*texture;	// checked for animating textures
	byte			data[4];	// width*height elements
} surfcache_t;


typedef struct
{
	pixel_t		*surfdat;	// destination for generated surface
	int		rowbytes;	// destination logical width in bytes
	msurface_t	*surf;		// description for surface to generate
	fixed8_t	lightadj[MAXLIGHTMAPS]; // adjust for lightmap levels for dynamic lighting
	texture_t	*texture;	// corrected for animating textures
	int		surfmip;	// mipmapped ratio of surface texels / world pixels
	int		surfwidth;	// in mipmapped texels
	int		surfheight;	// in mipmapped texels
} drawsurf_t;


// Changes to ptype_t must also be made in d_iface.h
typedef enum {
	pt_static,
	pt_grav,
	pt_fastgrav,
	pt_slowgrav,
	pt_fire,
	pt_explode,
	pt_explode2,
	pt_blob,
	pt_blob2,
	pt_rain,
	pt_c_explode,
	pt_c_explode2,
	pt_spit,
	pt_fireball,
	pt_ice,
	pt_spell,
	pt_test,
	pt_quake,
	pt_rd,			// rider's death
	pt_vorpal,
	pt_setstaff,
	pt_magicmissile,
	pt_boneshard,
	pt_scarab,
	pt_acidball,
	pt_darken,
	pt_snow,
	pt_gravwell,
	pt_redfire
} ptype_t;

// Changes to rtype_t must also be made in glquake.h
typedef enum
{
	rt_rocket_trail = 0,
	rt_smoke,
	rt_blood,
	rt_tracer,
	rt_slight_blood,
	rt_tracer2,
	rt_voor_trail,
	rt_fireball,
	rt_ice,
	rt_spit,
	rt_spell,
	rt_vorpal,
	rt_setstaff,
	rt_magicmissile,
	rt_boneshard,
	rt_scarab,
	rt_acidball,
	rt_bloodshot
} rt_type_t;

// if this is changed, it must be changed in d_iface.h too !!!
typedef struct particle_s
{
// driver-usable fields
	vec3_t		org;
	float		color;
// drivers never touch the following fields
	struct particle_s	*next;
	vec3_t		vel;
	vec3_t		min_org;
	vec3_t		max_org;
	float		ramp;
	float		die;
	byte		type;
	byte		flags;
	byte		count;
} particle_t;


void R_ReadPointFile_f (void);
void R_TranslatePlayerSkin (int playernum);


//====================================================


extern	entity_t	r_worldentity;
extern	qboolean	r_cache_thrash;		// compatability
extern	vec3_t		modelorg, r_entorigin;
extern	entity_t	*currententity;
extern	int		r_visframecount;	// ??? what difs?
extern	int		r_framecount;
extern	mplane_t	frustum[4];
extern	int		c_brush_polys, c_alias_polys;


//
// view origin
//
extern	vec3_t	vup;
extern	vec3_t	vpn;
extern	vec3_t	vright;
extern	vec3_t	r_origin;

//
// screen size info
//
extern	refdef_t	r_refdef;
extern	mleaf_t		*r_viewleaf, *r_oldviewleaf;
extern	texture_t	*r_notexture_mip;
extern	int		d_lightstylevalue[256];	// 8.8 fraction of base light value

extern	qboolean	envmap;
extern	int	currenttexture;
extern	int	particletexture;
extern	int	skytexturenum;		// index in cl.loadmodel, not gl texture object
extern	int	playertextures[16];
extern	byte	*playerTranslation;
extern	int	gl_texlevel;
extern	int	numgltextures;
extern	qboolean	flush_textures;

extern	cvar_t	r_norefresh;
extern	cvar_t	r_drawentities;
extern	cvar_t	r_drawworld;
extern	cvar_t	r_drawviewmodel;
extern	cvar_t	r_speeds;
extern	cvar_t	r_waterwarp;
extern	cvar_t	r_fullbright;
extern	cvar_t	r_lightmap;
extern	cvar_t	r_shadows;
extern	cvar_t	r_mirroralpha;
extern	cvar_t	r_wateralpha;
extern	cvar_t	r_skyalpha;
extern	cvar_t	r_dynamic;
extern	cvar_t	r_novis;
extern	cvar_t	r_wholeframe;

extern	cvar_t	gl_clear;
extern	cvar_t	gl_cull;
extern	cvar_t	gl_poly;
extern	cvar_t	gl_ztrick;
extern	cvar_t	gl_multitexture;
extern	cvar_t	gl_purge_maptex;
extern	cvar_t	gl_smoothmodels;
extern	cvar_t	gl_affinemodels;
extern	cvar_t	gl_polyblend;
extern	cvar_t	gl_keeptjunctions;
extern	cvar_t	gl_reporttjunctions;
extern	cvar_t	gl_flashblend;
extern	cvar_t	gl_nocolors;
extern	cvar_t	gl_waterripple;
extern	cvar_t	gl_waterwarp;
extern	cvar_t	gl_stencilshadow;
extern	cvar_t	gl_glows;
extern	cvar_t	gl_other_glows;
extern	cvar_t	gl_missile_glows;

extern	cvar_t	gl_coloredlight;
extern	cvar_t	gl_colored_dynamic_lights;
extern	cvar_t	gl_extra_dynamic_lights;
extern	int	gl_coloredstatic;

extern	cvar_t	gl_lightmapfmt;
extern	int	gl_lightmap_format;
extern	qboolean lightmap_modified[MAX_LIGHTMAPS];

extern	qboolean gl_mtexable;
extern	qboolean have_stencil;

extern	int	gl_max_size;
extern	cvar_t	gl_playermip;

extern	int		mirrortexturenum;	// quake texturenum, not gltexturenum
extern	qboolean	mirror;
extern	mplane_t	*mirror_plane;

extern	float	r_world_matrix[16];

extern	const char *gl_vendor;
extern	const char *gl_renderer;
extern	const char *gl_version;
extern	const char *gl_extensions;


/*
 * $Log: not supported by cvs2svn $
 * Revision 1.48  2006/04/10 12:02:08  sezero
 * gathered all compile-time opengl options into a gl_opt.h header file.
 * changed USE_HEXEN2_PALTEX_CODE and USE_HEXEN2_RESAMPLER_CODE to 0 / 1
 * instead of the previous define/undef.
 *
 * Revision 1.47  2006/03/29 20:46:02  sezero
 * minor fixups
 *
 * Revision 1.46  2006/03/24 15:05:39  sezero
 * killed the archive, server and info members of the cvar structure.
 * the new flags member is now employed for all those purposes. also
 * made all non-globally used cvars static.
 *
 * Revision 1.45  2006/03/23 20:01:33  sezero
 * made the lightmap format configurable via the menu system using a new
 * cvar gl_lightmapfmt. -lm_1 and -lm_4 are still functional as commandline
 * overrides even with commandline options dictating a direct load of a map,
 * such as +playdemo xxx or +load xxx. updated the opengl features menu with
 * new proper notifications about the colored lighting option's level reload
 * necessity and with new entries for the ligthmap format.
 *
 * Revision 1.44  2006/03/23 18:44:27  sezero
 * killed unnecessary includes, tiny tidy-up
 *
 * Revision 1.43  2006/03/14 11:44:19  sezero
 * arghhh... cvs changelogs...
 *
 * Revision 1.42  2006/03/11 22:51:16  sezero
 * continue making static functions and vars static. whitespace and coding style
 * cleanup. part 41: gl_draw.c, gl_rlight.c, gl_rmain.c, gl_rmisc.c, gl_rsurf.c,
 * gl_test.c, gl_warp.c, gl_mesh.c, gl_refrag.c, gl_ngraph.c.
 * a few compiler warnings were fixed and tiny synchronizations between h2 and
 * h2w versions were done along the way. more to come.
 *
 * Revision 1.41  2006/03/10 08:08:45  sezero
 * Added support for colored lights and .lit files;. initially extracted
 * from jshexen2 (thanks Michal Wozniak). Colored lights and lit file
 * support is now added to hexenworld, as well, although the new cvars
 * gl_colored_dynamic_lights and gl_extra_dynamic_light aren't functional
 * for it: hexenworld had some sort of "colored" dynamic lights in it,
 * and they aren't changed by this patch. The cvars mentions are fully
 * functional for hexen2. Colored lights member of the dlight_t in h2
 * is changed to be an array of 4, instead of vec3_t as it used to appear
 * in jshexen2, so that things look more alike with the hw version. The
 * default lightmap format is changed to GL_RGBA, (previously it was
 * GL_LUMINANCE.) Command line arguments can be employed to change the
 * lightmap format:  -lm_1 : GL_LUMINANCE, -lm_4 : GL_RGBA (default).
 * (Note: Would it be a good (AND a feasible) idea to add a menu+cvar
 * control for the lightmap format?)
 *
 * Revision 1.40  2006/03/04 15:35:23  sezero
 * opengl headers tidy-up
 *
 * Revision 1.39  2006/02/24 14:43:55  sezero
 * created a new "opengl features" entry under the options menu and moved opengl
 * options under it. added new opengl menu options for texture filtering, glow
 * effects, multitexturing, stencil buffered shadows and texture purging upon
 * map change.
 *
 * Revision 1.38  2006/01/20 07:36:47  sezero
 * Switched back to original hexen2 version of GL_ResampleTexture: the h2w (quake)
 * code gave slightly sharper but "jaggier" textures here and there. bad results
 * were especially detectable on help screens when using old cards with small (256
 * kb) texture size support (read: old voodoo boards). one can always use the h2w
 * version by changing the macro USE_HEXEN2_RESAMPLER_CODE from define to undef in
 * glquake.h.
 * Also changed the palettized texture code macro name from OLD_8_BIT_PALETTE_CODE
 * to USE_HEXEN2_PALTEX_CODE.
 *
 * Revision 1.37  2006/01/15 22:07:48  sezero
 * switched back to original hexen II code for palettized textures for better
 * quality, leaving the newer hexenworld (quake) code as option: changing the
 * macro OLD_8_BIT_PALETTE_CODE from define to undef in glquake.h will do the
 * switching.
 *
 * Revision 1.36  2006/01/07 09:50:09  sezero
 * added the last pieces (lightmaps) of adjustable filters which I've been
 * forgetting for a very long time
 *
 * Revision 1.35  2005/10/21 18:02:06  sezero
 * merged the gl headers back into glquake.h.
 * merged gl_func_nondl.h into gl_func.h.
 * added a unix makefile option for linking to gl functions at runtime,
 * similar to win32 version.
 *
 * Revision 1.34  2005/09/29 14:05:45  sezero
 * cleaned-up hash check stuff in GL_LoadTexture. added
 * crc check as an alternative (disabled by default.)
 *
 * Revision 1.33  2005/09/12 08:17:46  sezero
 * added stencil buffered shadows (extracted from jshexen2, code by MrG)
 *
 * Revision 1.32  2005/06/15 13:18:17  sezero
 * killed the glfunc struct
 *
 * Revision 1.31  2005/06/07 20:29:44  sezero
 * Added texture cheksumming (from Pa3PyX) for verification in GL_LoadTexture.
 * Die white textures, die die die!!!
 *
 * Revision 1.30  2005/06/05 16:14:07  sezero
 * patches I've been forgetting 6/6:
 * enable the irritating water-warp only by a cvar
 * and add water-ripple code by Jacques Krige
 *
 * Revision 1.29  2005/06/05 16:07:54  sezero
 * patches I've been forgetting 1/6:
 * use GL_LoadTexture for player textures
 *
 * Revision 1.28  2005/05/29 11:23:10  sezero
 * moved some defines to gl header files
 *
 * Revision 1.27  2005/05/28 12:31:47  sezero
 * added/updated multitexture detection. doesn't do anything yet.
 *
 * Revision 1.26  2005/05/28 12:02:03  sezero
 * Killed gl_texsort (thus, R_DrawSequentailPoly and co) which was
 * fairly broken. This also effectively killed the unfunctional
 * multitextune support in hexenworld: updated the detection but
 * doesn't do anything for now.
 *
 * Revision 1.25  2005/05/27 17:59:52  sezero
 * removed some dead code
 *
 * Revision 1.24  2005/05/27 17:56:40  sezero
 * merged texture scale/upload and 8-bit extension
 * functions from hexenworld to hexen2
 *
 * Revision 1.23  2005/05/20 18:15:22  sezero
 * stripped the platform spesific stuff out from glquake.h
 * into glxunix.h and glwin32.h
 *
 * Revision 1.22  2005/05/20 17:30:57  sezero
 * initial slight gl syncing-2 (kill unused c_sky_polys)
 *
 * Revision 1.21  2005/05/20 17:05:55  sezero
 * initial slight gl syncing
 *
 * Revision 1.20  2005/05/20 16:17:50  sezero
 * keep ID style in declerations (less diff between h2/h2w...)
 *
 * Revision 1.19  2005/04/30 09:59:16  sezero
 * Many things in gl_vidsdl.c, and *especially in vid_sdl.c, are there
 * for the dynamic video mode swithching which we removed a long time
 * ago (and we don't seem to put it back in any foreseeable future.)
 * Some stuff were there only to provide human readable descriptions in
 * the menu and I removed them in 1.2.3 or in 1.2.4. In this patch:
 * 1. Tried cleaning-up the remaining mess: There still were some
 *    windoze left-overs, unused variables/cvars, functions using those
 *    vars/cvars serving no purpose (especially those window_rect and
 *    window_center stuff, and more). I removed them as best as I could.
 *    There still are things in vid_sdl.c that I didn't fully understand,
 *    they are there, for now.
 * 2. The -window and -w cmdline args are now now removed: They actually
 *    did nothing, unless the user did some silly thing like using both
 *    -w and -f on the same cmdline.
 * 3. The two mode-setting functions (windowed and f/s) are made into one
 *    as VID_SDL_SetMode
 * 4. The -height arg now is functional *only* if used together -height.
 *    Since we only do the normal modes, I removed the width switch and
 *    calculated:  height = 3*width/4
 *    Issue: We need some sanity check in case of both -width and -height
 *    args are specified
 * 5. -bpp wasn't written into modenum[x].bpp, I did it here. As a side
 *    note, bpp doesn't affect anything, or my eyes are in more need of a
 *    doctor than I know: -bpp 8 / 16 / 32 give the same picture.
 * 6. The code calls VID_SetPalette very multiple times in gl_vidsdl.c.
 *    Why the hell is that?.. Something windoze spesific?  I unified them
 *    here in VID_Init: After VID_SetMode, VID_SetPalette is called first,
 *    and then 8-bit palette is activated if -paltex is specified.
 *    Note: I didn't touch vid_sdl.c in this manner, but DDOI (one of the
 *    guys during Dan's porting, perpahs) has a comment on a VID_SetPalette
 *    call being "Useless".
 * 7. Many whitespace clean-up as a bonus material ;)
 *
 * Revision 1.18  2005/04/30 09:06:07  sezero
 * Remove the 3dfx-spesific 3DFX_set_global_palette usage and favor
 * GL_EXT_shared_texture_palette, instead. VID_Download3DfxPalette
 * code taken from Pa3PyX.
 *
 * Revision 1.17  2005/04/30 09:02:17  sezero
 * Patch for voodoo1/voodoo2/rush, enabling 3dfx paletted texture extensions:
 * * it may help low vidmem issues without using gl_picmip uglies.
 * * may also help with a little performance.
 * Notes:
 * * paletted textures enabled only when -paltex cmdline arg is used, otherwise
 *   voodoo2 rendering is borked with both GL_EXT_shared_texture_palette and
 *   3DFX_set_global_palette (at least in my experience with Mesa-3.4.2 on
 *   RedHat-7.3)
 * * some dlsym fiddling from quake1 was added in order to successfuly link to
 *   the gl function. SDL_GL_GetProcAddress did not work for me...
 * * This 3dfx extension is hardware spesific and even Mesa says not to use it,
 *   but use the generic GL_EXT_shared_texture_palette, instead.
 *
 * Revision 1.16  2005/04/30 08:26:11  sezero
 * changed texnum member of gltexture_t to be of unsigned int type rather
 * than int
 *
 * Revision 1.15  2005/04/30 08:21:42  sezero
 * int texture_mode seem to serve nothing...
 *
 * Revision 1.14  2005/04/13 12:20:44  sezero
 * made GL_Bind a macro (from Pa3PyX)
 *
 * Revision 1.13  2005/04/08 18:30:08  sezero
 * 3dfx stuff initial clean-up
 *
 * Revision 1.12  2005/01/24 20:32:56  sezero
 * add sky alpha
 *
 * Revision 1.11  2005/01/24 20:27:25  sezero
 * consolidate GL_LoadTexture functions
 *
 * Revision 1.10  2005/01/12 11:59:11  sezero
 * remove some commented-out win32-stuff
 *
 * Revision 1.9  2005/01/08 16:07:45  sezero
 * don't die on cache mismatch, delete and and reload the new texture
 *
 * Revision 1.8  2005/01/01 21:43:47  sezero
 * prototypes clean-up
 *
 * Revision 1.7  2004/12/19 12:47:27  sezero
 * fix the datatype for our new load32
 *
 * Revision 1.6  2004/12/18 14:20:40  sezero
 * Clean-up and kill warnings: 11
 * A lot of whitespace cleanups.
 *
 * Revision 1.5  2004/12/18 13:59:25  sezero
 * Clean-up and kill warnings 8:
 * Missing prototypes.
 *
 * Revision 1.4  2004/12/18 13:30:50  sezero
 * Hack to prevent textures going awol and some info-plaques start looking
 * white upon succesive load games. The solution is not beautiful but seems
 * to work for now. Adapted from Pa3PyX sources.
 *
 * Revision 1.3  2004/12/16 18:10:12  sezero
 * - Add glGetIntegerv, glActiveTextureARB and glMultiTexCoord2fARB to the
 *   gl_func lists. (glGetIntegerv is required to init. The others are for
 *   future use).
 * - Use glGetIntegerv to detect gl_max_size, not vendor string. if > 1024,
 *   default to 1024 (inspired from pa3pyx).
 * - gl_max_size is not a cvar anymore (ditto).
 * - Kill cvar gl_nobind (ditto).
 *
 * Revision 1.2  2004/11/28 00:37:43  sezero
 * add gl-glow. code borrowed from the js sources
 *
 * Revision 1.1.1.1  2004/11/28 00:04:22  sezero
 * Initial import of AoT 1.2.0 code
 *
 * Revision 1.2  2002/01/02 15:23:23  phneutre
 * include gl_func.h (dlsym'ed functions)
 *
 * Revision 1.1.1.1  2001/11/09 17:03:58  theoddone33
 * Inital import
 *
 * 
 * 10    3/09/98 11:24p Mgummelt
 * 
 * 9     3/05/98 7:54p Jmonroe
 * fixed startRain, optimized particle struct
 * 
 * 8     1/27/98 10:57a Plipo
 * 
 * 20    9/18/97 2:34p Rlove
 * 
 * 19    9/17/97 1:27p Rlove
 * 
 * 18    9/17/97 11:11a Rlove
 * 
 * 17    7/15/97 4:09p Rjohnson
 * New particle effect
 * 
 * 16    7/03/97 12:22p Rjohnson
 * Fix for reading in the inverse palette
 * 
 * 15    6/16/97 4:20p Rjohnson
 * Added a sky poly count
 * 
 * 14    6/14/97 1:59p Rjohnson
 * Updated the fps display in the gl size, as well as meshing directories
 * 
 * 13    6/12/97 9:02a Rlove
 * New vorpal particle effect
 * 
 * 12    6/02/97 3:42p Gmctaggart
 * GL Catchup
 * 
 * 11    5/30/97 11:42a Rjohnson
 * Added new effect type for the rider's death
 * 
 * 10    5/23/97 3:05p Rjohnson
 * Update to effects / particle types
 * 
 * 9     4/30/97 11:19a Rjohnson
 * Added a particle type from the other version
 * 
 * 8     4/04/97 4:10p Rjohnson
 * Added proper transparent particles for the gl version
 * 
 * 7     3/22/97 5:19p Rjohnson
 * Changed the stone drawing flag to just a generic way based upon the
 * skin number
 * 
 * 6     3/22/97 3:21p Rjohnson
 * Moved the glpic structure to this header
 * 
 * 5     3/13/97 10:53p Rjohnson
 * New function to upload a texture with a specific alpha value
 * 
 * 4     3/07/97 1:11p Rjohnson
 * Added the rocket trail types
 * 
 * 3     3/07/97 1:06p Rjohnson
 * Id Updates
 * 
 * 2     2/20/97 12:13p Rjohnson
 * Code fixes for id update
 */
