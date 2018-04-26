#pragma once

#ifndef WL_SHM_FORMAT_ENUM
#	define WL_SHM_FORMAT_ENUM
/**
 * @ingroup iface_wl_shm
 * pixel formats
 *
 * This describes the memory layout of an individual pixel.
 *
 * All renderers should support argb8888 and xrgb8888 but any other
 * formats are optional and may not be supported by the particular
 * renderer in use.
 *
 * The drm format codes match the macros defined in drm_fourcc.h.
 * The formats actually supported by the compositor will be
 * reported by the format event.
 */
enum wl_shm_format {
	/**
	 * 32-bit ARGB format, [31:0] A:R:G:B 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_ARGB8888 = 0,
	/**
	 * 32-bit RGB format, [31:0] x:R:G:B 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_XRGB8888 = 1,
	/**
	 * 8-bit color index format, [7:0] C
	 */
	WL_SHM_FORMAT_C8 = 0x20203843,
	/**
	 * 8-bit RGB format, [7:0] R:G:B 3:3:2
	 */
	WL_SHM_FORMAT_RGB332 = 0x38424752,
	/**
	 * 8-bit BGR format, [7:0] B:G:R 2:3:3
	 */
	WL_SHM_FORMAT_BGR233 = 0x38524742,
	/**
	 * 16-bit xRGB format, [15:0] x:R:G:B 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_XRGB4444 = 0x32315258,
	/**
	 * 16-bit xBGR format, [15:0] x:B:G:R 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_XBGR4444 = 0x32314258,
	/**
	 * 16-bit RGBx format, [15:0] R:G:B:x 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_RGBX4444 = 0x32315852,
	/**
	 * 16-bit BGRx format, [15:0] B:G:R:x 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_BGRX4444 = 0x32315842,
	/**
	 * 16-bit ARGB format, [15:0] A:R:G:B 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_ARGB4444 = 0x32315241,
	/**
	 * 16-bit ABGR format, [15:0] A:B:G:R 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_ABGR4444 = 0x32314241,
	/**
	 * 16-bit RBGA format, [15:0] R:G:B:A 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_RGBA4444 = 0x32314152,
	/**
	 * 16-bit BGRA format, [15:0] B:G:R:A 4:4:4:4 little endian
	 */
	WL_SHM_FORMAT_BGRA4444 = 0x32314142,
	/**
	 * 16-bit xRGB format, [15:0] x:R:G:B 1:5:5:5 little endian
	 */
	WL_SHM_FORMAT_XRGB1555 = 0x35315258,
	/**
	 * 16-bit xBGR 1555 format, [15:0] x:B:G:R 1:5:5:5 little endian
	 */
	WL_SHM_FORMAT_XBGR1555 = 0x35314258,
	/**
	 * 16-bit RGBx 5551 format, [15:0] R:G:B:x 5:5:5:1 little endian
	 */
	WL_SHM_FORMAT_RGBX5551 = 0x35315852,
	/**
	 * 16-bit BGRx 5551 format, [15:0] B:G:R:x 5:5:5:1 little endian
	 */
	WL_SHM_FORMAT_BGRX5551 = 0x35315842,
	/**
	 * 16-bit ARGB 1555 format, [15:0] A:R:G:B 1:5:5:5 little endian
	 */
	WL_SHM_FORMAT_ARGB1555 = 0x35315241,
	/**
	 * 16-bit ABGR 1555 format, [15:0] A:B:G:R 1:5:5:5 little endian
	 */
	WL_SHM_FORMAT_ABGR1555 = 0x35314241,
	/**
	 * 16-bit RGBA 5551 format, [15:0] R:G:B:A 5:5:5:1 little endian
	 */
	WL_SHM_FORMAT_RGBA5551 = 0x35314152,
	/**
	 * 16-bit BGRA 5551 format, [15:0] B:G:R:A 5:5:5:1 little endian
	 */
	WL_SHM_FORMAT_BGRA5551 = 0x35314142,
	/**
	 * 16-bit RGB 565 format, [15:0] R:G:B 5:6:5 little endian
	 */
	WL_SHM_FORMAT_RGB565 = 0x36314752,
	/**
	 * 16-bit BGR 565 format, [15:0] B:G:R 5:6:5 little endian
	 */
	WL_SHM_FORMAT_BGR565 = 0x36314742,
	/**
	 * 24-bit RGB format, [23:0] R:G:B little endian
	 */
	WL_SHM_FORMAT_RGB888 = 0x34324752,
	/**
	 * 24-bit BGR format, [23:0] B:G:R little endian
	 */
	WL_SHM_FORMAT_BGR888 = 0x34324742,
	/**
	 * 32-bit xBGR format, [31:0] x:B:G:R 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_XBGR8888 = 0x34324258,
	/**
	 * 32-bit RGBx format, [31:0] R:G:B:x 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_RGBX8888 = 0x34325852,
	/**
	 * 32-bit BGRx format, [31:0] B:G:R:x 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_BGRX8888 = 0x34325842,
	/**
	 * 32-bit ABGR format, [31:0] A:B:G:R 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_ABGR8888 = 0x34324241,
	/**
	 * 32-bit RGBA format, [31:0] R:G:B:A 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_RGBA8888 = 0x34324152,
	/**
	 * 32-bit BGRA format, [31:0] B:G:R:A 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_BGRA8888 = 0x34324142,
	/**
	 * 32-bit xRGB format, [31:0] x:R:G:B 2:10:10:10 little endian
	 */
	WL_SHM_FORMAT_XRGB2101010 = 0x30335258,
	/**
	 * 32-bit xBGR format, [31:0] x:B:G:R 2:10:10:10 little endian
	 */
	WL_SHM_FORMAT_XBGR2101010 = 0x30334258,
	/**
	 * 32-bit RGBx format, [31:0] R:G:B:x 10:10:10:2 little endian
	 */
	WL_SHM_FORMAT_RGBX1010102 = 0x30335852,
	/**
	 * 32-bit BGRx format, [31:0] B:G:R:x 10:10:10:2 little endian
	 */
	WL_SHM_FORMAT_BGRX1010102 = 0x30335842,
	/**
	 * 32-bit ARGB format, [31:0] A:R:G:B 2:10:10:10 little endian
	 */
	WL_SHM_FORMAT_ARGB2101010 = 0x30335241,
	/**
	 * 32-bit ABGR format, [31:0] A:B:G:R 2:10:10:10 little endian
	 */
	WL_SHM_FORMAT_ABGR2101010 = 0x30334241,
	/**
	 * 32-bit RGBA format, [31:0] R:G:B:A 10:10:10:2 little endian
	 */
	WL_SHM_FORMAT_RGBA1010102 = 0x30334152,
	/**
	 * 32-bit BGRA format, [31:0] B:G:R:A 10:10:10:2 little endian
	 */
	WL_SHM_FORMAT_BGRA1010102 = 0x30334142,
	/**
	 * packed YCbCr format, [31:0] Cr0:Y1:Cb0:Y0 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_YUYV = 0x56595559,
	/**
	 * packed YCbCr format, [31:0] Cb0:Y1:Cr0:Y0 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_YVYU = 0x55595659,
	/**
	 * packed YCbCr format, [31:0] Y1:Cr0:Y0:Cb0 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_UYVY = 0x59565955,
	/**
	 * packed YCbCr format, [31:0] Y1:Cb0:Y0:Cr0 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_VYUY = 0x59555956,
	/**
	 * packed AYCbCr format, [31:0] A:Y:Cb:Cr 8:8:8:8 little endian
	 */
	WL_SHM_FORMAT_AYUV = 0x56555941,
	/**
	 * 2 plane YCbCr Cr:Cb format, 2x2 subsampled Cr:Cb plane
	 */
	WL_SHM_FORMAT_NV12 = 0x3231564e,
	/**
	 * 2 plane YCbCr Cb:Cr format, 2x2 subsampled Cb:Cr plane
	 */
	WL_SHM_FORMAT_NV21 = 0x3132564e,
	/**
	 * 2 plane YCbCr Cr:Cb format, 2x1 subsampled Cr:Cb plane
	 */
	WL_SHM_FORMAT_NV16 = 0x3631564e,
	/**
	 * 2 plane YCbCr Cb:Cr format, 2x1 subsampled Cb:Cr plane
	 */
	WL_SHM_FORMAT_NV61 = 0x3136564e,
	/**
	 * 3 plane YCbCr format, 4x4 subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV410 = 0x39565559,
	/**
	 * 3 plane YCbCr format, 4x4 subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU410 = 0x39555659,
	/**
	 * 3 plane YCbCr format, 4x1 subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV411 = 0x31315559,
	/**
	 * 3 plane YCbCr format, 4x1 subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU411 = 0x31315659,
	/**
	 * 3 plane YCbCr format, 2x2 subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV420 = 0x32315559,
	/**
	 * 3 plane YCbCr format, 2x2 subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU420 = 0x32315659,
	/**
	 * 3 plane YCbCr format, 2x1 subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV422 = 0x36315559,
	/**
	 * 3 plane YCbCr format, 2x1 subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU422 = 0x36315659,
	/**
	 * 3 plane YCbCr format, non-subsampled Cb (1) and Cr (2) planes
	 */
	WL_SHM_FORMAT_YUV444 = 0x34325559,
	/**
	 * 3 plane YCbCr format, non-subsampled Cr (1) and Cb (2) planes
	 */
	WL_SHM_FORMAT_YVU444 = 0x34325659,
};
#endif /* WL_SHM_FORMAT_ENUM */

