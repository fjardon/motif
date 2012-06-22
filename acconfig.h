#ifndef __CONFIG_H__
#define __CONFIG_H__
@TOP@

#undef CSRG_BASED

/* define to avoid building the message catalog */
#undef NO_MESSAGE_CATALOG

/* Define if libXt was compiled with -DXTHREADS */
#undef XTHREADS
#undef XUSE_MTSAFE_API

/* Define if wctype.h is available */
#undef HAS_WIDECHAR_FUNCTIONS

/* Define to disable OpenLook compatibility */
#undef NO_OL_COMPAT

/* Define to enable the Theme code */
#undef USE_XMTHEMES
#undef DEBUGTHEMES

#undef HAVE_LIBXMU

#undef HAVE_SLIDEC_H

#undef NEED_STRCASECMP

#undef NEED_STRDUP

#undef NO_MEMMOVE

#undef NO_PUTENV

#undef NO_REGCOMP

@BOTTOM@
#endif
