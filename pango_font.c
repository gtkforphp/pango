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

/* {{{ proto PangoFontDescription pango_font_description_new(string description)
 	   Creates a new font description object. This merges pango_font_description_new 
	   and pango_font_description_from_string. */
PHP_FUNCTION(pango_font_description_new)
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

	object_init_ex(return_value, pango_ce_pangofontdescription);
	if(text_len) {
		fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(return_value TSRMLS_CC);
		fontdesc_object->fontdesc = pango_font_description_from_string(text);
	} else {
		fontdesc_object->fontdesc = pango_font_description_new();
	}

	if(fontdesc_object->fontdesc == NULL) {
	}
}
/* }}} */

/* {{{ proto long pango_font_description_get_variant(PangoFontDescription fontdesc)
 	   proto long PangoFontDescription::getVariant()
	   Sets the variant of the font description. */
PHP_FUNCTION(pango_font_description_get_variant)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &fontdesc_zval, pango_ce_pangofontdescription) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	RETURN_LONG(pango_font_description_get_variant(fontdesc_object->fontdesc));
}

/* }}} */

/* {{{ proto void pango_font_description_set_variant (PangoFontDescription fontdesc, long variant)
 	   proto void PangoFontDescription::setVariant(long variant)
	   Sets the variant of the layout. */
PHP_FUNCTION(pango_font_description_set_variant)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	long variant;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &fontdesc_zval, pango_ce_pangofontdescription, &variant) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	pango_font_description_set_variant(fontdesc_object->fontdesc, variant);
}

/* }}} */

/* {{{ proto boolean pango_font_description_equal(PangoFontDescription fontdesc1, PangoFontDescription fontdesc2)
 	   proto boolean PangoFontDescription::equal(PangoFontDescription fontdesc2)
	   Compares two font description objects for equality. 
       */
PHP_FUNCTION(pango_font_description_equal)
{
	zval *fontdesc_zval = NULL, *fontdesc2_zval = NULL;
	pango_fontdesc_object *fontdesc_object, *fontdesc2_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO", 
			   &fontdesc_zval, pango_ce_pangofontdescription, &fontdesc2_zval, pango_ce_pangofontdescription) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	fontdesc2_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc2_zval TSRMLS_CC);
	RETURN_BOOL(pango_font_description_equal(fontdesc_object->fontdesc, fontdesc2_object->fontdesc));
}

/* }}} */

/* {{{ proto void pango_font_description_set_family (PangoFontDescription fontdesc, string family)
 	   proto void PangoFontDescription::setFamily(string family)
	   Sets the family name field of a font description. */
PHP_FUNCTION(pango_font_description_set_family)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	const char *family;
	long family_len;

	PHP_PANGO_ERROR_HANDLING(FALSE)
		if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &fontdesc_zval, pango_ce_pangofontdescription, &family, &family_len) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	pango_font_description_set_family(fontdesc_object->fontdesc, family);
}

/* }}} */

/* {{{ proto string pango_font_description_get_family (PangoFontDescription fontdesc)
 	   proto string PangoFontDescription::getFamily()
	   Sets the family name field of a font description. */
PHP_FUNCTION(pango_font_description_get_family)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	const char *family;

	PHP_PANGO_ERROR_HANDLING(FALSE)
		if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &fontdesc_zval, pango_ce_pangofontdescription) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	if ((family = pango_font_description_get_family(fontdesc_object->fontdesc))) {
		RETURN_STRING((char *)family, 1);
	}
	RETURN_FALSE;
}

/* }}} */

/* {{{ proto void pango_font_description_set_size (PangoFontDescription fontdesc, long size)
 	   proto void PangoFontDescription::setSize(long size)
	   Sets the size field of a font description. */
PHP_FUNCTION(pango_font_description_set_size)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	long size;

	PHP_PANGO_ERROR_HANDLING(FALSE)
		if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &fontdesc_zval, pango_ce_pangofontdescription, &size) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	pango_font_description_set_size(fontdesc_object->fontdesc, size);
}

/* }}} */

/* {{{ proto long pango_font_description_get_size (PangoFontDescription fontdesc)
 	   proto long PangoFontDescription::getSize()
	   Gets the size field of a font description. */
PHP_FUNCTION(pango_font_description_get_size)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	long size;

	PHP_PANGO_ERROR_HANDLING(FALSE)
		if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &fontdesc_zval, pango_ce_pangofontdescription) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	RETURN_LONG (pango_font_description_get_size(fontdesc_object->fontdesc));
}

/* }}} */

/* {{{ proto long pango_font_description_get_style(PangoFontDescription fontdesc)
 	   proto long PangoFontDescription::getStyle()
	   Gets the style of the font description. */
PHP_FUNCTION(pango_font_description_get_style)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &fontdesc_zval, pango_ce_pangofontdescription) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	RETURN_LONG(pango_font_description_get_style(fontdesc_object->fontdesc));
}

/* }}} */

/* {{{ proto void pango_font_description_set_style (PangoFontDescription fontdesc, long style)
 	   proto void PangoFontDescription::setVariant(long style)
	   Sets the style of the layout. */
