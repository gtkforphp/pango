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

#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 6, 0)
zend_class_entry *pango_ce_pangogravity;
zend_class_entry *pango_ce_pangogravityhint;
#endif
#endif

PHP_PANGO_API zend_class_entry* php_pango_get_context_ce() {
	return pango_ce_pangocontext;
}

#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 6, 0)
/* {{{ proto void pango_context_set_base_gravity(PangoContext context, int gravity)
 	   proto void PangoContext::setBaseGravity(int gravity)
	   Sets the gravity to be used to lay out the text */
PHP_FUNCTION(pango_context_set_base_gravity)
{
	zval *context_zval = NULL;
	pango_context_object *context_object;
	long gravity;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &context_zval, pango_ce_pangocontext, &gravity) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	context_object = (pango_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	pango_context_set_base_gravity(context_object->context, gravity);
}
/* }}} */

/* {{{ proto void pango_context_get_base_gravity(PangoContext context, int gravity)
 	   proto void PangoContext::setBaseGravity(int gravity)
	   Gets the gravity to be used to lay out the text */
PHP_FUNCTION(pango_context_get_base_gravity)
{
	zval *context_zval = NULL;
	pango_context_object *context_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &context_zval, pango_ce_pangocontext) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	context_object = (pango_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	RETURN_LONG(pango_context_get_base_gravity(context_object->context));
}
/* }}} */

/* {{{ proto void pango_context_get_gravity(PangoContext context, int gravity)
 	   proto void PangoContext::getGravity(int gravity)
	   Gets the gravity to be used to lay out the text */
PHP_FUNCTION(pango_context_get_gravity)
{
	zval *context_zval = NULL;
	pango_context_object *context_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &context_zval, pango_ce_pangocontext) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	context_object = (pango_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	RETURN_LONG(pango_context_get_gravity(context_object->context));
}
/* }}} */

/* {{{ proto void pango_context_set_gravity_hint(PangoContext context, int gravityHint)
 	   proto void PangoContext::setGravityHint(int gravityHint)
	   Sets the gravity to be used to lay out the text */
PHP_FUNCTION(pango_context_set_gravity_hint)
{
	zval *context_zval = NULL;
	pango_context_object *context_object;
	long gravity;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &context_zval, pango_ce_pangocontext, &gravity) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	context_object = (pango_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	pango_context_set_gravity_hint(context_object->context, gravity);
}
/* }}} */

/* {{{ proto void pango_context_get_gravity_hint(PangoContext context)
 	   proto void PangoContext::getGravityHint()
	   Gets the gravity hint to be used to lay out the text */
PHP_FUNCTION(pango_context_get_gravity_hint)
{
	zval *context_zval = NULL;
	pango_context_object *context_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &context_zval, pango_ce_pangocontext) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	context_object = (pango_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	RETURN_LONG(pango_context_get_gravity_hint(context_object->context));
}
/* }}} */
#endif
#endif

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
    #if PHP_VERSION_ID < 50399
        zend_hash_copy(context->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &temp, sizeof(zval *));
    #else
        object_properties_init(&(context->std),ce);
    #endif
    retval.handle = zend_objects_store_put(context, NULL, (zend_objects_free_object_storage_t)pango_context_object_destroy, NULL TSRMLS_CC);
    retval.handlers = &pango_std_object_handlers;
    return retval;
}
/* }}} */

const zend_function_entry pango_context_methods[] = {
#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 6, 0)
	PHP_ME_MAPPING(setBaseGravity, pango_context_set_base_gravity, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getBaseGravity, pango_context_get_base_gravity, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getGravity, pango_context_get_gravity, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setGravityHint, pango_context_set_gravity_hint, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getGravityHint, pango_context_get_gravity_hint, NULL, ZEND_ACC_PUBLIC)
#endif
#endif
	{ NULL, NULL, NULL }
};

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(pango_context)
{
	zend_class_entry context_ce;
	zend_class_entry direction_ce;
#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 6, 0)
	zend_class_entry gravity_ce;
	zend_class_entry gravityhint_ce;
#endif
#endif

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

#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 6, 0)
	INIT_CLASS_ENTRY(gravity_ce, "PangoGravity", NULL);
	pango_ce_pangogravity = zend_register_internal_class(&gravity_ce TSRMLS_CC);
	pango_ce_pangogravity->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_GRAVITY_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangogravity, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_GRAVITY_LONG_CONST("SOUTH", PANGO_GRAVITY_SOUTH);
	REGISTER_PANGO_GRAVITY_LONG_CONST("EAST", PANGO_GRAVITY_EAST);
	REGISTER_PANGO_GRAVITY_LONG_CONST("NORTH", PANGO_GRAVITY_NORTH);
	REGISTER_PANGO_GRAVITY_LONG_CONST("WEST", PANGO_GRAVITY_WEST);
	REGISTER_PANGO_GRAVITY_LONG_CONST("AUTO", PANGO_GRAVITY_AUTO);

	INIT_CLASS_ENTRY(gravityhint_ce, "PangoGravityHint", NULL);
	pango_ce_pangogravityhint = zend_register_internal_class(&gravityhint_ce TSRMLS_CC);
	pango_ce_pangogravityhint->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_GRAVITY_HINT_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangogravityhint, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_GRAVITY_HINT_LONG_CONST("NATURAL", PANGO_GRAVITY_HINT_NATURAL);
	REGISTER_PANGO_GRAVITY_HINT_LONG_CONST("STRONG", PANGO_GRAVITY_HINT_STRONG);
	REGISTER_PANGO_GRAVITY_HINT_LONG_CONST("LINE", PANGO_GRAVITY_HINT_LINE);
#endif
#endif

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
