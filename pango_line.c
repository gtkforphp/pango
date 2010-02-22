/*
  +----------------------------------------------------------------------+
  | PHP Version 5														|
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group								|
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,	  |
  | that is bundled with this package in the file LICENSE, and is		|
  | available through the world-wide-web at the following url:		   |
  | http://www.php.net/license/3_01.txt								  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to		  |
  | license@php.net so we can mail you a copy immediately.			   |
  +----------------------------------------------------------------------+
  | Author:  Michael Maclean <mgdm@php.net>							  |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_pango.h"

#include <string.h>
#include "zend_exceptions.h"

zend_class_entry *pango_ce_pangolayoutline;

static zend_object_handlers pango_layoutline_object_handlers;

PHP_PANGO_API zend_class_entry* php_pango_get_layoutline_ce()
{
	return pango_ce_pangolayoutline;
}

/*  {{{ php_pango_make_layoutline_zval 
	Convenience function to handle setting all the properties etc */
PHP_PANGO_API zval* php_pango_make_layoutline_zval(PangoLayoutLine *line, zval *layout TSRMLS_DC)
{
	zval *return_value, *length, *is_paragraph_start, *resolved_dir;
	pango_layoutline_object *layoutline_object;

	MAKE_STD_ZVAL(return_value);
	object_init_ex(return_value, pango_ce_pangolayoutline);
	layoutline_object = (pango_layoutline_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	layoutline_object->line = line;

	/* Optionally cache the PangoLayout zval for later */
	if(layout != NULL) {
		Z_ADDREF_P(layout);
		layoutline_object->layout_zval = layout;
	}

	MAKE_STD_ZVAL(length);
	ZVAL_LONG(length, line->length);
	zend_hash_update(Z_OBJPROP_P(return_value), "length", sizeof("length"), (void **) &length, sizeof(zval *), NULL);

	MAKE_STD_ZVAL(is_paragraph_start);
	ZVAL_BOOL(is_paragraph_start, line->is_paragraph_start);
	zend_hash_update(Z_OBJPROP_P(return_value), "is_paragraph_start", sizeof("is_paragraph_start"), (void **) &is_paragraph_start, sizeof(zval *), NULL);

	MAKE_STD_ZVAL(resolved_dir);
	ZVAL_LONG(resolved_dir, line->resolved_dir);
	zend_hash_update(Z_OBJPROP_P(return_value), "resolved_dir", sizeof("resolved_dir"), (void **) &resolved_dir, sizeof(zval *), NULL);

	return return_value;
}
/* }}} */

/* {{{ proto array pango_layout_line_get_extents(PangoLayoutLine line)
	   proto array PangoLayoutLine::getExtents()
	   Get the logical and ink extents for the line */
PHP_FUNCTION(pango_layout_line_get_extents)
{
	zval *layoutline_zval = NULL, *array = NULL;
	pango_layoutline_object *layoutline_object = NULL;
	PangoRectangle ink, logical;
	
	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",
				&layoutline_zval, pango_ce_pangolayoutline) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}

	layoutline_object = (pango_layoutline_object *)zend_object_store_get_object(layoutline_zval TSRMLS_CC);
	pango_layout_line_get_extents(layoutline_object->line, &ink, &logical);

	array_init(return_value);
	MAKE_STD_ZVAL(array);
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

/* {{{ proto array pango_layout_line_get_pixel_extents(PangoLayoutLine line)
	   proto array PangoLayoutLine::getPixelExtents()
	   Get the logical and ink extents for the line, in device units */
PHP_FUNCTION(pango_layout_line_get_pixel_extents)
{
	zval *layoutline_zval = NULL, *array = NULL;
	pango_layoutline_object *layoutline_object = NULL;
	PangoRectangle ink, logical;
	
	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O",
				&layoutline_zval, pango_ce_pangolayoutline) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}

	layoutline_object = (pango_layoutline_object *)zend_object_store_get_object(layoutline_zval TSRMLS_CC);
	pango_layout_line_get_pixel_extents(layoutline_object->line, &ink, &logical);

	array_init(return_value);
	MAKE_STD_ZVAL(array);
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

/* {{{ proto void pango_cairo_show_layout_line(PangoLayoutLine line, CairoContext context)
	   proto void PangoLayoutLine::show([CairoContext context])
	   Draws a PangoLayoutLine in the specified cairo context. If no context
	   is specified, use the cached one from when the PangoLayoutLine was created */
