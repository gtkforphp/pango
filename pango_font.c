/*
   +----------------------------------------------------------------------+
   | PHP Version 5														|
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2009 The PHP Group								|
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

zend_class_entry *pango_ce_pangostyle;
zend_class_entry *pango_ce_pangoweight;
zend_class_entry *pango_ce_pangovariant;
zend_class_entry *pango_ce_pangostretch;
zend_class_entry *pango_ce_pangofontmask;

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(pango_font)
{
	zend_class_entry style_ce;
	zend_class_entry weight_ce;
	zend_class_entry variant_ce;
	zend_class_entry stretch_ce;
	zend_class_entry fontmask_ce;

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
