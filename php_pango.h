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
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 252479 2008-02-07 19:39:50Z iliaa $ */

#ifndef PHP_PANGO_H
#define PHP_PANGO_H

#define PHP_PANGO_VERSION "0.1.0-dev"

extern zend_module_entry pango_module_entry;
#define phpext_pango_ptr &pango_module_entry

#ifdef PHP_WIN32
#	define PHP_PANGO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_PANGO_API __attribute__ ((visibility("default")))
#else
#	define PHP_PANGO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(pango);
PHP_MSHUTDOWN_FUNCTION(pango);
PHP_MINFO_FUNCTION(pango);

PHP_FUNCTION(pango_version);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(pango)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(pango)
*/

/* In every utility function you add that needs to use variables 
   in php_pango_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as PANGO_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define PANGO_G(v) TSRMG(pango_globals_id, zend_pango_globals *, v)
#else
#define PANGO_G(v) (pango_globals.v)
#endif

#endif	/* PHP_PANGO_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