PHP_FUNCTION(pango_font_description_set_style)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	long style;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &fontdesc_zval, pango_ce_pangofontdescription, &style) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	pango_font_description_set_style(fontdesc_object->fontdesc, style);
}

/* }}} */


/* {{{ proto long pango_font_description_get_weight(PangoFontDescription fontdesc)
 	   proto long PangoFontDescription::getWeight()
	   Sets the weight of the font description. */
PHP_FUNCTION(pango_font_description_get_weight)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &fontdesc_zval, pango_ce_pangofontdescription) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	RETURN_LONG(pango_font_description_get_weight(fontdesc_object->fontdesc));
}

/* }}} */

/* {{{ proto void pango_font_description_set_weight (PangoFontDescription fontdesc, long weight)
 	   proto void PangoFontDescription::setWeight(long weight)
	   Sets the weight of the layout. */
PHP_FUNCTION(pango_font_description_set_weight)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	long weight;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &fontdesc_zval, pango_ce_pangofontdescription, &weight) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	pango_font_description_set_weight(fontdesc_object->fontdesc, weight);
}

/* }}} */


/* {{{ proto long pango_font_description_get_stretch(PangoFontDescription fontdesc)
 	   proto long PangoFontDescription::getStretch()
	   Sets the stretch of the font description. */
PHP_FUNCTION(pango_font_description_get_stretch)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &fontdesc_zval, pango_ce_pangofontdescription) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	RETURN_LONG(pango_font_description_get_stretch(fontdesc_object->fontdesc));
}

/* }}} */

/* {{{ proto void pango_font_description_set_stretch (PangoFontDescription fontdesc, long stretch)
 	   proto void PangoFontDescription::setStretch(long stretch)
	   Sets the stretch of the layout. */
PHP_FUNCTION(pango_font_description_set_stretch)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	long stretch;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &fontdesc_zval, pango_ce_pangofontdescription, &stretch) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	pango_font_description_set_stretch(fontdesc_object->fontdesc, stretch);
}

/* }}} */

/* {{{ proto string pango_font_description_to_string (PangoFontDescription fontdesc)
 	   proto string PangoFontDescription::toString()
	   Creates a string representation of a font description. */
PHP_FUNCTION(pango_font_description_to_string)
{
	zval *fontdesc_zval = NULL;
	pango_fontdesc_object *fontdesc_object;
	const char *result;

	PHP_PANGO_ERROR_HANDLING(FALSE)
		if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &fontdesc_zval, pango_ce_pangofontdescription) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	fontdesc_object = (pango_fontdesc_object *)zend_object_store_get_object(fontdesc_zval TSRMLS_CC);
	if ((result = pango_font_description_to_string(fontdesc_object->fontdesc))) {
		RETURN_STRING((char *)result, 1);
	}
	RETURN_FALSE;
}

/* }}} */




/* {{{ Object creation/destruction functions */
static void pango_fontdesc_object_destroy(void *object TSRMLS_DC)
{
    pango_fontdesc_object *fontdesc = (pango_fontdesc_object *)object;
    zend_hash_destroy(fontdesc->std.properties);
    FREE_HASHTABLE(fontdesc->std.properties);

    if(fontdesc->fontdesc) {
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
	PHP_ME_MAPPING(getVariant, pango_font_description_get_variant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setVariant, pango_font_description_set_variant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(equal, pango_font_description_equal, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setFamily, pango_font_description_set_family, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getFamily, pango_font_description_get_family, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setSize, pango_font_description_set_size, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getSize, pango_font_description_get_size, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getStyle, pango_font_description_get_style, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setStyle, pango_font_description_set_style, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getWeight, pango_font_description_get_weight, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setWeight, pango_font_description_set_weight, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getStretch, pango_font_description_get_stretch, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setStretch, pango_font_description_set_stretch, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(toString, pango_font_description_to_string, NULL, ZEND_ACC_PUBLIC)
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
	REGISTER_PANGO_WEIGHT_LONG_CONST("NORMAL", PANGO_WEIGHT_NORMAL);
	REGISTER_PANGO_WEIGHT_LONG_CONST("SEMIBOLD", PANGO_WEIGHT_SEMIBOLD);
	REGISTER_PANGO_WEIGHT_LONG_CONST("BOLD", PANGO_WEIGHT_BOLD);
	REGISTER_PANGO_WEIGHT_LONG_CONST("ULTRABOLD", PANGO_WEIGHT_ULTRABOLD);
	REGISTER_PANGO_WEIGHT_LONG_CONST("HEAVY", PANGO_WEIGHT_HEAVY);

#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 24, 0)
	REGISTER_PANGO_WEIGHT_LONG_CONST("THIN", PANGO_WEIGHT_THIN);
	REGISTER_PANGO_WEIGHT_LONG_CONST("BOOK", PANGO_WEIGHT_BOOK);
	REGISTER_PANGO_WEIGHT_LONG_CONST("MEDIUM", PANGO_WEIGHT_MEDIUM);
	REGISTER_PANGO_WEIGHT_LONG_CONST("ULTRAHEAVY", PANGO_WEIGHT_ULTRAHEAVY);
#endif

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
