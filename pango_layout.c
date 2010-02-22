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

#include "zend_exceptions.h"

zend_class_entry *pango_ce_pangolayout;
zend_class_entry *pango_ce_pangowrapmode;
zend_class_entry *pango_ce_pangoellipsizemode;

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

	context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);

	layout_object = (pango_layout_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	layout_object->layout = pango_cairo_create_layout(context_object->context);
	
	if(layout_object->layout == NULL) {
		zend_throw_exception(pango_ce_pangoexception, "Could not create the Pango layout", 0 TSRMLS_CC);
		return;
	}
	
	/* We may want this later, so reference it and store */
	layout_object->cairo_context = context_zval;
	Z_ADDREF_P(context_zval);
}

/* }}} */

/* {{{ proto pango_layout_new(CairoContext cr)
   Creates a PangoLayout based on the CairoContext object */

PHP_FUNCTION(pango_layout_new)
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

	context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);

	object_init_ex(return_value, pango_ce_pangolayout);
	layout_object = (pango_layout_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	layout_object->layout = pango_cairo_create_layout(context_object->context);
	
	if(layout_object->layout == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not create the Pango layout");
		return;
	}
	
	/* We may want this later, so reference it and store */
	layout_object->cairo_context = context_zval;
	Z_ADDREF_P(context_zval);
}


/* }}} */

/* {{{ proto PangoContext PangoLayout::getContext()
   proto PangoContext pango_layout_get_context()
   Return the PangoContext for the current layout */
PHP_FUNCTION(pango_layout_get_context)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	pango_context_object *context_object;
	PangoContext *context;	
	zend_class_entry *ce;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	context = pango_layout_get_context(layout_object->layout);

	/* Have we already got the context object and cached it? */
	if(layout_object->pango_context) {		
		zval_dtor(return_value);
		*return_value = *layout_object->pango_context;
		zval_copy_ctor(return_value);
		Z_SET_REFCOUNT_P(return_value, 1);
	} else {
		/* We haven't already got one, let's make one */
		ce = php_pango_get_context_ce();
		object_init_ex(return_value, ce);
	}

	    /* Get the context_object and replace the internal context pointer 
		 * with what we fetched (should be the same) */
    context_object = (pango_context_object *)zend_object_store_get_object(return_value TSRMLS_CC);
    /* if there IS a value in context, destroy it cause we're getting a new one */
    if (context_object->context != NULL) {
		g_object_unref(context_object->context);
    }    
    /* Grab the context properly */
    context_object->context = context;
    g_object_ref(context_object->context);
}
/* }}} */

/* {{{ proto void pango_layout_set_text(PangoLayout layout, string text)
 	   proto void PangoLayout::setText(string text)
	   Sets the text of the layout. */
PHP_FUNCTION(pango_layout_set_text)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	const char *text;
	long text_len;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &layout_zval, pango_ce_pangolayout, &text, &text_len) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_text(layout_object->layout, text, text_len);
}

/* }}} */

/* {{{ proto string pango_layout_get_text(PangoLayout layout)
 	   proto string PangoLayout::getText(string text)
	   Gets the text currently in the layout */
PHP_FUNCTION(pango_layout_get_text)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	const char *text;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	if((text = pango_layout_get_text(layout_object->layout))) {
		RETURN_STRING((char *)text, 1);
	}
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto void pango_layout_set_markup(PangoLayout layout, string markup)
 	   proto void PangoLayout::setMarkup(string markup)
	   Sets the markup of the layout. */
PHP_FUNCTION(pango_layout_set_markup)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	const char *markup;
	long markup_len;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &layout_zval, pango_ce_pangolayout, &markup, &markup_len) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_markup(layout_object->layout, markup, markup_len);
}

/* {{{ proto void pango_layout_context_changed(PangoLayout layout)
 	   proto void PangoLayout::contextChanged() 
	   Updates the private PangoContext of a PangoLayout to match the current transformation 
	   and target surface of a Cairo context. 
	   NB: PARAMS ARE REVERSED FROM NATIVE PANGO
	   */

PHP_FUNCTION(pango_layout_context_changed)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_parameters_none() == FAILURE) {
	   PHP_PANGO_RESTORE_ERRORS(FALSE)
	   return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)	
	
	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_context_changed(layout_object->layout);
}
/* }}} */


/* }}} */


