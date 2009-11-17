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

/* {{{ pango_layout_class_functions */
const zend_function_entry pango_layout_methods[] = {
	PHP_ME(PangoLayout, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(pango_layout)
{
	zend_class_entry layout_ce;

	INIT_CLASS_ENTRY(layout_ce, "PangoLayout", pango_layout_methods);
	pango_ce_pangolayout = zend_register_internal_class(&layout_ce TSRMLS_CC);
	pango_ce_pangolayout->create_object = pango_layout_object_new;

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
