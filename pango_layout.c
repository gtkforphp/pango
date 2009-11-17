/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  Michael Maclean <mgdm@php.net>                              |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_pango.h"

zend_class_entry *pango_ce_pangolayout;

/* {{{ proto PangoLayout::__construct(CairoContext cr)
   Creates a PangoLayout based on the CairoContext object */

PHP_METHOD(PangoLayout, __construct)
{
	zval *context_zval = NULL;
	zend_class_entry *cairo_ce_cairocontext = php_cairo_get_context_ce();
	cairo_context_object *context_object;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(TRUE)
		if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &context_zval, cairo_ce_cairocontext) == FAILURE)
		{
			PHP_PANGO_RESTORE_ERRORS(TRUE)
			return;
		}
	PHP_PANGO_RESTORE_ERRORS(TRUE)
	
	cairo_context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	layout_object->layout = pango_cairo_create_layout(context_object->context);
	php_pango_trigger_error(cairo_status(layout_object->layout));
	
	/* We may want this later, so reference it and store */
	layout_object->context = context_zval;
	Z_ADDREF_P(context_zval);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
