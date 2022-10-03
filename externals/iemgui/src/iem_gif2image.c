/* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.

iem_gif2image written by Thomas Musil, Copyright (c) IEM KUG Graz Austria 2000 - 2009 */

#include "m_pd.h"
#include "iemlib.h"
#include "g_canvas.h"
#include "g_all_guis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MSW
#include <io.h>
#else
#include <unistd.h>
#endif

#define IEM_GIF2IMAGE_WAIT 1000

/* ------------------------ setup routine ------------------------- */

t_widgetbehavior iem_gif2image_widgetbehavior;
static t_class *iem_gif2image_class;
static char iem_gif2image_base64_transform[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

typedef struct _iem_gif2image_base64_4hex
{
	unsigned int dummy	: 8;
	unsigned int hex4		: 6;
	unsigned int hex3		: 6;
	unsigned int hex2		: 6;
	unsigned int hex1		: 6;
} t_iem_gif2image_base64_4hex;

typedef struct _iem_gif2image_base64_3byte
{
	unsigned int dummy		: 8;
	unsigned int byte3		: 8;
	unsigned int byte2		: 8;
	unsigned int byte1		: 8;
} t_iem_gif2image_base64_3byte;

typedef union _iem_gif2image_base64_3u4
{
	t_iem_gif2image_base64_4hex			h4;
	t_iem_gif2image_base64_3byte		b3;
} t_iem_gif2image_base64_3u4;

typedef struct _iem_gif2image
{
	t_iemgui	x_gui;
	t_symbol	*x_gifname;
  t_symbol  *x_s_dst_base;
  t_clock   *x_clock;
} t_iem_gif2image;

static void iem_gif2image_do_read_write_tick(t_iem_gif2image *x);

static void iem_gif2image_convert_gif_to_base64(char *beg_src, char *beg_dst, int work_size)
{
	int i, j, k;
	unsigned int uindex;
	t_iem_gif2image_base64_3u4 work;
  
	work.b3.dummy = 0;
  
	for(i=0, j=0, k=0; i<work_size; i++)
	{
		work.b3.byte1 = (unsigned char)beg_src[j];
    j++;
		work.b3.byte2 = (unsigned char)beg_src[j];
		j++;
		work.b3.byte3 = (unsigned char)beg_src[j];
		j++;
    
		uindex = (unsigned int)work.h4.hex1;
		beg_dst[k] = iem_gif2image_base64_transform[uindex];
		k++;
		uindex = (unsigned int)work.h4.hex2;
		beg_dst[k] = iem_gif2image_base64_transform[uindex];
		k++;
		uindex = (unsigned int)work.h4.hex3;
		beg_dst[k] = iem_gif2image_base64_transform[uindex];
		k++;
		uindex = (unsigned int)work.h4.hex4;
		beg_dst[k] = iem_gif2image_base64_transform[uindex];
		k++;
	}
}

static int iem_gif2image_calc_size(t_iem_gif2image *x, t_symbol	*gifname)
{
	char dirbuf[MAXPDSTRING], *namebufptr;
	char namebuf[MAXPDSTRING];
	unsigned char buf[222];
	unsigned int i;
	char *c;
	int fd;
	FILE *fh;

  if(gifname)
	{
    if(!gifname->s_name)
    {
      post("iem_gif2image-ERROR: no gifname");
      x->x_gifname = (t_symbol *)0;
      return(0);
    }
    fd = open_via_path(canvas_getdir(glist_getcanvas(x->x_gui.x_glist))->s_name, gifname->s_name,
                       "", dirbuf, &namebufptr, MAXPDSTRING, 1);
    if (fd < 0)
    {
      post("iem_gif2image-ERROR: cannot open %s first time", gifname->s_name);
      x->x_gifname = (t_symbol *)0;
      return(0);
    }
    else
    {
      if(fd >= 0)
        close(fd);
      strcpy(namebuf, dirbuf);
      strcat(namebuf, "/");
      strcat(namebuf, namebufptr);
      fh = fopen(namebuf, "r");
      if(fh == NULL)
      {
        post("iem_gif2image-ERROR: cannot open %s second time", namebuf);
        x->x_gifname = (t_symbol *)0;
        return(0);
      }
      else
      {
        fread(buf, 22, sizeof(unsigned char), fh);
        fclose(fh);
        c = (char *)buf;
        if((c[0] != 'G')||(c[1] != 'I')||(c[2] != 'F'))
        {
          post("iem_gif2image-ERROR: %s is not a GIF-file", namebuf);
          x->x_gifname = (t_symbol *)0;
          return(0);
        }
        i = 256*(unsigned int)buf[7];
        i += (unsigned int)buf[6];
        x->x_gui.x_w = (int)i;
        i = 256*(unsigned int)buf[9];
        i += (unsigned int)buf[8];
        x->x_gui.x_h = (int)i;
        post("width = %d, height = %d",x->x_gui.x_w,x->x_gui.x_h);
        x->x_gifname = gensym(namebuf);
        return(1);
      }
    }
  }
}

static void iem_gif2image_do_read_write_tick(t_iem_gif2image *x)
{
  FILE *fh;
	int i, h, src_size, dst_size, work_size;
	char fn_base_gif[MAXPDSTRING], *beg_src, *beg_dst;
  
  strcpy(fn_base_gif, x->x_s_dst_base->s_name);
  strcat(fn_base_gif, ".gif");
  fh = fopen(fn_base_gif, "rb");
  h = (int)fh;
  if(h == 0)
  {
    printf("iem_gif2image_ERROR: cannot open %s\n", fn_base_gif);
    return;
  }
  i = (int)fseek(fh, 0, SEEK_END);
  src_size = (int)ftell(fh);
  h = src_size * 8;
  i = h % 6;
  if(i > 0)
    dst_size = (h / 6) + 1;
  else
    dst_size = h / 6;
  beg_src = (char *)malloc((src_size + 100)*sizeof(char));
  beg_dst = (char *)malloc((dst_size + 100)*sizeof(char));
  i = src_size % 3;
  if(i > 0)
    work_size = (src_size / 3) + 1;
  else
    work_size = src_size / 3;
  h = src_size + 100;
  for(i=src_size-1; i<h; i++)
    beg_src[i] = 0;
  
  i = (int)fseek(fh, 0, SEEK_SET);
  fread(beg_src, src_size, sizeof(char), fh);
  fclose(fh);
  
  iem_gif2image_convert_gif_to_base64(beg_src, beg_dst, work_size);
  
  fh = fopen(x->x_s_dst_base->s_name, "wb");
  h = (int)fh;
  if(h == 0)
  {
    printf("iem_gif2image_ERROR: cannot create %s\n", x->x_s_dst_base->s_name);
    free(beg_src);
    free(beg_dst);
    return;
  }
  fwrite(beg_dst, dst_size, sizeof(char), fh);
  fclose(fh);
  free(beg_src);
  free(beg_dst);
}

static void iem_gif2image_draw_new(t_iem_gif2image *x, t_glist *glist)
{
	int xpos=text_xpix(&x->x_gui.x_obj, glist);
	int ypos=text_ypix(&x->x_gui.x_obj, glist);
	t_canvas *canvas=glist_getcanvas(glist);

	if(iem_gif2image_calc_size(x, x->x_gifname))
	{
		sys_vgui("image create photo %xPHOTOIMAGE -file {%s} -format gif -width %d -height %d\n",
				x, x->x_gifname->s_name, x->x_gui.x_w, x->x_gui.x_h);
		sys_vgui(".x%x.c create image %d %d -image %xPHOTOIMAGE -tags %xPHOTO\n",
				canvas, xpos+x->x_gui.x_w/2, ypos+x->x_gui.x_h/2, x, x);
	}	

	if(x->x_gui.x_fsf.x_selected)
		sys_vgui(".x%x.c create rectangle %d %d %d %d -outline #%6.6x -tags %xBASE\n",
			canvas, xpos, ypos, xpos + x->x_gui.x_w, ypos + x->x_gui.x_h, IEM_GUI_COLOR_SELECTED, x);
}

static void iem_gif2image_draw_move(t_iem_gif2image *x, t_glist *glist)
{
	int xpos=text_xpix(&x->x_gui.x_obj, glist);
	int ypos=text_ypix(&x->x_gui.x_obj, glist);
	t_canvas *canvas=glist_getcanvas(glist);

	if(x->x_gifname)
		sys_vgui(".x%x.c coords %xPHOTO %d %d\n", canvas, x, xpos+x->x_gui.x_w/2, ypos+x->x_gui.x_h/2);
	if(x->x_gui.x_fsf.x_selected)
		sys_vgui(".x%x.c coords %xBASE %d %d %d %d\n",
		 canvas, x, xpos, ypos, xpos + x->x_gui.x_w, ypos + x->x_gui.x_h);
	canvas_fixlinesfor(glist_getcanvas(x->x_gui.x_glist), (t_text*)x);
}

static void iem_gif2image_draw_erase(t_iem_gif2image* x, t_glist* glist)
{
	t_canvas *canvas=glist_getcanvas(glist);

	if(x->x_gui.x_fsf.x_selected)
		sys_vgui(".x%x.c delete %xBASE\n", canvas, x);
	if(x->x_gifname)
	{
		sys_vgui("image delete %xPHOTOIMAGE\n", x);
		sys_vgui(".x%x.c delete %xPHOTO\n", canvas, x);
	}
}

static void iem_gif2image_draw_select(t_iem_gif2image* x, t_glist* glist)
{
	t_canvas *canvas=glist_getcanvas(glist);

	if(x->x_gui.x_fsf.x_selected)
	{
		int xpos=text_xpix(&x->x_gui.x_obj, glist);
		int ypos=text_ypix(&x->x_gui.x_obj, glist);

		sys_vgui(".x%x.c create rectangle %d %d %d %d -outline #%6.6x -tags %xBASE\n",
			canvas, xpos, ypos, xpos + x->x_gui.x_w,
				ypos + x->x_gui.x_h, IEM_GUI_COLOR_SELECTED, x);
	}
	else
		sys_vgui(".x%x.c delete %xBASE\n", canvas, x);
}

static void iem_gif2image_draw(t_iem_gif2image *x, t_glist *glist, int mode)
{
	if(mode == IEM_GUI_DRAW_MODE_MOVE)
		iem_gif2image_draw_move(x, glist);
	else if(mode == IEM_GUI_DRAW_MODE_NEW)
		iem_gif2image_draw_new(x, glist);
	else if(mode == IEM_GUI_DRAW_MODE_SELECT)
		iem_gif2image_draw_select(x, glist);
	else if(mode == IEM_GUI_DRAW_MODE_ERASE)
		iem_gif2image_draw_erase(x, glist);
}

/* ------------------------ cnv widgetbehaviour----------------------------- */

static void iem_gif2image_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
	t_iem_gif2image *x = (t_iem_gif2image *)z;

	*xp1 = text_xpix(&x->x_gui.x_obj, glist);
	*yp1 = text_ypix(&x->x_gui.x_obj, glist);
	*xp2 = *xp1 + x->x_gui.x_w;
	*yp2 = *yp1 + x->x_gui.x_h;
}

