#include "m_pd.h"

void ead_tilde_setup(void);
void ear_tilde_setup(void);
void eadsr_tilde_setup(void);
void dist_tilde_setup(void);
void tabreadmix_tilde_setup(void);
void xfm_tilde_setup(void);
void biquadseries_tilde_setup(void);
void filterortho_tilde_setup(void);
void qmult_tilde_setup(void);
void qnorm_tilde_setup(void);
void cheby_tilde_setup(void);
void abs_tilde_setup(void);
void ramp_tilde_setup(void);
void dwt_tilde_setup(void);
void bfft_tilde_setup(void);
void dynwav_tilde_setup(void);
void statwav_tilde_setup(void);
void bdiag_tilde_setup(void);
void diag_tilde_setup(void);
void bmatrix_tilde_setup(void);
void permut_tilde_setup(void);
void lattice_tilde_setup(void);
void ratio_setup(void);
void ffpoly_setup(void);
void dt_ms_setup(void);
void fwarp_setup(void);
void junction_tilde_setup(void);
void fdn_tilde_setup(void);
void bwin_tilde_setup(void);
void blosc_tilde_setup(void);
void cmath_tilde_setup(void);
void bitsplit_tilde_setup(void);
void sbosc_tilde_setup(void);
void blocknorm_tilde_setup(void);
void resofilt_tilde_setup(void);
void scrollgrid1D_tilde_setup(void);
void sawtooth_tilde_setup(void);


#ifndef CREB_VERSION
#define CREB_VERSION "externals cvs"
#endif

#include "m_imp.h"
#include <fcntl.h>
#include "s_stuff.h"

#if PD_MAJOR_VERSION>0 || PD_MINOR_VERSION>47
/* gone from the headers, but still present for binary compat */
extern t_namelist *sys_searchpath;
#endif
static int _add_search_path(const char*libpath){
    int major, minor, bugfix;
    sys_getversion(&major, &minor, &bugfix);
    if((major==0 && minor < 48)) {
        sys_searchpath = namelist_append(sys_searchpath, libpath, 0);
    } else {
        char encoded[MAXPDSTRING];
        char *inptr = libpath, *outptr = encoded;
        t_atom ap[2];
        *outptr++='+';
        while(inptr && ((outptr+2) < (encoded+MAXPDSTRING))) {
            *outptr++ = *inptr++;
            if ('+'==inptr[-1])
                 *outptr++='+';
        }
        *outptr=0;
        SETSYMBOL(ap+0, gensym(encoded));
        SETFLOAT(ap+1, 0.f);
        pd_typedmess(gensym("pd")->s_thing, gensym("add-to-path"), 2, ap);
    }
    return 1;
}

static t_class *creb_class;
static void*creb_class_new(void)
{
  t_pd*x=pd_new(creb_class);
  return (x);
}
/* check whether we can find the creb-abstractions
 * (because they are already in Pd's path)
 */
void creb_class_setup(void) {
  const char*absname="creb-dev.txt";
  char buf[MAXPDSTRING];
  char*bufptr=NULL;
  const char*libpath=NULL;

  int fd=-1;

  /* create a [creb] objectclass */
  creb_class = class_new(gensym("creb"), creb_class_new, 0, sizeof(t_object), CLASS_NOINLET, 0);

  /* check whether we can find a creb-abstraction */
  if ((fd=canvas_open(NULL, absname, "", buf, &bufptr, MAXPDSTRING, 1))>=0){
    sys_close(fd);
    return;
  }

  /* couldn't find creb-abstraction, print-warning */
  libpath=creb_class->c_externdir->s_name;
  /* check whether we can find the abstractions in creb's own path */
  snprintf(buf, MAXPDSTRING-1, "%s/%s", libpath, absname);
  buf[MAXPDSTRING-1]=0;
  if ((fd=sys_open(buf, O_RDONLY))>=0){
    sys_close(fd);
    logpost(NULL, 3, "CREB: adding '%s' to your search-path", libpath);
    if(!_add_search_path(libpath))
      fd=-1;
  }
  if (fd<0) {
    // can't find this abstraction...giving up
    logpost(NULL, 1, "CREB: cannot find creb-abstractions");
    logpost(NULL, 3, "CREB: please add path to '%s' to your search-path!", absname);
  }
}



void creb_setup(void)
{
  post("CREB: version " CREB_VERSION);

  /* setup tilde objects */
  ead_tilde_setup();
  ear_tilde_setup();
  eadsr_tilde_setup();
  dist_tilde_setup();
  tabreadmix_tilde_setup();
  xfm_tilde_setup();
  qmult_tilde_setup();
  qnorm_tilde_setup();
  cheby_tilde_setup();
  ramp_tilde_setup();
  dwt_tilde_setup();
  bfft_tilde_setup();
  dynwav_tilde_setup();
  statwav_tilde_setup();
  bdiag_tilde_setup();
  diag_tilde_setup();
  bmatrix_tilde_setup();
  permut_tilde_setup();
  lattice_tilde_setup();
  junction_tilde_setup();
  fdn_tilde_setup();
  bwin_tilde_setup();
  blosc_tilde_setup();
  cmath_tilde_setup();
  bitsplit_tilde_setup();
  sbosc_tilde_setup();
  blocknorm_tilde_setup();
  resofilt_tilde_setup();
  scrollgrid1D_tilde_setup();
  sawtooth_tilde_setup();

  /* setup other objects */
  ratio_setup();
  ffpoly_setup();
  dt_ms_setup();
  fwarp_setup();

  /* setup c++ modules */
  biquadseries_tilde_setup();
  filterortho_tilde_setup();

  /* optional modules */
#ifdef HAVE_ABS_TILDE
  abs_tilde_setup();
#endif

  creb_class_setup();
}
