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

/* $Id: $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pango.h"

/* If you declare any globals in php_pango.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(pango)
*/

zend_class_entry *pango_ce_pango;
zend_object_handlers pango_std_object_handlers;

#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 16, 0)
/* proto int pango_version(void) {{{
   returns the Pango version */
PHP_FUNCTION(pango_version)
{
	if(zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(pango_version());
}
/* }}} */

/* proto int pango_version_string(void) {{{
   returns the Pango version as a string */
PHP_FUNCTION(pango_version_string)
{
	if(zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_STRING((char *)pango_version_string(), 1);
}
/* }}} */
#endif
#endif

/* {{{ pango_functions[]
 *
 * Every user visible function must have an entry in pango_functions[].
 */
const zend_function_entry pango_functions[] = {
#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 16, 0)
	PHP_FE(pango_version, NULL)
	PHP_FE(pango_version_string, NULL)
#endif
#endif
	PHP_FE(pango_layout_new, NULL)
	PHP_FE(pango_cairo_update_layout, NULL)
	PHP_FE(pango_cairo_show_layout, NULL)
#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 6, 0)
	PHP_FE(pango_context_set_base_gravity, NULL)
	PHP_FE(pango_context_get_base_gravity, NULL)
	PHP_FE(pango_context_get_gravity, NULL)
	PHP_FE(pango_context_set_gravity_hint, NULL)
	PHP_FE(pango_context_get_gravity_hint, NULL)
#endif
#endif
	PHP_FE(pango_layout_get_context, NULL)
	PHP_FE(pango_layout_set_text, NULL)
	PHP_FE(pango_layout_get_text, NULL)
	PHP_FE(pango_layout_set_markup, NULL)
	PHP_FE(pango_layout_get_width, NULL)
	PHP_FE(pango_layout_get_size, NULL)
	PHP_FE(pango_layout_get_pixel_size, NULL)
	PHP_FE(pango_layout_get_extents, NULL)
	PHP_FE(pango_layout_get_pixel_extents, NULL)
	PHP_FE(pango_layout_set_width, NULL)
#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 20, 0)
	PHP_FE(pango_layout_set_height, NULL)
	PHP_FE(pango_layout_get_height, NULL)
#endif
#endif
	PHP_FE(pango_layout_set_font_description, NULL)
	PHP_FE(pango_layout_set_alignment, NULL)
	PHP_FE(pango_layout_get_alignment, NULL)
	PHP_FE(pango_layout_set_justify, NULL)
	PHP_FE(pango_layout_get_justify, NULL)
	PHP_FE(pango_layout_set_wrap, NULL)
	PHP_FE(pango_layout_get_wrap, NULL)
#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 16, 0)
	PHP_FE(pango_layout_is_wrapped, NULL)
#endif
#endif
	PHP_FE(pango_layout_set_indent, NULL)
	PHP_FE(pango_layout_get_indent, NULL)
 	PHP_FE(pango_layout_set_spacing, NULL)
 	PHP_FE(pango_layout_get_spacing, NULL)
	PHP_FE(pango_layout_set_ellipsize, NULL)
	PHP_FE(pango_layout_get_ellipsize, NULL)
#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 16, 0)
	PHP_FE(pango_layout_is_ellipsized, NULL)
#endif
#endif
	PHP_FE(pango_layout_get_lines, NULL)
	PHP_FE(pango_layout_get_line, NULL)
	PHP_FE(pango_layout_get_line_count, NULL)
	PHP_FE(pango_layout_get_baseline, NULL)
	PHP_FE(pango_layout_context_changed, NULL)

	/* PangoLayoutLine functions */
	PHP_FE(pango_layout_line_get_extents, NULL)
	PHP_FE(pango_layout_line_get_pixel_extents, NULL)
	PHP_FE(pango_cairo_show_layout_line, NULL)

	/* PangoFontDescription functions */
	PHP_FE(pango_font_description_new, NULL)
	PHP_FE(pango_font_description_get_variant, NULL)
	PHP_FE(pango_font_description_set_variant, NULL)

/*	PHP_FE(pango_font_description_new, NULL) */
	{NULL, NULL, NULL}	
};
/* }}} */

const zend_function_entry pango_methods[] = {
#ifdef PANGO_VERSION
#if PANGO_VERSION >= PANGO_VERSION_ENCODE(1, 16, 0)
	PHP_ME_MAPPING(version, pango_version, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME_MAPPING(versionString, pango_version_string, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
#endif
#endif
	{NULL, NULL, NULL}
};

/* {{{ pango_module_entry
 */
zend_module_entry pango_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"pango",
	pango_functions,
	PHP_MINIT(pango),
	PHP_MSHUTDOWN(pango),
	NULL,
	NULL,
	PHP_MINFO(pango),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_PANGO_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PANGO
ZEND_GET_MODULE(pango)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pango)
{
	zend_class_entry pango_ce;

    memcpy(&pango_std_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    pango_std_object_handlers.clone_obj = NULL;

	INIT_CLASS_ENTRY(pango_ce, "Pango", pango_methods);
	pango_ce_pango = zend_register_internal_class(&pango_ce TSRMLS_CC);
	pango_ce_pango->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS | ZEND_ACC_FINAL_CLASS;

#define REGISTER_PANGO_LONG_CONST(const_name, value) \
	zend_declare_class_constant_long(pango_ce_pango, const_name, sizeof(const_name)-1, (long)value TSRMLS_CC); \
	REGISTER_LONG_CONSTANT(#value,  value,  CONST_CS | CONST_PERSISTENT);

	REGISTER_PANGO_LONG_CONST("SCALE", PANGO_SCALE);

	PHP_MINIT(pango_error)(INIT_FUNC_ARGS_PASSTHRU);
 	PHP_MINIT(pango_context)(INIT_FUNC_ARGS_PASSTHRU); 
	PHP_MINIT(pango_layout)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(pango_font)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(pango_line)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pango)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pango)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Pango text rendering support", "enabled");
	php_info_print_table_colspan_header(2,
#ifdef COMPILE_DL_PANGO
			"compiled as dynamic module"
#else
			"compiled as static module"
#endif
	);
#ifdef PANGO_VERSION_STRING
	php_info_print_table_row(2, "Pango version", PANGO_VERSION_STRING);
#else
	php_info_print_table_row(2, "Pango version", "Unknown");
#endif
	php_info_print_table_row(2, "Extension version", PHP_PANGO_VERSION);
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
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
