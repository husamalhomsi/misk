// img2doc.c - embed an image in a document

#include <cairo.h>
#include <cairo-pdf.h>
#include <stdio.h>

double in2pt(double in) {
  return in * 72;
}

int main(int argc, char *argv[]) {
  if (argc != 3)
    return 1;

  double margin     = in2pt(0.5);
  double img_width  = in2pt(10);
  double img_height = in2pt(7.5);
  double doc_width  = img_width  + 2 * margin;
  double doc_height = img_height + 2 * margin;

  const char *img_filename = argv[1];
  const char *doc_filename = argv[2];

  cairo_surface_t *img = cairo_image_surface_create_from_png(img_filename);
  cairo_surface_t *doc = cairo_pdf_surface_create(doc_filename, doc_width, doc_height);
  cairo_t *cr = cairo_create(doc);

  if (cairo_surface_status(img) != CAIRO_STATUS_SUCCESS)
    return 2;

  if (cairo_surface_status(doc) != CAIRO_STATUS_SUCCESS)
    return 3;

  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS)
    return 4;

  double scale_x = img_width  / cairo_image_surface_get_width(img);
  double scale_y = img_height / cairo_image_surface_get_height(img);
  double scale = scale_x < scale_y ? scale_x : scale_y;

  cairo_translate(cr, margin, margin);
  cairo_scale(cr, scale, scale);
  cairo_set_source_surface(cr, img, 0, 0);
  cairo_paint(cr);
  cairo_destroy(cr);
  cairo_surface_destroy(img);
  cairo_surface_destroy(doc);

  printf("Wrote %s\n", doc_filename);
}