#if 0
/* Need to work out how to handle multibyte characters here */
/* {{{ proto void pango_layout_set_markup(PangoLayout layout, string markup)
 	   proto void PangoLayout::setText(string markup)
	   Sets the markup of the layout with accelerator markers. */
PHP_FUNCTION(pango_layout_set_markup_with_accel)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	const char *markup;
	const char *accel_marker;
	long markup_len = 0, accel_marker_len = 0, first_accel = 0;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Oss", &layout_zval, pango_ce_pangolayout, &markup, &markup_len, &accel_marker, &accel_marker_len) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	if(accel_marker_len > 0) {
		pango_layout_set_markup_with_accel(
				layout_object->layout, markup, markup_len, accel_marker[0], &first_accel);				
	} else {
		pango_layout_set_markup_with_accel(layout_object->layout, markup, markup_len, 0, &first_accel);
	}
	RETURN_LONG(first_accel);
}
#endif

/* }}} */

/* {{{ proto void pango_cairo_update_layout(CairoContext cr, PangoLayout layout)
 	   proto void PangoLayout::updateLayout(CairoContext cr) 
	   Updates the private PangoContext of a PangoLayout to match the current transformation 
	   and target surface of a Cairo context. 
	   NB: PARAMS ARE REVERSED FROM NATIVE PANGO
	   */

PHP_FUNCTION(pango_cairo_update_layout)
{
	zval *layout_zval = NULL, *context_zval = NULL;
	pango_layout_object *layout_object;
	cairo_context_object *context_object;
	zend_class_entry *cairo_ce_cairocontext = php_cairo_get_context_ce();

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O|O", &layout_zval, pango_ce_pangolayout, &context_zval, cairo_ce_cairocontext) == FAILURE) {
	   PHP_PANGO_RESTORE_ERRORS(FALSE)
	   return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)	
	
	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	
	/* If the user supplies a context, use that, otherwise get the one from the layout */
	if(context_zval) {
		context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	} else {
		context_object = (cairo_context_object *)zend_object_store_get_object(layout_object->cairo_context TSRMLS_CC);
	}

	pango_cairo_update_layout(context_object->context, layout_object->layout);
}
/* }}} */

/* {{{ proto void pango_cairo_show_layout(CairoContext cr, PangoLayout layout)
 	   proto void PangoLayout::showLayout(CairoContext cr) 
	   Draws a PangoLayoutLine in the specified cairo context.
	   NB: PARAMS ARE REVERSED FROM NATIVE PANGO
	   */

PHP_FUNCTION(pango_cairo_show_layout)
{
	zval *layout_zval = NULL, *context_zval = NULL;
	pango_layout_object *layout_object;
	cairo_context_object *context_object;
	zend_class_entry *cairo_ce_cairocontext = php_cairo_get_context_ce();

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O|O", &layout_zval, pango_ce_pangolayout, &context_zval, cairo_ce_cairocontext) == FAILURE) {
	   PHP_PANGO_RESTORE_ERRORS(FALSE)
	   return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)	
	
	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);

	/* If the user supplies a context, use that, otherwise get the one from the layout */
	if(context_zval) {
		context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	} else {
		context_object = (cairo_context_object *)zend_object_store_get_object(layout_object->cairo_context TSRMLS_CC);
	}

	pango_cairo_show_layout(context_object->context, layout_object->layout);
}
/* }}} */

/* {{{ proto void pango_cairo_layout_path(CairoContext cr, PangoLayout layout)
 	   proto void PangoLayout::layoutPath(CairoContext cr) 
	   Adds the specified text to the current path in the specified cairo context.
	   NB: PARAMS ARE REVERSED FROM NATIVE PANGO
	   */

PHP_FUNCTION(pango_cairo_layout_path)
{
	zval *layout_zval = NULL, *context_zval = NULL;
	pango_layout_object *layout_object;
	cairo_context_object *context_object;
	zend_class_entry *cairo_ce_cairocontext = php_cairo_get_context_ce();

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O|O", &layout_zval, pango_ce_pangolayout, &context_zval, cairo_ce_cairocontext) == FAILURE) {
	   PHP_PANGO_RESTORE_ERRORS(FALSE)
	   return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)	
	
	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	
	/* If the user supplies a context, use that, otherwise get the one from the layout */
	if(context_zval) {
		context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	} else {
		context_object = (cairo_context_object *)zend_object_store_get_object(layout_object->cairo_context TSRMLS_CC);
	}
	
	pango_cairo_layout_path(context_object->context, layout_object->layout);
}
/* }}} */

