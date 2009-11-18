#define DDEBUG 0

#include "ddebug.h"

#include "ngx_http_headers_more_filter_module.h"
#include "ngx_http_headers_more_output_headers.h"
/* #include "ngx_http_headers_more_input_headers.h" */

#include <ngx_config.h>

/* config handlers */

static void *ngx_http_headers_more_create_conf(ngx_conf_t *cf);

static char *ngx_http_headers_more_merge_conf(ngx_conf_t *cf,
    void *parent, void *child);

/* filter handlers */

static ngx_int_t ngx_http_headers_more_filter_init(ngx_conf_t *cf);

static ngx_command_t  ngx_http_headers_more_filter_commands[] = {

    { ngx_string("more_set_headers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_1MORE,
      ngx_http_headers_more_set_headers,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL},

    { ngx_string("more_clear_headers"),
      NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF
                        |NGX_CONF_1MORE,
      ngx_http_headers_more_clear_headers,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL},

      ngx_null_command
};

static ngx_http_module_t  ngx_http_headers_more_filter_module_ctx = {
    NULL,                                  /* preconfiguration */
    ngx_http_headers_more_filter_init,     /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_headers_more_create_conf,     /* create location configuration */
    ngx_http_headers_more_merge_conf       /* merge location configuration */
};

ngx_module_t  ngx_http_headers_more_filter_module = {
    NGX_MODULE_V1,
    &ngx_http_headers_more_filter_module_ctx,   /* module context */
    ngx_http_headers_more_filter_commands,      /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;

static ngx_int_t
ngx_http_headers_more_filter(ngx_http_request_t *r)
{
    ngx_int_t                       rc;
    /* ngx_str_t                       value; */
    ngx_uint_t                      i;
    ngx_http_headers_more_conf_t    *conf;
    ngx_http_headers_more_cmd_t     *cmd;

    conf = ngx_http_get_module_loc_conf(r, ngx_http_headers_more_filter_module);

    if (conf->cmds) {
        cmd = conf->cmds->elts;
        for (i = 0; i < conf->cmds->nelts; i++) {
            rc = ngx_http_headers_more_exec_cmd(r, &cmd[i]);

            if (rc != NGX_OK) {
                return rc;
            }
        }
    }

    return ngx_http_next_header_filter(r);
}

static ngx_int_t
ngx_http_headers_more_filter_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_headers_more_filter;

    return NGX_OK;
}

static void *
ngx_http_headers_more_create_conf(ngx_conf_t *cf)
{
    ngx_http_headers_more_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_headers_more_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *     conf->cmds = NULL;
     */

    return conf;
}


static char *
ngx_http_headers_more_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_uint_t                   i, orig_len;
    ngx_http_headers_more_cmd_t  *prev_cmd, *cmd;
    ngx_http_headers_more_conf_t *prev = parent;
    ngx_http_headers_more_conf_t *conf = child;

    if (conf->cmds == NULL || conf->cmds->nelts == 0) {
        conf->cmds = prev->cmds;
    } else if (prev->cmds && prev->cmds->nelts) {
        orig_len = conf->cmds->nelts;

        (void) ngx_array_push_n(conf->cmds, prev->cmds->nelts);

        cmd = conf->cmds->elts;
        for (i = 0; i < orig_len; i++) {
            cmd[conf->cmds->nelts - 1 - i] = cmd[orig_len - 1 - i];
        }

        prev_cmd = prev->cmds->elts;
        for (i = 0; i < prev->cmds->nelts; i++) {
            cmd[i] = prev_cmd[i];
        }
    }

    return NGX_CONF_OK;
}

