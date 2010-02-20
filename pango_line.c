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

zend_class_entry *pango_ce_pangolayoutline;

PHP_PANGO_API zend_class_entry* php_pango_get_layoutline_ce()
{
	return pango_ce_pangolayoutline;
}

/* {{{ Object creation/destruction functions */
static void pango_layoutline_object_destroy(void *object TSRMLS_DC)
{
    pango_layoutline_object *layoutline = (pango_layoutline_object *)object;
    zend_hash_destroy(layoutline->std.properties);
    FREE_HASHTABLE(layoutline->std.properties);

    efree(object);
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
    retval.handlers = &pango_std_object_handlers;
    return retval;
}
/* }}} */

/* {{{ pango_layout_class_functions */
const zend_function_entry pango_layoutline_methods[] = {
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

	return SUCCESS;
}