/* {{{ proto void pango_layout_get_width(PangoLayout layout, long width)
 	   proto void PangoLayout::getWidth(long width)
	   Sets the width of the layout. */
PHP_FUNCTION(pango_layout_get_width)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	long width;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	if((width = pango_layout_get_width(layout_object->layout))) {
		RETURN_LONG(width);
	}
	RETURN_FALSE;
}

/* }}} */

/* {{{ proto void pango_layout_get_height(PangoLayout layout, long height)
 	   proto void PangoLayout::getHeight(long height)
	   Sets the height of the layout. */
PHP_FUNCTION(pango_layout_get_height)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	long height;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	if((height = pango_layout_get_height(layout_object->layout))) {
		RETURN_LONG(height);
	}
	RETURN_FALSE;
}

/* }}} */

/* {{{ proto void pango_layout_get_size(PangoLayout layout)
 	   proto void PangoLayout::getHeight()
	   Sets the size of the layout. */
PHP_FUNCTION(pango_layout_get_size)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	int height = 0, width = 0;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_get_size(layout_object->layout, &width, &height);

	array_init(return_value);
	add_assoc_long(return_value, "width", width);
	add_assoc_long(return_value, "height", height);
}

/* }}} */

/* {{{ proto void pango_layout_get_pixel_size(PangoLayout layout)
 	   proto void PangoLayout::getPixelSize()
	   Gets the size of layout in pixels */
PHP_FUNCTION(pango_layout_get_pixel_size)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	int height = 0, width = 0;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_get_pixel_size(layout_object->layout, &width, &height);

	array_init(return_value);
	add_assoc_long(return_value, "width", width);
	add_assoc_long(return_value, "height", height);
}

/* }}} */

/* {{{ proto void pango_layout_get_extents(PangoLayout layout)
 	   proto void PangoLayout::getExtents()
	   Gets the extents of layout in */
PHP_FUNCTION(pango_layout_get_extents)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	PangoRectangle ink;
	PangoRectangle logical;
	zval *array;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_get_extents(layout_object->layout, &ink, &logical);

	array_init(return_value);
	ALLOC_INIT_ZVAL(array);
	array_init(array);
	add_assoc_long(array, "x", ink.x);
	add_assoc_long(array, "y", ink.y);
	add_assoc_long(array, "width", ink.width);
	add_assoc_long(array, "height", ink.height);
	add_assoc_zval(return_value, "ink", array);
	ALLOC_INIT_ZVAL(array);
	array_init(array);
	add_assoc_long(array, "x", logical.x);
	add_assoc_long(array, "y", logical.y);
	add_assoc_long(array, "width", logical.width);
	add_assoc_long(array, "height", logical.height);
	add_assoc_zval(return_value, "logical", array);
}

/* }}} */

/* {{{ proto void pango_layout_get_pixel_extents(PangoLayout layout)
 	   proto void PangoLayout::getPixelExtents()
	   Gets the extents of layout in pixels */
PHP_FUNCTION(pango_layout_get_pixel_extents)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	PangoRectangle ink;
	PangoRectangle logical;
	zval *array;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_get_pixel_extents(layout_object->layout, &ink, &logical);

	array_init(return_value);
	ALLOC_INIT_ZVAL(array);
	array_init(array);
	add_assoc_long(array, "x", ink.x);
	add_assoc_long(array, "y", ink.y);
	add_assoc_long(array, "width", ink.width);
	add_assoc_long(array, "height", ink.height);
	add_assoc_zval(return_value, "ink", array);
	ALLOC_INIT_ZVAL(array);
	array_init(array);
	add_assoc_long(array, "x", logical.x);
	add_assoc_long(array, "y", logical.y);
	add_assoc_long(array, "width", logical.width);
	add_assoc_long(array, "height", logical.height);
	add_assoc_zval(return_value, "logical", array);
}

/* }}} */

/* {{{ proto void pango_layout_set_width(PangoLayout layout, long width)
 	   proto void PangoLayout::setWidth(long width)
	   Sets the width of the layout. */
PHP_FUNCTION(pango_layout_set_width)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	long width;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &layout_zval, pango_ce_pangolayout, &width) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_width(layout_object->layout, width);
}