static void iem_gif2image_save(t_gobj *z, t_binbuf *b)
{
	t_iem_gif2image *x = (t_iem_gif2image *)z;
	
	binbuf_addv(b, "ssiis", gensym("#X"),gensym("obj"),
		(t_int)x->x_gui.x_obj.te_xpix, (t_int)x->x_gui.x_obj.te_ypix,
		gensym("iem_gif2image"));
	binbuf_addv(b, ";");
}

static void iem_gif2image_read_gif(t_iem_gif2image *x, t_symbol *name)
{
	if(name && name->s_name)
	{
		if(x->x_gifname)
		{
			(*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_ERASE);
			x->x_gifname = (t_symbol *)0;
		}
		x->x_gifname = name;
		(*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_NEW);
	}
}

static void iem_gif2image_clear(t_iem_gif2image *x)
{
	(*x->x_gui.x_draw)(x, x->x_gui.x_glist, IEM_GUI_DRAW_MODE_ERASE);
	x->x_gifname = (t_symbol *)0;
}

static void iem_gif2image_write_base64(t_iem_gif2image *x, t_symbol *s_dst_base)
{
  FILE *fh;
	int i, h, src_size, dst_size, work_size;
	char fn_base_gif[MAXPDSTRING];
  
	if((x->x_gifname) && (x->x_gifname->s_name) && s_dst_base && s_dst_base->s_name)
  {
    strcpy(fn_base_gif, s_dst_base->s_name);
    strcat(fn_base_gif, ".gif");
		sys_vgui("%xPHOTOIMAGE write {%s} -format gif\n", x, fn_base_gif);
    x->x_s_dst_base = s_dst_base;
    clock_delay(x->x_clock, IEM_GIF2IMAGE_WAIT);
	}
}

