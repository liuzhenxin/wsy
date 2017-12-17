/*
 * =====================================================================================
 *
 *       Filename:  z_sign.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2016 17:35:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef Z_SIGN_H
#define Z_SIGN_H

#include "mupdf/pdf.h"

typedef struct z_device_s z_device;
typedef struct z_pdf_sign_appearance_s z_pdf_sign_appearance;

typedef fz_buffer* (*z_sign_get_digest_fn)(fz_context *ctx, pdf_document *doc, z_device *device, char *filename, pdf_obj *byte_range);

typedef void (*z_sign_signature_fn)(fz_context *, z_device *, pdf_document*, pdf_annot *annot, z_pdf_sign_appearance *app);

typedef void (*z_sign_set_appearance_fn)(fz_context *, pdf_document *, pdf_annot *, z_pdf_sign_appearance *);
typedef void*(*z_sign_keep_appearance_fn)(fz_context*, z_pdf_sign_appearance *);
typedef void (*z_sign_drop_appearance_fn)(fz_context*, z_pdf_sign_appearance *);

typedef void (*z_sign_setsign_value_fn)(fz_context*, z_device *);
typedef void (*z_sign_releasedevice_fn)(fz_context*, z_device *);

struct z_pdf_sign_appearance_s {
    int refcount;
    fz_rect rect;
    void *app;
    char *text;
    z_sign_set_appearance_fn  set_app;
    z_sign_keep_appearance_fn keep_app;
    z_sign_drop_appearance_fn drop_app;
};

struct z_device_s {
    int refcount;
    z_sign_signature_fn       do_sign;
	z_sign_get_digest_fn      get_digest;
    z_sign_releasedevice_fn   release;
};

z_device *z_keep_device(fz_context *ctx, z_device *device);
void z_drop_device(fz_context *ctx, z_device *device);

z_pdf_sign_appearance *z_pdf_keep_sign_apperance(fz_context *ctx, z_pdf_sign_appearance *app);
void z_pdf_drop_sign_appreance(fz_context *ctx, z_pdf_sign_appearance *app);
z_pdf_sign_appearance *z_pdf_new_sign_appearance_with_image(fz_context *ctx, fz_image *image, fz_rect r, char *text);
z_pdf_sign_appearance *z_pdf_new_sign_appearance_with_paths(fz_context *ctx, z_fpoint_arraylist *al, fz_rect r,char *text);

// this func not support updtate page view
void z_pdf_dosign(fz_context *ctx, z_device *device, pdf_document *doc,int pageno, z_pdf_sign_appearance *app);
// this function support update page view
void z_pdf_dosign_with_page(fz_context *ctx, z_device *device, pdf_document *doc,pdf_page *page, z_pdf_sign_appearance *app);

void z_pdf_write_sign(fz_context *ctx, char *file, fz_buffer *buf, int ofs, int len);
void z_fz_stream_save(fz_context *ctx, fz_stream *stm, char *filename); 

#endif