/* }}} */

/* {{{ proto void pango_layout_set_height(PangoLayout layout, long height)
 	   proto void PangoLayout::setHeight(long height)
	   Sets the height of the layout. */
PHP_FUNCTION(pango_layout_set_height)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	long height;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &layout_zval, pango_ce_pangolayout, &height) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_height(layout_object->layout, height);
}

/* }}} */

/* {{{ proto void pango_layout_set_font_description(PangoLayout layout, long font_description)
 	   proto void PangoLayout::setHeight(long font_description)
	   Sets the font_description of the layout. */
PHP_FUNCTION(pango_layout_set_font_description)
{
	zval *layout_zval = NULL, *fontdesc_zval = NULL;
	pango_layout_object *layout_object;
	pango_fontdesc_object *fontdesc_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO", &layout_zval, pango_ce_pangolayout, &fontdesc_zval, pango_ce_pangofontdescription) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	pango_layout_set_font_description(layout_object->layout, fontdesc_object->fontdesc);
}

/* }}} */

/* {{{ proto void pango_layout_set_justify(PangoLayout layout, bool justify)
 	   proto void PangoLayout::setJustify(bool justify)
	   Sets whether each line should be justified. */
PHP_FUNCTION(pango_layout_set_justify)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	zend_bool justify;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ob", &layout_zval, pango_ce_pangolayout, &justify) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_justify(layout_object->layout, justify);
}

/* }}} */

/* {{{ proto bool pango_layout_get_justify(PangoLayout layout)
 	   proto bool PangoLayout::getJustify(void)
	   Returns whether text will be justified or not in the current layout */
PHP_FUNCTION(pango_layout_get_justify)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	RETURN_BOOL(pango_layout_get_justify(layout_object->layout));
}
/* }}} */

/* {{{ proto void pango_layout_set_wrap(PangoLayout layout, long wrap)
 	   proto void PangoLayout::setWrap(bool wrap)
	   Sets how each line should be wrapped. */
PHP_FUNCTION(pango_layout_set_wrap)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	long wrap;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &layout_zval, pango_ce_pangolayout, &wrap) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_wrap(layout_object->layout, wrap);
}

/* }}} */

/* {{{ proto bool pango_layout_get_wrap(PangoLayout layout)
 	   proto bool PangoLayout::getWrap(void)
	   Returns how text will be wrapped or not in the current layout */
PHP_FUNCTION(pango_layout_get_wrap)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	RETURN_LONG(pango_layout_get_wrap(layout_object->layout));
}
/* }}} */

/* {{{ proto bool pango_layout_is_wrapped(PangoLayout layout)
 	   proto bool PangoLayout::isWrapped(void)
	   Returns how text will be wrapped or not in the current layout */
PHP_FUNCTION(pango_layout_is_wrapped)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	RETURN_BOOL(pango_layout_is_wrapped(layout_object->layout));
}
/* }}} */

/* {{{ proto void pango_layout_set_indent(PangoLayout layout, int indent)
 	   proto void PangoLayout::setWrap(bool indent)
	   Sets how far each paragraph should be indented. */
PHP_FUNCTION(pango_layout_set_indent)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	long indent;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &layout_zval, pango_ce_pangolayout, &indent) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_indent(layout_object->layout, indent);
}

/* }}} */

/* {{{ proto bool pango_layout_get_indent(PangoLayout layout)
 	   proto bool PangoLayout::getIndent(void)
	   Returns how text will be indentped or not in the current layout */
PHP_FUNCTION(pango_layout_get_indent)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	RETURN_LONG(pango_layout_get_indent(layout_object->layout));
}
/* }}} */

/* {{{ proto void pango_layout_set_spacing(PangoLayout layout, int spacing)
 	   proto void PangoLayout::setWrap(bool spacing)
	   Sets the line spacing for the paragraph */
PHP_FUNCTION(pango_layout_set_spacing)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	long spacing;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &layout_zval, pango_ce_pangolayout, &spacing) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_spacing(layout_object->layout, spacing);
}

/* }}} */

/* {{{ proto bool pango_layout_get_spacing(PangoLayout layout)
 	   proto bool PangoLayout::getWrap(void)
	   Returns the spacing for the current layout */
