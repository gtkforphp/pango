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

zend_class_entry *pango_ce_pangofontdescription;
zend_class_entry *pango_ce_pangostyle;
zend_class_entry *pango_ce_pangoweight;
zend_class_entry *pango_ce_pangovariant;
zend_class_entry *pango_ce_pangostretch;
zend_class_entry *pango_ce_pangofontmask;

/* {{{ proto PangoFontDescription::__construct(string description)
 	   Creates a new font description object. This merges pango_font_description_new 
	   and pango_font_description_from_string. */
PHP_METHOD(PangoFontDescription, __construct)
{
	pango_fontdesc_object *fontdesc_object = NULL;
	const char *text;
	long text_len = -1;

	PHP_PANGO_ERROR_HANDLING(TRUE)
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &text, &text_len) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(TRUE)
		return;
	}

	if(text_len) {
		fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
		fontdesc_object->fontdesc = pango_font_description_from_string(text);
	} else {
		fontdesc_object->fontdesc = pango_font_description_new();
	}

	if(!fontdesc_object->fontdesc) {
		zend_throw_exception(pango_ce_pangoexception, "Could not create the Pango font description", 0 TSRMLS_CC);
		return;
	}
}
/* }}} */

/* {{{ Object creation/destruction functions */
static void pango_fontdesc_object_destroy(void *object TSRMLS_DC)
{
    pango_fontdesc_object *fontdesc = (pango_fontdesc_object *)object;
    zend_hash_destroy(fontdesc->std.properties);
    FREE_HASHTABLE(fontdesc->std.properties);

    if(fontdesc->fontdesc){
		pango_font_description_free(fontdesc->fontdesc);
    }    
    efree(object);
}

static zend_object_value pango_fontdesc_object_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    pango_fontdesc_object *fontdesc;
    zval *temp;

    fontdesc = ecalloc(1, sizeof(pango_fontdesc_object));

    fontdesc->std.ce = ce;
	fontdesc->fontdesc = NULL;

    ALLOC_HASHTABLE(fontdesc->std.properties);
    zend_hash_init(fontdesc->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(fontdesc->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &temp, sizeof(zval *)); 
    retval.handle = zend_objects_store_put(fontdesc, NULL, (zend_objects_free_object_storage_t)pango_fontdesc_object_destroy, NULL TSRMLS_CC);
    retval.handlers = &pango_std_object_handlers;
    return retval;
}
/* }}} */

