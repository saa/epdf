#include "erl_nif.h"
#include <stdbool.h>
#include <stdio.h>
#include <wkhtmltox/pdf.h>


char *from_html(const char *html_path, const char *pdf_path) {
    wkhtmltopdf_global_settings * gs;
    wkhtmltopdf_object_settings * os;
    wkhtmltopdf_converter * c;

    wkhtmltopdf_init(false);

    gs = wkhtmltopdf_create_global_settings();
    wkhtmltopdf_set_global_setting(gs, "out", pdf_path);

    os = wkhtmltopdf_create_object_settings();
    wkhtmltopdf_set_object_setting(os, "page", html_path);

    c = wkhtmltopdf_create_converter(gs);

    wkhtmltopdf_add_object(c, os, NULL);

    if (!wkhtmltopdf_convert(c)) {
        return "error";
    };

    wkhtmltopdf_destroy_converter(c);
    wkhtmltopdf_deinit();

    return "ok";
}


static ERL_NIF_TERM from_html_nif(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    char html_path[4096];
    char pdf_path[4096];
    char *ret;

    if(!enif_get_string(env, argv[0], html_path, sizeof(html_path), ERL_NIF_LATIN1) ||
       !enif_get_string(env, argv[1], pdf_path, sizeof(pdf_path), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    ret = from_html(html_path, pdf_path);
    return enif_make_atom(env, ret);
}


static ErlNifFunc nif_funcs[] = {
    {"from_html", 2, from_html_nif}
};


ERL_NIF_INIT(epdf, nif_funcs, NULL, NULL, NULL, NULL)
