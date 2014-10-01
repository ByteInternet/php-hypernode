/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_hypernode.h"
#include "main/php_syslog.h"

#include "SAPI.h"
#include "fastcgi.h"

#include <netinet/tcp.h>
#include <netinet/in.h>
#include <syslog.h>


ZEND_DECLARE_MODULE_GLOBALS(hypernode)

/* True global resources - no need for thread safety here */
static int le_hypernode;

/* {{{ hypernode_functions[]
 *
 * Every user visible function must have an entry in hypernode_functions[].
 */
const zend_function_entry hypernode_functions[] = {
	PHP_FE_END	/* Must be the last line in hypernode_functions[] */
};
/* }}} */

/* {{{ hypernode_module_entry
 */
zend_module_entry hypernode_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"hypernode",
	hypernode_functions,
	PHP_MINIT(hypernode),
	PHP_MSHUTDOWN(hypernode),
	PHP_RINIT(hypernode),		/* Replace with NULL if there's nothing to do at request start */
	NULL,
	PHP_MINFO(hypernode),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_HYPERNODE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HYPERNODE
ZEND_GET_MODULE(hypernode)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("hypernode.kill_gone_requests",  "1", PHP_INI_ALL, OnUpdateBool, kill_gone_requests, zend_hypernode_globals, hypernode_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_hypernode_init_globals
 */
static void php_hypernode_init_globals(zend_hypernode_globals *hypernode_globals)
{
	hypernode_globals->kill_gone_requests = 1;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(hypernode)
{
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(hypernode)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(hypernode)
{
	if (!HYPERNODE_G(kill_gone_requests)) {
		return SUCCESS;
	}

	fcgi_request* request = (fcgi_request*) SG(server_context);
	if (!request) {
		// Not running FCGI request
		return SUCCESS;
	}

	struct tcp_info tcp_info;
	int tcp_info_length = sizeof(tcp_info);
	if (getsockopt(request->fd, IPPROTO_TCP, TCP_INFO, (void*)&tcp_info, (socklen_t*)&tcp_info_length)) {
		php_syslog(LOG_ERR, "Error determining socket status!");
		return SUCCESS;
	}

	if (tcp_info.tcpi_state == TCP_CLOSE_WAIT) {
		char *http_host, *remote_addr;

		// sapi_getenv will emalloc
		http_host = sapi_getenv("HTTP_HOST", strlen("HTTP_HOST") TSRMLS_CC);
		remote_addr = sapi_getenv("REMOTE_ADDR", strlen("REMOTE_ADDR") TSRMLS_CC);

		php_syslog(LOG_NOTICE, "Terminated request %s %s%s%s%s because client at %s is already gone",
				SG(request_info).request_method,
				http_host ? http_host : "-",
				SG(request_info).request_uri,
				(strlen(SG(request_info).query_string) > 0) ? "?" : "",
				SG(request_info).query_string,
				remote_addr ? remote_addr : "-");

		if (http_host) efree(http_host);
		if (remote_addr) efree(remote_addr);

		// By clearing the request method, the request will be terminated in fpm_main.c
		SG(request_info).request_method = NULL;
	}

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(hypernode)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "hypernode support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
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