/* {{{ pango fontdescription class functions */
const zend_function_entry pango_fontdesc_methods[] = {
	PHP_ME(PangoFontDescription, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	{ NULL, NULL, NULL }
};

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(pango_font)
{
	zend_class_entry fontdescription_ce;
	zend_class_entry style_ce;
	zend_class_entry weight_ce;
	zend_class_entry variant_ce;
	zend_class_entry stretch_ce;
	zend_class_entry fontmask_ce;

	INIT_CLASS_ENTRY(fontdescription_ce, "PangoFontDescription", pango_fontdesc_methods);
	pango_ce_pangofontdescription = zend_register_internal_class(&fontdescription_ce TSRMLS_CC);
	pango_ce_pangofontdescription->create_object = pango_fontdesc_object_new;

	INIT_CLASS_ENTRY(style_ce, "PangoStyle", NULL);
	pango_ce_pangostyle = zend_register_internal_class(&style_ce TSRMLS_CC);
	pango_ce_pangostyle->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_STYLE_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangostyle, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_STYLE_LONG_CONST("NORMAL", PANGO_STYLE_NORMAL);
	REGISTER_PANGO_STYLE_LONG_CONST("OBLIQUE", PANGO_STYLE_OBLIQUE);
	REGISTER_PANGO_STYLE_LONG_CONST("ITALIC", PANGO_STYLE_ITALIC);

	INIT_CLASS_ENTRY(weight_ce, "PangoWeight", NULL);
	pango_ce_pangoweight = zend_register_internal_class(&weight_ce TSRMLS_CC);
	pango_ce_pangoweight->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_WEIGHT_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangoweight, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_WEIGHT_LONG_CONST("ULTRALIGHT", PANGO_WEIGHT_ULTRALIGHT);
	REGISTER_PANGO_WEIGHT_LONG_CONST("LIGHT", PANGO_WEIGHT_LIGHT);
	REGISTER_PANGO_WEIGHT_LONG_CONST("BOOK", PANGO_WEIGHT_BOOK);
	REGISTER_PANGO_WEIGHT_LONG_CONST("NORMAL", PANGO_WEIGHT_NORMAL);
	REGISTER_PANGO_WEIGHT_LONG_CONST("MEDIUM", PANGO_WEIGHT_MEDIUM);
	REGISTER_PANGO_WEIGHT_LONG_CONST("SEMIBOLD", PANGO_WEIGHT_SEMIBOLD);
	REGISTER_PANGO_WEIGHT_LONG_CONST("BOLD", PANGO_WEIGHT_BOLD);
	REGISTER_PANGO_WEIGHT_LONG_CONST("ULTRABOLD", PANGO_WEIGHT_ULTRABOLD);
	REGISTER_PANGO_WEIGHT_LONG_CONST("HEAVY", PANGO_WEIGHT_HEAVY);
	REGISTER_PANGO_WEIGHT_LONG_CONST("ULTRAHEAVY", PANGO_WEIGHT_ULTRAHEAVY);

	INIT_CLASS_ENTRY(variant_ce, "PangoVariant", NULL);
	pango_ce_pangovariant = zend_register_internal_class(&variant_ce TSRMLS_CC);
	pango_ce_pangovariant->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_VARIANT_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangovariant, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_VARIANT_LONG_CONST("NORMAL", PANGO_VARIANT_NORMAL);
	REGISTER_PANGO_VARIANT_LONG_CONST("SMALL_CAPS", PANGO_VARIANT_SMALL_CAPS);

	INIT_CLASS_ENTRY(stretch_ce, "PangoStretch", NULL);
	pango_ce_pangostretch = zend_register_internal_class(&stretch_ce TSRMLS_CC);
	pango_ce_pangostretch->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_STRETCH_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangostretch, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_STRETCH_LONG_CONST("ULTRA_CONDENSED", PANGO_STRETCH_ULTRA_CONDENSED);
	REGISTER_PANGO_STRETCH_LONG_CONST("EXTRA_CONDENSED", PANGO_STRETCH_EXTRA_CONDENSED);
	REGISTER_PANGO_STRETCH_LONG_CONST("CONDENSED", PANGO_STRETCH_CONDENSED);
	REGISTER_PANGO_STRETCH_LONG_CONST("SEMI_CONDENSED", PANGO_STRETCH_SEMI_CONDENSED);
	REGISTER_PANGO_STRETCH_LONG_CONST("NORMAL", PANGO_STRETCH_NORMAL);
	REGISTER_PANGO_STRETCH_LONG_CONST("SEMI_EXPANDED", PANGO_STRETCH_SEMI_EXPANDED);
	REGISTER_PANGO_STRETCH_LONG_CONST("EXPANDED", PANGO_STRETCH_EXPANDED);
	REGISTER_PANGO_STRETCH_LONG_CONST("EXTRA_EXPANDED", PANGO_STRETCH_EXTRA_EXPANDED);
	REGISTER_PANGO_STRETCH_LONG_CONST("ULTRA_EXPANDED", PANGO_STRETCH_ULTRA_EXPANDED);

	INIT_CLASS_ENTRY(fontmask_ce, "PangoFontMask", NULL);
	pango_ce_pangofontmask = zend_register_internal_class(&fontmask_ce TSRMLS_CC);
	pango_ce_pangofontmask->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_FONT_MASK_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pangofontmask, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_FONT_MASK_LONG_CONST("FAMILY", PANGO_FONT_MASK_FAMILY);
	REGISTER_PANGO_FONT_MASK_LONG_CONST("STYLE", PANGO_FONT_MASK_STYLE);
	REGISTER_PANGO_FONT_MASK_LONG_CONST("VARIANT", PANGO_FONT_MASK_VARIANT);
	REGISTER_PANGO_FONT_MASK_LONG_CONST("WEIGHT", PANGO_FONT_MASK_WEIGHT);
	REGISTER_PANGO_FONT_MASK_LONG_CONST("STRETCH", PANGO_FONT_MASK_STRETCH);
	REGISTER_PANGO_FONT_MASK_LONG_CONST("SIZE", PANGO_FONT_MASK_SIZE);
	REGISTER_PANGO_FONT_MASK_LONG_CONST("GRAVITY", PANGO_FONT_MASK_GRAVITY);

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