PHP_FUNCTION(pango_layout_get_spacing)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	RETURN_LONG(pango_layout_get_spacing(layout_object->layout));
}
/* }}} */

/* {{{ proto void pango_layout_set_ellipsize(PangoLayout layout, long ellipsize)
 	   proto void PangoLayout::setEllipsize(long ellipsize)
	   Sets the ellipsize mode for the layout */
PHP_FUNCTION(pango_layout_set_ellipsize)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	long ellipsize;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &layout_zval, pango_ce_pangolayout, &ellipsize) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_ellipsize(layout_object->layout, ellipsize);
}

/* }}} */

/* {{{ proto long pango_layout_get_ellipsize(PangoLayout layout)
 	   proto long PangoLayout::getEllipsize(void)
	   Returns the ellipsize for the current layout */
PHP_FUNCTION(pango_layout_get_ellipsize)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	RETURN_LONG(pango_layout_get_ellipsize(layout_object->layout));
}
/* }}} */

/* {{{ proto bool pango_layout_is_ellipsized(PangoLayout layout)
 	   proto bool PangoLayout::isEllipsized(void)
	   Returns the ellipsize for the current layout */
PHP_FUNCTION(pango_layout_is_ellipsized)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	RETURN_BOOL(pango_layout_is_ellipsized(layout_object->layout));
}
/* }}} */

/* {{{ proto array pango_layout_get_lines(PangoLayout layout)
	   proto array PangoLayout::getLines(void)
	   Returns an array of PangoLayoutLines representing each line of the layout */
PHP_FUNCTION(pango_layout_get_lines)
{
	zval *layout_zval = NULL, *elem = NULL;
	pango_layout_object *layout_object;
	GSList *lines, *iter;
	zend_class_entry *layoutline_ce;


	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	lines = pango_layout_get_lines(layout_object->layout);
	
	layoutline_ce = php_pango_get_layoutline_ce();
	array_init(return_value);
	for(iter = lines; iter != NULL; iter = iter->next) {
		elem = php_pango_make_layoutline_zval((PangoLayoutLine *)iter->data, layout_zval TSRMLS_CC);
		add_next_index_zval(return_value, elem);
	}
}
/* }}} */

/* {{{ proto array pango_layout_get_line(PangoLayout layout, long line)
	   proto array PangoLayout::getLine(long line)
	   Returns a particular PangoLayoutLine from the layout */
PHP_FUNCTION(pango_layout_get_line)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	pango_layoutline_object *layoutline_object;
	PangoLayoutLine *layoutline;
	long line_number = 0;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &layout_zval, pango_ce_pangolayout, &line_number) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)
	
	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	layoutline = pango_layout_get_line(layout_object->layout, line_number);

	if(!layoutline) {
		RETURN_FALSE;
	}

	zval_dtor(return_value);
	*return_value = *php_pango_make_layoutline_zval(layoutline, layout_zval TSRMLS_CC);
}

/* {{{ proto array pango_layout_get_line_count(PangoLayout layout, long line)
	   proto array PangoLayout::getLineCount(long line)
	   Returns the number of PangoLayoutLines in the layout */
PHP_FUNCTION(pango_layout_get_line_count)
{
	zval *layout_zval = NULL, *elem = NULL;
	pango_layout_object *layout_object;
	pango_layoutline_object *layoutline_object;
	PangoLayoutLine *layoutline;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)
	
	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	RETURN_LONG(pango_layout_get_line_count(layout_object->layout));
}

/* {{{ Object creation/destruction functions */
static void pango_layout_object_destroy(void *object TSRMLS_DC)
{
    pango_layout_object *layout = (pango_layout_object *)object;
    zend_hash_destroy(layout->std.properties);
    FREE_HASHTABLE(layout->std.properties);

    if(layout->cairo_context) {
        Z_DELREF_P(layout->cairo_context);
        layout->cairo_context = NULL;
    }    

    if(layout->layout){
		g_object_unref(layout->layout);
    }    
    efree(object);
}

static zend_object_value pango_layout_object_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    pango_layout_object *layout;
    zval *temp;

    layout = ecalloc(1, sizeof(pango_layout_object));

    layout->std.ce = ce;
	layout->layout = NULL;
	layout->cairo_context = NULL;

    ALLOC_HASHTABLE(layout->std.properties);
    zend_hash_init(layout->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(layout->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &temp, sizeof(zval *)); 
    retval.handle = zend_objects_store_put(layout, NULL, (zend_objects_free_object_storage_t)pango_layout_object_destroy, NULL TSRMLS_CC);
    retval.handlers = &pango_std_object_handlers;
    return retval;
}
/* }}} */

