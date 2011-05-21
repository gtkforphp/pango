/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors:  Michael Maclean <mgdm@php.net>                             |
  |           David Marín <davefx@gmail.com>                             |
  +----------------------------------------------------------------------+
*/

/* $Id: header 252479 2008-02-07 19:39:50Z iliaa $ */

#ifndef PHP_PANGO_H
#define PHP_PANGO_H

#define PHP_PANGO_VERSION "0.1.0-dev"

extern zend_module_entry pango_module_entry;
#define phpext_pango_ptr &pango_module_entry

extern zend_class_entry *pango_ce_pangoexception;

/* TODO: Move this elsewhere */
extern zend_class_entry *pango_ce_pangofontdescription;

extern zend_object_handlers pango_std_object_handlers;

#ifdef PHP_WIN32
#	define PHP_PANGO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_PANGO_API __attribute__ ((visibility("default")))
#else
#	define PHP_PANGO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* for PHP 5.2 */
#ifndef zend_parse_parameters_none
#define zend_parse_parameters_none()                                        \
    zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "")
#endif

#include <pango/pango.h>
#include <pango/pangocairo.h>
#include "php_cairo_api.h"

PHP_PANGO_API extern zend_class_entry *php_pango_get_context_ce(); 
PHP_PANGO_API extern zend_class_entry *php_pango_get_layoutline_ce(); 
PHP_PANGO_API extern zval* php_pango_make_layoutline_zval(PangoLayoutLine *line, zval *layout TSRMLS_DC);

/* Objects */
typedef struct _pango_context_object {
	zend_object std;
	PangoContext *context;
} pango_context_object;

typedef struct _pango_layout_object {
	zend_object std;
	PangoLayout *layout;
	zval *cairo_context;
	zval *pango_context;
} pango_layout_object;

typedef struct _pango_fontdesc_object {
	zend_object std;
	PangoFontDescription *fontdesc;
} pango_fontdesc_object;

typedef struct _pango_item_object {
	zend_object std;
	PangoItem *item;
} pango_item_object;

typedef struct _pango_layoutline_object {
	zend_object std;
	PangoLayoutLine *line;
	zval *layout_zval;
} pango_layoutline_object;

PHP_MINIT_FUNCTION(pango);
PHP_MSHUTDOWN_FUNCTION(pango);
PHP_MINFO_FUNCTION(pango);

PHP_MINIT_FUNCTION(pango_error);
PHP_MINIT_FUNCTION(pango_context);
PHP_MINIT_FUNCTION(pango_layout);
PHP_MINIT_FUNCTION(pango_font);

PHP_FUNCTION(pango_version);
PHP_FUNCTION(pango_version_string);

/* PangoLayout functions */
PHP_FUNCTION(pango_layout_new);
PHP_FUNCTION(pango_cairo_update_layout);
PHP_FUNCTION(pango_cairo_show_layout);
PHP_FUNCTION(pango_cairo_show_path);
PHP_FUNCTION(pango_layout_get_context);
PHP_FUNCTION(pango_layout_set_text);
PHP_FUNCTION(pango_layout_get_text);
PHP_FUNCTION(pango_layout_set_markup);
PHP_FUNCTION(pango_layout_get_width);
PHP_FUNCTION(pango_layout_get_height);
PHP_FUNCTION(pango_layout_get_size);
PHP_FUNCTION(pango_layout_get_pixel_size);
PHP_FUNCTION(pango_layout_get_extents);
PHP_FUNCTION(pango_layout_get_pixel_extents);
PHP_FUNCTION(pango_layout_set_width);
PHP_FUNCTION(pango_layout_set_height);
PHP_FUNCTION(pango_layout_set_font_description);
PHP_FUNCTION(pango_layout_get_font_description);
PHP_FUNCTION(pango_layout_get_alignment);
PHP_FUNCTION(pango_layout_set_alignment);
PHP_FUNCTION(pango_layout_get_justify);
PHP_FUNCTION(pango_layout_set_justify);
PHP_FUNCTION(pango_layout_get_wrap);
PHP_FUNCTION(pango_layout_set_wrap);
PHP_FUNCTION(pango_layout_is_wrapped);
PHP_FUNCTION(pango_layout_get_indent);
PHP_FUNCTION(pango_layout_set_indent);
PHP_FUNCTION(pango_layout_get_spacing);
PHP_FUNCTION(pango_layout_set_spacing);
PHP_FUNCTION(pango_layout_set_ellipsize);
PHP_FUNCTION(pango_layout_get_ellipsize);
PHP_FUNCTION(pango_layout_is_ellipsized);
PHP_FUNCTION(pango_layout_get_lines);
PHP_FUNCTION(pango_layout_get_line);
PHP_FUNCTION(pango_layout_get_line_count);
PHP_FUNCTION(pango_layout_context_changed);