static void *iem_gif2image_new(void)
{
	t_iem_gif2image *x = (t_iem_gif2image *)pd_new(iem_gif2image_class);

	x->x_gui.x_draw = (t_iemfunptr)iem_gif2image_draw;
	x->x_gui.x_glist = (t_glist *)canvas_getcurrent();
	x->x_gui.x_w = 100;
	x->x_gui.x_h = 60;
	x->x_gifname = (t_symbol *)0;
  x->x_s_dst_base = (t_symbol *)0;
	x->x_gui.x_fsf.x_selected = 0;
  x->x_clock = clock_new(x, (t_method)iem_gif2image_do_read_write_tick);
	return(x);
}

static void iem_gif2image_ff(t_iem_gif2image *x)
{
	gfxstub_deleteforkey(x);
  clock_free(x->x_clock);
}

void iem_gif2image_setup(void)
{
	iem_gif2image_class = class_new(gensym("iem_gif2image"), (t_newmethod)iem_gif2image_new,
				(t_method)iem_gif2image_ff, sizeof(t_iem_gif2image), 0, 0);	
	class_addmethod(iem_gif2image_class, (t_method)iem_gif2image_read_gif, gensym("read_gif"), A_SYMBOL, 0);
	class_addmethod(iem_gif2image_class, (t_method)iem_gif2image_clear, gensym("clear"), 0);
	class_addmethod(iem_gif2image_class, (t_method)iem_gif2image_write_base64, gensym("write_base64"), A_SYMBOL, 0);

	iem_gif2image_widgetbehavior.w_getrectfn = iem_gif2image_getrect;
	iem_gif2image_widgetbehavior.w_displacefn = iemgui_displace;
	iem_gif2image_widgetbehavior.w_selectfn = iemgui_select;
	iem_gif2image_widgetbehavior.w_activatefn = NULL;
	iem_gif2image_widgetbehavior.w_deletefn = iemgui_delete;
	iem_gif2image_widgetbehavior.w_visfn = iemgui_vis;
	iem_gif2image_widgetbehavior.w_clickfn = NULL;

#if defined(PD_MAJOR_VERSION) && (PD_MINOR_VERSION >= 37)
	class_setsavefn(iem_gif2image_class, iem_gif2image_save);
#else
	iem_gif2image_widgetbehavior.w_propertiesfn = NULL;
	iem_gif2image_widgetbehavior.w_savefn = iem_gif2image_save;
#endif

	class_setwidget(iem_gif2image_class, &iem_gif2image_widgetbehavior);
}
