#!/bin/bash

TRD_DIR=$PWD/3rd-party

./configure $* \
    --add-module=${TRD_DIR}/ngx_http_neteye_security \
    --add-module=${TRD_DIR}/ngx_http_naxsi_neteye_helper \
    --add-module=${TRD_DIR}/naxsi/naxsi_src \
    --add-module=${TRD_DIR}/nginx-upstream-fair \
    --add-module=${TRD_DIR}/headers-more-nginx-module \
    --add-module=${TRD_DIR}/ngx_http_substitutions_filter_module \
    --add-module=${TRD_DIR}/nginx_tcp_proxy_module \
    --add-module=${TRD_DIR}/ngx_http_upstream_fastest \
    --add-module=${TRD_DIR}/ngx_http_upstream_persistence \
    --add-module=${TRD_DIR}/ngx_http_session \
    --add-module=${TRD_DIR}/ngx_http_robot_mitigation \
    --add-module=${TRD_DIR}/ngx_http_status_page \
    --add-module=${TRD_DIR}/ngx_http_if_extend \
    --add-module=${TRD_DIR}/ngx_http_cache_extend