PHP_FUNCTION(pango_cairo_show_layout_line)
{
	zval *layoutline_zval = NULL, *layout_zval = NULL, *cairocontext_zval = NULL;	
	pango_layoutline_object *layoutline_object = NULL;
	pango_layout_object *layout_object = NULL;
	cairo_context_object *cairocontext_object = NULL;
	
	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O|O",
				&layoutline_zval, pango_ce_pangolayoutline,
				&cairocontext_zval, php_cairo_get_context_ce()) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}

	layoutline_object = (pango_layoutline_object *)zend_object_store_get_object(layoutline_zval TSRMLS_CC);

	if(cairocontext_zval == NULL) {
		layout_zval = layoutline_object->layout_zval;
		layout_object = zend_object_store_get_object(layout_zval TSRMLS_CC);
		cairocontext_zval = layout_object->cairo_context;
	}	
	cairocontext_object = zend_object_store_get_object(cairocontext_zval TSRMLS_CC);

	pango_cairo_show_layout_line(cairocontext_object->context, layoutline_object->line);
}
/* }}} */

/* {{{ Object creation/destruction functions */
static void pango_layoutline_object_destroy(void *object TSRMLS_DC)
{
	pango_layoutline_object *layoutline = (pango_layoutline_object *)object;
	zend_hash_destroy(layoutline->std.properties);
	FREE_HASHTABLE(layoutline->std.properties);

	if(layoutline->layout_zval != NULL) {
		Z_DELREF_P(layoutline->layout_zval);
	}

	efree(object);
}

static void php_pango_layoutline_write_property(zval *object, zval *member, zval *value TSRMLS_DC)
{
	zval tmp_member;

	if (member->type != IS_STRING) {
		tmp_member = *member;
		zval_copy_ctor(&tmp_member);
		convert_to_string(&tmp_member);
		member = &tmp_member;
	}

	if(((Z_STRLEN_P(member) == sizeof("length") - 1  && !memcmp(Z_STRVAL_P(member), "length",  sizeof("length")))
				|| (Z_STRLEN_P(member) == sizeof("is_paragraph_start") - 1 && !memcmp(Z_STRVAL_P(member), "is_paragraph_start", sizeof("is_paragraph_start")))
				|| (Z_STRLEN_P(member) == sizeof("resolved_dir") - 1 && !memcmp(Z_STRVAL_P(member), "resolved_dir", sizeof("resolved_dir")))))
	{
		zend_throw_exception_ex(pango_ce_pangoexception, 0 TSRMLS_CC,
				"Cannot set read-only property %s::$%s", Z_OBJCE_P(object)->name, Z_STRVAL_P(member));
	} else {
		pango_std_object_handlers.write_property(object, member, value TSRMLS_CC);
	}

	if (member == &tmp_member) {
		zval_dtor(member);
	}
}


static zend_object_value pango_layoutline_object_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	pango_layoutline_object *layoutline;
	zval *temp;

	layoutline = ecalloc(1, sizeof(pango_layoutline_object));

	layoutline->std.ce = ce;
	layoutline->line = NULL;

	ALLOC_HASHTABLE(layoutline->std.properties);
	zend_hash_init(layoutline->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_copy(layoutline->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &temp, sizeof(zval *)); 
	retval.handle = zend_objects_store_put(layoutline, NULL, (zend_objects_free_object_storage_t)pango_layoutline_object_destroy, NULL TSRMLS_CC);
	retval.handlers = &pango_layoutline_object_handlers;
	return retval;
}
/* }}} */

/* {{{ pango_layout_class_functions */
const zend_function_entry pango_layoutline_methods[] = {
	PHP_ME_MAPPING(getExtents, pango_layout_line_get_extents, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME_MAPPING(getPixelExtents, pango_layout_line_get_pixel_extents, NULL, ZEND_ACC_PUBLIC) 
	PHP_ME_MAPPING(show, pango_cairo_show_layout_line, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(pango_line)
{
	zend_class_entry line_ce;

	INIT_CLASS_ENTRY(line_ce, "PangoLayoutLine", pango_layoutline_methods);
	pango_ce_pangolayoutline = zend_register_internal_class(&line_ce TSRMLS_CC);
	pango_ce_pangolayoutline->create_object = pango_layoutline_object_new;
	memcpy(&pango_layoutline_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	pango_layoutline_object_handlers.write_property = php_pango_layoutline_write_property;

	return SUCCESS;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