/* {{{ pango_layout_class_functions */
const zend_function_entry pango_layout_methods[] = {
	PHP_ME(PangoLayout, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME_MAPPING(getContext, pango_layout_get_context, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME_MAPPING(setText, pango_layout_set_text, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getText, pango_layout_get_text, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getWidth, pango_layout_get_width, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getHeight, pango_layout_get_height, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getSize, pango_layout_get_size, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getPixelSize, pango_layout_get_pixel_size, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getExtents, pango_layout_get_extents, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getPixelExtents, pango_layout_get_pixel_extents, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setWidth, pango_layout_set_width, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setHeight, pango_layout_set_height, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setMarkup, pango_layout_set_markup, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(updateLayout, pango_cairo_update_layout, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(showLayout, pango_cairo_show_layout, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(layoutPath, pango_cairo_layout_path, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setFontDescription, pango_layout_set_font_description, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setJustify, pango_layout_set_justify, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getJustify, pango_layout_get_justify, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setWrap, pango_layout_set_wrap, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getWrap, pango_layout_get_wrap, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(isWrapped, pango_layout_is_wrapped, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setIndent, pango_layout_set_indent, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getIndent, pango_layout_get_indent, NULL, ZEND_ACC_PUBLIC)
 	PHP_ME_MAPPING(setSpacing, pango_layout_set_spacing, NULL, ZEND_ACC_PUBLIC)
 	PHP_ME_MAPPING(getSpacing, pango_layout_get_spacing, NULL, ZEND_ACC_PUBLIC)
 	PHP_ME_MAPPING(setEllipsize, pango_layout_set_ellipsize, NULL, ZEND_ACC_PUBLIC)
 	PHP_ME_MAPPING(getEllipsize, pango_layout_get_ellipsize, NULL, ZEND_ACC_PUBLIC)
 	PHP_ME_MAPPING(isEllipsized, pango_layout_is_ellipsized, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(contextChanged, pango_layout_context_changed, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getLines, pango_layout_get_lines, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getLine, pango_layout_get_line, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getLineCount, pango_layout_get_line_count, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(pango_layout)
{
	zend_class_entry layout_ce;
	zend_class_entry wrapmode_ce;
	zend_class_entry ellipsizemode_ce;

	INIT_CLASS_ENTRY(layout_ce, "PangoLayout", pango_layout_methods);
	pango_ce_pangolayout = zend_register_internal_class(&layout_ce TSRMLS_CC);
	pango_ce_pangolayout->create_object = pango_layout_object_new;
	
	INIT_CLASS_ENTRY(wrapmode_ce, "PangoWrapMode", NULL);
	pango_ce_pangowrapmode = zend_register_internal_class(&wrapmode_ce TSRMLS_CC);
	pango_ce_pangowrapmode->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_WRAPMODE_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangowrapmode, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_WRAPMODE_LONG_CONST("WORD", PANGO_WRAP_WORD);
	REGISTER_PANGO_WRAPMODE_LONG_CONST("CHAR", PANGO_WRAP_CHAR);
	REGISTER_PANGO_WRAPMODE_LONG_CONST("WORD_CHAR", PANGO_WRAP_WORD_CHAR);

	INIT_CLASS_ENTRY(ellipsizemode_ce, "PangoEllipsizeMode", NULL);
	pango_ce_pangoellipsizemode = zend_register_internal_class(&ellipsizemode_ce TSRMLS_CC);
	pango_ce_pangoellipsizemode->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_ELLIPSIZEMODE_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangoellipsizemode, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_ELLIPSIZEMODE_LONG_CONST("NONE", PANGO_ELLIPSIZE_NONE);
	REGISTER_PANGO_ELLIPSIZEMODE_LONG_CONST("START", PANGO_ELLIPSIZE_START);
	REGISTER_PANGO_ELLIPSIZEMODE_LONG_CONST("MIDDLE", PANGO_ELLIPSIZE_MIDDLE);
	REGISTER_PANGO_ELLIPSIZEMODE_LONG_CONST("END", PANGO_ELLIPSIZE_END);

	return SUCCESS;
}

/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