/* PangoLayoutLine functions */
PHP_FUNCTION(pango_layout_line_get_extents);
PHP_FUNCTION(pango_layout_line_get_pixel_extents);
PHP_FUNCTION(pango_cairo_show_layout_line);

/* PangoFontDescription functions */
PHP_FUNCTION(pango_font_description_new);
PHP_FUNCTION(pango_font_description_get_variant);
PHP_FUNCTION(pango_font_description_set_variant);
PHP_FUNCTION(pango_font_description_equal);
PHP_FUNCTION(pango_font_description_set_family);
PHP_FUNCTION(pango_font_description_get_family);
PHP_FUNCTION(pango_font_description_set_size);
PHP_FUNCTION(pango_font_description_get_size);
PHP_FUNCTION(pango_font_description_get_style);
PHP_FUNCTION(pango_font_description_set_style);
PHP_FUNCTION(pango_font_description_get_weight);
PHP_FUNCTION(pango_font_description_set_weight);
PHP_FUNCTION(pango_font_description_get_stretch);
PHP_FUNCTION(pango_font_description_set_stretch);
PHP_FUNCTION(pango_font_description_to_string);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(pango)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(pango)
*/

/* In every utility function you add that needs to use variables 
   in php_pango_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as PANGO_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define PANGO_G(v) TSRMG(pango_globals_id, zend_pango_globals *, v)
#else
#define PANGO_G(v) (pango_globals.v)
#endif


/* turn error handling to exception mode and restore */
/* Borrowed from pecl/cairo, ta auroraeosrose */
#if PHP_VERSION_ID >= 50300
/* 5.3 version of the macros */
#define PHP_PANGO_ERROR_HANDLING(force_exceptions) \
    zend_error_handling error_handling; \
    if(force_exceptions || getThis()) { \
        zend_replace_error_handling(EH_THROW, pango_ce_pangoexception, &error_handling TSRMLS_CC); \
    }

#define PHP_PANGO_RESTORE_ERRORS(force_exceptions) \
    if(force_exceptions || getThis()) { \
        zend_restore_error_handling(&error_handling TSRMLS_CC); \
    }

#else
/* 5.2 versions of the macros */
#define PHP_PANGO_ERROR_HANDLING(force_exceptions) \
    if(force_exceptions || getThis()) { \
        php_set_error_handling(EH_THROW, pango_ce_pangoexception TSRMLS_CC); \
    }

#define PHP_PANGO_RESTORE_ERRORS(force_exceptions) \
    if(force_exceptions || getThis()) { \
        php_std_error_handling(); \
    }

#endif

/* do error or exception based on "are we in method or in function" */
#define PHP_PANGO_ERROR(status) \
    if(!getThis()) { \
        php_pango_trigger_error(status TSRMLS_CC); \
    } else { \
        php_pango_throw_exception(status TSRMLS_CC); \
    }


/* refcount macros */
#ifndef Z_ADDREF_P
#define Z_ADDREF_P(pz)                (pz)->refcount++
#endif

#ifndef Z_DELREF_P
#define Z_DELREF_P(pz)                (pz)->refcount--
#endif

#ifndef Z_SET_REFCOUNT_P
#define Z_SET_REFCOUNT_P(pz, rc)      (pz)->refcount = rc
#endif

#endif	/* PHP_PANGO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