const char* PixelBuffer::wl_shm_format_get_name(uint32_t format)
{
	switch (format)
	{
	case WL_SHM_FORMAT_ARGB8888: return "WL_SHM_FORMAT_ARGB8888";
	case WL_SHM_FORMAT_XRGB8888: return "WL_SHM_FORMAT_XRGB8888";
	case WL_SHM_FORMAT_C8: return "WL_SHM_FORMAT_C8";
	case WL_SHM_FORMAT_RGB332: return "WL_SHM_FORMAT_RGB332";
	case WL_SHM_FORMAT_BGR233: return "WL_SHM_FORMAT_BGR233";
	case WL_SHM_FORMAT_XRGB4444: return "WL_SHM_FORMAT_XRGB4444";
	case WL_SHM_FORMAT_XBGR4444: return "WL_SHM_FORMAT_XBGR4444";
	case WL_SHM_FORMAT_RGBX4444: return "WL_SHM_FORMAT_RGBX4444";
	case WL_SHM_FORMAT_BGRX4444: return "WL_SHM_FORMAT_BGRX4444";
	case WL_SHM_FORMAT_ARGB4444: return "WL_SHM_FORMAT_ARGB4444";
	case WL_SHM_FORMAT_ABGR4444: return "WL_SHM_FORMAT_ABGR4444";
	case WL_SHM_FORMAT_RGBA4444: return "WL_SHM_FORMAT_RGBA4444";
	case WL_SHM_FORMAT_BGRA4444: return "WL_SHM_FORMAT_BGRA4444";
	case WL_SHM_FORMAT_XRGB1555: return "WL_SHM_FORMAT_XRGB1555";
	case WL_SHM_FORMAT_XBGR1555: return "WL_SHM_FORMAT_XBGR1555";
	case WL_SHM_FORMAT_RGBX5551: return "WL_SHM_FORMAT_RGBX5551";
	case WL_SHM_FORMAT_BGRX5551: return "WL_SHM_FORMAT_BGRX5551";
	case WL_SHM_FORMAT_ARGB1555: return "WL_SHM_FORMAT_ARGB1555";
	case WL_SHM_FORMAT_ABGR1555: return "WL_SHM_FORMAT_ABGR1555";
	case WL_SHM_FORMAT_RGBA5551: return "WL_SHM_FORMAT_RGBA5551";
	case WL_SHM_FORMAT_BGRA5551: return "WL_SHM_FORMAT_BGRA5551";
	case WL_SHM_FORMAT_RGB565: return "WL_SHM_FORMAT_RGB565";
	case WL_SHM_FORMAT_BGR565: return "WL_SHM_FORMAT_BGR565";
	case WL_SHM_FORMAT_RGB888: return "WL_SHM_FORMAT_RGB888";
	case WL_SHM_FORMAT_BGR888: return "WL_SHM_FORMAT_BGR888";
	case WL_SHM_FORMAT_XBGR8888: return "WL_SHM_FORMAT_XBGR8888";
	case WL_SHM_FORMAT_RGBX8888: return "WL_SHM_FORMAT_RGBX8888";
	case WL_SHM_FORMAT_BGRX8888: return "WL_SHM_FORMAT_BGRX8888";
	case WL_SHM_FORMAT_ABGR8888: return "WL_SHM_FORMAT_ABGR8888";
	case WL_SHM_FORMAT_RGBA8888: return "WL_SHM_FORMAT_RGBA8888";
	case WL_SHM_FORMAT_BGRA8888: return "WL_SHM_FORMAT_BGRA8888";
	case WL_SHM_FORMAT_XRGB2101010: return "WL_SHM_FORMAT_XRGB2101010";
	case WL_SHM_FORMAT_XBGR2101010: return "WL_SHM_FORMAT_XBGR2101010";
	case WL_SHM_FORMAT_RGBX1010102: return "WL_SHM_FORMAT_RGBX1010102";
	case WL_SHM_FORMAT_BGRX1010102: return "WL_SHM_FORMAT_BGRX1010102";
	case WL_SHM_FORMAT_ARGB2101010: return "WL_SHM_FORMAT_ARGB2101010";
	case WL_SHM_FORMAT_ABGR2101010: return "WL_SHM_FORMAT_ABGR2101010";
	case WL_SHM_FORMAT_RGBA1010102: return "WL_SHM_FORMAT_RGBA1010102";
	case WL_SHM_FORMAT_BGRA1010102: return "WL_SHM_FORMAT_BGRA1010102";
	case WL_SHM_FORMAT_YUYV: return "WL_SHM_FORMAT_YUYV";
	case WL_SHM_FORMAT_YVYU: return "WL_SHM_FORMAT_YVYU";
	case WL_SHM_FORMAT_UYVY: return "WL_SHM_FORMAT_UYVY";
	case WL_SHM_FORMAT_VYUY: return "WL_SHM_FORMAT_VYUY";
	case WL_SHM_FORMAT_AYUV: return "WL_SHM_FORMAT_AYUV";
	case WL_SHM_FORMAT_NV12: return "WL_SHM_FORMAT_NV12";
	case WL_SHM_FORMAT_NV21: return "WL_SHM_FORMAT_NV21";
	case WL_SHM_FORMAT_NV16: return "WL_SHM_FORMAT_NV16";
	case WL_SHM_FORMAT_NV61: return "WL_SHM_FORMAT_NV61";
	case WL_SHM_FORMAT_YUV410: return "WL_SHM_FORMAT_YUV410";
	case WL_SHM_FORMAT_YVU410: return "WL_SHM_FORMAT_YVU410";
	case WL_SHM_FORMAT_YUV411: return "WL_SHM_FORMAT_YUV411";
	case WL_SHM_FORMAT_YVU411: return "WL_SHM_FORMAT_YVU411";
	case WL_SHM_FORMAT_YUV420: return "WL_SHM_FORMAT_YUV420";
	case WL_SHM_FORMAT_YVU420: return "WL_SHM_FORMAT_YVU420";
	case WL_SHM_FORMAT_YUV422: return "WL_SHM_FORMAT_YUV422";
	case WL_SHM_FORMAT_YVU422: return "WL_SHM_FORMAT_YVU422";
	case WL_SHM_FORMAT_YUV444: return "WL_SHM_FORMAT_YUV444";
	case WL_SHM_FORMAT_YVU444: return "WL_SHM_FORMAT_YVU444";
	default: return "WL_SHM_FORMAT_UNKNOWN";
	}
}
