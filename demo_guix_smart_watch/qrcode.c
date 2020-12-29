#include <stdlib.h>
#include <stdio.h>

#include "demo_guix_smart_watch.h"
#include "gx_api.h"
#include "qrcode/qrcodegen.h"



static void printQr(const uint8_t qrcode[]) 
{
	int size = qrcodegen_getSize(qrcode);
	int border = 4;
	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
			fputs((qrcodegen_getModule(qrcode, x, y) ? "##" : "  "), stdout);
		}
		fputs("\n", stdout);
	}
	fputs("\n", stdout);
}

static void guix_show_qr(GX_WIDGET *widget, const uint8_t *qrcode)
{
	int size = qrcodegen_getSize(qrcode);
	GX_RECTANGLE rect = widget->gx_widget_size;
	GX_RECTANGLE fill_rect;
	int border = 4;

	rect.gx_rectangle_top += 50;
	fill_rect = rect;
	gx_context_brush_define(GX_COLOR_ID_WHITE, GX_COLOR_ID_WHITE, GX_BRUSH_SOLID_FILL | GX_BRUSH_ROUND);
	gx_context_brush_width_set(0);
	gx_canvas_rectangle_draw(&rect);

	gx_context_brush_define(GX_COLOR_ID_BLACK, GX_COLOR_ID_BLACK, GX_BRUSH_SOLID_FILL | GX_BRUSH_ROUND);
	gx_context_brush_width_set(4);
	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
			if (qrcodegen_getModule(qrcode, x, y)) {
				fill_rect.gx_rectangle_right = fill_rect.gx_rectangle_left + 3;
				fill_rect.gx_rectangle_bottom = fill_rect.gx_rectangle_top + 3;
				gx_canvas_rectangle_draw(&fill_rect);
				//_gx_canvas_line_draw(point.gx_point_x, point.gx_point_y, point.gx_point_x + 1, point.gx_point_y + 1);
				//gx_canvas_pixel_draw(point);
			}
			fill_rect.gx_rectangle_left += 4;
		}

		fill_rect.gx_rectangle_left = rect.gx_rectangle_left;
		fill_rect.gx_rectangle_top += 4;
	}
}

VOID qr_window_draw(GX_WINDOW* window)
{
	const char* text = "Hello, world!";                // User-supplied text
	uint8_t* buffer;

	buffer = calloc(1, qrcodegen_BUFFER_LEN_MAX * 2);
	if (!buffer)
		goto next;

	uint8_t *qrcode = buffer;
	uint8_t *tempBuffer = buffer + qrcodegen_BUFFER_LEN_MAX;
	bool ok = qrcodegen_encodeText(text, tempBuffer, qrcode, qrcodegen_Ecc_LOW,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok) {
		printQr(qrcode);
		guix_show_qr((GX_WIDGET*)window, qrcode);
	}

	free(buffer);

next:
	gx_widget_children_draw((GX_WIDGET*)window);
}