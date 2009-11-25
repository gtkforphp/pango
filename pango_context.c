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

zend_class_entry *pango_ce_pangocontext;
zend_class_entry *pango_ce_pangodirection;

PHP_PANGO_API zend_class_entry* php_pango_get_context_ce() {
	return pango_ce_pangocontext;
}

/* {{{ Object creation/destruction functions */
static void pango_context_object_destroy(void *object TSRMLS_DC)
{
    pango_context_object *context = (pango_context_object *)object;
    zend_hash_destroy(context->std.properties);
    FREE_HASHTABLE(context->std.properties);

    if(context->context){
		g_object_unref(context->context);
    }    
    efree(object);
}

static zend_object_value pango_context_object_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    pango_context_object *context;
    zval *temp;

    context = ecalloc(1, sizeof(pango_context_object));

    context->std.ce = ce;
	context->context = NULL;

    ALLOC_HASHTABLE(context->std.properties);
    zend_hash_init(context->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(context->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &temp, sizeof(zval *)); 
    retval.handle = zend_objects_store_put(context, NULL, (zend_objects_free_object_storage_t)pango_context_object_destroy, NULL TSRMLS_CC);
    retval.handlers = &pango_std_object_handlers;
    return retval;
}
/* }}} */

const zend_function_entry pango_context_methods[] = {
	{ NULL, NULL, NULL }
};

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(pango_context)
{
	zend_class_entry context_ce;
	zend_class_entry direction_ce;

	INIT_CLASS_ENTRY(context_ce, "PangoContext", pango_context_methods);
	pango_ce_pangocontext = zend_register_internal_class(&context_ce TSRMLS_CC);
	pango_ce_pangocontext->create_object = pango_context_object_new;

	INIT_CLASS_ENTRY(direction_ce, "PangoDirection", NULL);
	pango_ce_pangodirection = zend_register_internal_class(&direction_ce TSRMLS_CC);
	pango_ce_pangodirection->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_DIRECTION_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangodirection, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);
	
	REGISTER_PANGO_DIRECTION_LONG_CONST("LTR", PANGO_DIRECTION_LTR);
	REGISTER_PANGO_DIRECTION_LONG_CONST("RTL", PANGO_DIRECTION_RTL);
	REGISTER_PANGO_DIRECTION_LONG_CONST("TTB_LTR", PANGO_DIRECTION_TTB_LTR);
	REGISTER_PANGO_DIRECTION_LONG_CONST("TTB_RTL", PANGO_DIRECTION_TTB_RTL);
	REGISTER_PANGO_DIRECTION_LONG_CONST("WEAK_LTR", PANGO_DIRECTION_WEAK_LTR);
	REGISTER_PANGO_DIRECTION_LONG_CONST("WEAK_RTL", PANGO_DIRECTION_WEAK_RTL);
	REGISTER_PANGO_DIRECTION_LONG_CONST("NEUTRAL", PANGO_DIRECTION_NEUTRAL);

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
