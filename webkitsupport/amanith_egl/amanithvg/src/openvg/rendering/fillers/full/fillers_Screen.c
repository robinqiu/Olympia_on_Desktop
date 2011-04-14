/****************************************************************************
** Copyright (C) 2004-2009 Mazatech S.r.l. All rights reserved.
**
** This file is part of AmanithVG software, an OpenVG implementation.
** This file is strictly confidential under the signed Mazatech Software
** Non-disclosure agreement and it's provided according to the signed
** Mazatech Software licensing agreement.
**
** Khronos and OpenVG are trademarks of The Khronos Group Inc.
** OpenGL is a registered trademark and OpenGL ES is a trademark of
** Silicon Graphics, Inc.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** For any information, please contact info@mazatech.com
**
****************************************************************************/

/*!
	\file fillers_Screen.c
	\brief VG_BLEND_SCREEN fillers, implementation ( \b generated \b file ).
	\author Matteo Muratori
	\author Michele Fabbri
*/

#if defined (RIM_VG_SRC)
#pragma push
#pragma arm
#endif

#if defined(AM_SRE) && !defined(AM_LITE_PROFILE)

#include "fillers.h"
#include "vggradients.h"
#include "pixel_utils.h"

/*!
	\brief Filler: paint type VG_PAINT_TYPE_COLOR, blend mode VG_BLEND_SCREEN, masking off ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_ColorScreen(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 oldCov;
	AMPixel32 oldDcaDa;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	ScaSa.value = paintGen->paintColor32;
	i = x1 - x0 + 1;
	cov = (*covLine);
	for (;;) {
		*covLine = 0;
		while (cov == 0) {
			scrPixels++;
			if (--i == 0) {
				covLine[1] = 0;
				return;
			}
			covLine++;
			cov += (*covLine);
		}
		*covLine = 0;
		DcaDa.value = oldDcaDa.value = *scrPixels;
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		if (cov != AM_RAS_MAX_COVERAGE) {
			tmp.value = amPxlLerp(((AMuint32)cov) >> AM_RAS_COVERAGE_PRECISION, DcaDa.value, tmp.value);
			do {
				*scrPixels++ = tmp.value;
				if (--i == 0) {
					covLine[1] = 0;
					return;
				}
				oldCov = cov;
				oldDcaDa = DcaDa;
				covLine++;
				cov += *covLine;
				DcaDa.value = *scrPixels;
			} while ((cov == oldCov) && (DcaDa.value == oldDcaDa.value));
		}
		else {
			do {
				*scrPixels++ = tmp.value;
				if (--i == 0) {
					covLine[1] = 0;
					return;
				}
				oldDcaDa = DcaDa;
				covLine++;
				cov += *covLine;
				DcaDa.value = *scrPixels;
			} while ((cov == AM_RAS_MAX_COVERAGE) && (DcaDa.value == oldDcaDa.value));
		}
	}
}

/*!
	\brief Filler: paint type VG_PAINT_TYPE_LINEAR_GRADIENT, blend mode VG_BLEND_SCREEN, masking off ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_LinGradScreen(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;
	AMint32 fixedU;
	const AMuint32 *gradientPixels;
	VGColorRampSpreadMode spreadMode;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));
#if RIM_VG_SRC
    ScaSa.value = 0;
#endif

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// initialize the paint generation
	amPaintLinGradInit(paintGen, x0 - 1, y);
	spreadMode = paintGen->paintDesc->paint->colorRampSpreadMode;
	gradientPixels = paintGen->gradTexture;
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_LinGradScreen:
	// update paint generators
	paintGen->Ux += paintGen->DUx;
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	tmpCov = cov >> AM_RAS_COVERAGE_PRECISION;
	if (tmpCov != 0) {
		fixedU = paintGen->Ux;
		AM_GRADIENTS_LINEAR_PIXEL_GET(ScaSa.value, fixedU)
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_LinGradScreen;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_LinGradScreen;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_LinGradScreen;
	else 
		covLine[j] = 0;
}

/*!
	\brief Filler: paint type VG_PAINT_TYPE_RADIAL_GRADIENT, blend mode VG_BLEND_SCREEN, masking off ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_RadGradScreen(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;
	AMint32 fixedU;
	const AMuint32 *gradientPixels;
	VGColorRampSpreadMode spreadMode;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));
#if RIM_VG_SRC
    ScaSa.value = 0;
#endif

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// initialize the paint generation
	amPaintRadGradInit(paintGen, x0 - 1, y);
	spreadMode = paintGen->paintDesc->paint->colorRampSpreadMode;
	gradientPixels = paintGen->gradTexture;
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_RadGradScreen:
	// update paint generators
	paintGen->Uf += paintGen->DUf;
	paintGen->Vf += paintGen->DVf;
	paintGen->A += paintGen->dA;
	paintGen->B += paintGen->dB;
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	tmpCov = cov >> AM_RAS_COVERAGE_PRECISION;
	if (tmpCov != 0) {
		AMfloat s = paintGen->radiusSqr * (paintGen->Uf * paintGen->Uf + paintGen->Vf * paintGen->Vf);
		AMfloat u = paintGen->A + amFastSqrtf(s - (paintGen->B *paintGen->B));
		fixedU = (AMint32)(u * paintGen->K);
		AM_GRADIENTS_RADIAL_PIXEL_GET(ScaSa.value, fixedU)
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_RadGradScreen;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_RadGradScreen;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_RadGradScreen;
	else 
		covLine[j] = 0;
}

/*!
	\brief Filler: paint type VG_PAINT_TYPE_PATTERN, blend mode VG_BLEND_SCREEN, masking off ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_PatternScreen(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// initialize the paint generation
	amPaintPatternInit(paintGen, x0 - 1, y);
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_PatternScreen:
	// update paint generators
	paintGen->Ux += paintGen->DUx;
	paintGen->Vx += paintGen->DVx;
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	tmpCov = cov >> AM_RAS_COVERAGE_PRECISION;
	if (tmpCov != 0) {
		paintGen->patternSamplerParams.x = paintGen->Ux;
		paintGen->patternSamplerParams.y = paintGen->Vx;
		ScaSa.value = paintGen->patternSampler(&paintGen->patternSamplerParams);
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_PatternScreen;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_PatternScreen;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_PatternScreen;
	else 
		covLine[j] = 0;
}

#if defined(VG_MZT_conical_gradient)
/*!
	\brief Filler: paint type VG_PAINT_TYPE_CONICAL_GRADIENT_MZT, blend mode VG_BLEND_SCREEN, masking off ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_ConGradScreen(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;
	AMint32 fixedU;
	AMint32 fixedV;
	const AMuint32 *gradientPixels;
	VGColorRampSpreadMode spreadMode;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));
#if RIM_VG_SRC
    ScaSa.value = 0;
#endif

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// initialize the paint generation
	amPaintConGradInit(paintGen, x0 - 1, y);
	spreadMode = paintGen->paintDesc->paint->colorRampSpreadMode;
	gradientPixels = paintGen->gradTexture;
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_ConGradScreen:
	// update paint generators
	paintGen->Uf += paintGen->DUf;
	paintGen->Vf += paintGen->DVf;
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	tmpCov = cov >> AM_RAS_COVERAGE_PRECISION;
	if (tmpCov != 0) {
		AMfloat x = paintGen->Uf;
		AMfloat y = paintGen->Vf;
		AMfloat l = x * x + y * y;
		AMuint32 atan2Val;
		AMuint32 atan2U;
		if (l != 0) {
			AMfloat xhalf = 0.5f * l;
			AMint32 f = *(AMint32 *)&l;
			f = 0x5f3759df - (f >> 1);
			l = *(AMfloat*)&f;
			l = l * (1.5f - xhalf * l * l);
			x *= l;
			y *= l;
		}
		x = x * AM_ATAN2_TABLE_SIZE_K + AM_ATAN2_TABLE_SIZE_K;
		y = y * AM_ATAN2_TABLE_SIZE_K + AM_ATAN2_TABLE_SIZE_K;
		fixedU = (AMint32)x;
		fixedV = (AMint32)y;
		AM_ASSERT(fixedU >= 0 && fixedU < AM_GRADIENTS_CONICAL_TEXTURE_WIDTH);
		AM_ASSERT(fixedV >= 0 && fixedV < AM_GRADIENTS_CONICAL_TEXTURE_WIDTH);
		AM_GRADIENTS_CONICAL_PIXEL_GET(ScaSa.value, fixedU, fixedV)
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_ConGradScreen;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_ConGradScreen;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_ConGradScreen;
	else 
		covLine[j] = 0;
}

#endif
/*!
	\brief Filler: paint type VG_PAINT_TYPE_COLOR, blend mode VG_BLEND_SCREEN, masking on ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_ColorScreenMask(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;
	AMuint8 *alphaPixels;
	AMuint32 mask;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// go to the start alpha pixel
	alphaPixels = surface->alphaMaskPixels + ofs0;
	ScaSa.value = paintGen->paintColor32;
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_ColorScreenMask:
	// update paint generators
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	mask = alphaPixels[j];
	tmpCov = ((cov >> AM_RAS_COVERAGE_PRECISION) * mask) >> 8;
	if (tmpCov != 0) {
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_ColorScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_ColorScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_ColorScreenMask;
	else 
		covLine[j] = 0;
}

/*!
	\brief Filler: paint type VG_PAINT_TYPE_LINEAR_GRADIENT, blend mode VG_BLEND_SCREEN, masking on ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_LinGradScreenMask(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;
	AMint32 fixedU;
	const AMuint32 *gradientPixels;
	VGColorRampSpreadMode spreadMode;
	AMuint8 *alphaPixels;
	AMuint32 mask;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));
#if RIM_VG_SRC
    ScaSa.value = 0;
#endif

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// go to the start alpha pixel
	alphaPixels = surface->alphaMaskPixels + ofs0;
	// initialize the paint generation
	amPaintLinGradInit(paintGen, x0 - 1, y);
	spreadMode = paintGen->paintDesc->paint->colorRampSpreadMode;
	gradientPixels = paintGen->gradTexture;
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_LinGradScreenMask:
	// update paint generators
	paintGen->Ux += paintGen->DUx;
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	mask = alphaPixels[j];
	tmpCov = ((cov >> AM_RAS_COVERAGE_PRECISION) * mask) >> 8;
	if (tmpCov != 0) {
		fixedU = paintGen->Ux;
		AM_GRADIENTS_LINEAR_PIXEL_GET(ScaSa.value, fixedU)
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_LinGradScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_LinGradScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_LinGradScreenMask;
	else 
		covLine[j] = 0;
}

/*!
	\brief Filler: paint type VG_PAINT_TYPE_RADIAL_GRADIENT, blend mode VG_BLEND_SCREEN, masking on ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_RadGradScreenMask(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;
	AMint32 fixedU;
	const AMuint32 *gradientPixels;
	VGColorRampSpreadMode spreadMode;
	AMuint8 *alphaPixels;
	AMuint32 mask;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));
#if RIM_VG_SRC
    ScaSa.value = 0;
#endif

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// go to the start alpha pixel
	alphaPixels = surface->alphaMaskPixels + ofs0;
	// initialize the paint generation
	amPaintRadGradInit(paintGen, x0 - 1, y);
	spreadMode = paintGen->paintDesc->paint->colorRampSpreadMode;
	gradientPixels = paintGen->gradTexture;
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_RadGradScreenMask:
	// update paint generators
	paintGen->Uf += paintGen->DUf;
	paintGen->Vf += paintGen->DVf;
	paintGen->A += paintGen->dA;
	paintGen->B += paintGen->dB;
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	mask = alphaPixels[j];
	tmpCov = ((cov >> AM_RAS_COVERAGE_PRECISION) * mask) >> 8;
	if (tmpCov != 0) {
		AMfloat s = paintGen->radiusSqr * (paintGen->Uf * paintGen->Uf + paintGen->Vf * paintGen->Vf);
		AMfloat u = paintGen->A + amFastSqrtf(s - (paintGen->B *paintGen->B));
		fixedU = (AMint32)(u * paintGen->K);
		AM_GRADIENTS_RADIAL_PIXEL_GET(ScaSa.value, fixedU)
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_RadGradScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_RadGradScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_RadGradScreenMask;
	else 
		covLine[j] = 0;
}

/*!
	\brief Filler: paint type VG_PAINT_TYPE_PATTERN, blend mode VG_BLEND_SCREEN, masking on ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_PatternScreenMask(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;
	AMuint8 *alphaPixels;
	AMuint32 mask;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// go to the start alpha pixel
	alphaPixels = surface->alphaMaskPixels + ofs0;
	// initialize the paint generation
	amPaintPatternInit(paintGen, x0 - 1, y);
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_PatternScreenMask:
	// update paint generators
	paintGen->Ux += paintGen->DUx;
	paintGen->Vx += paintGen->DVx;
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	mask = alphaPixels[j];
	tmpCov = ((cov >> AM_RAS_COVERAGE_PRECISION) * mask) >> 8;
	if (tmpCov != 0) {
		paintGen->patternSamplerParams.x = paintGen->Ux;
		paintGen->patternSamplerParams.y = paintGen->Vx;
		ScaSa.value = paintGen->patternSampler(&paintGen->patternSamplerParams);
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_PatternScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_PatternScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_PatternScreenMask;
	else 
		covLine[j] = 0;
}

#if defined(VG_MZT_conical_gradient)
/*!
	\brief Filler: paint type VG_PAINT_TYPE_CONICAL_GRADIENT_MZT, blend mode VG_BLEND_SCREEN, masking on ( \b generated \b function ).
	\param _surface pointer to a AMDrawingSurface structure, defining the destination drawing surface.
	\param _paintGen pointer to a AMPaintGen structure, containing paint information and derived values.
	\param y y coordinate of the scanline to fill.
	\param x0 x coordinate of the left-most pixel to fill.
	\param x1 x coordinate of the right-most pixel to fill.
	\note coverage line deltas are cleared (set to 0) during the loop, if needed.
*/
void amFilPath_ConGradScreenMask(void *_surface, void *_paintGen, AMint32 y, AMint32 x0, AMint32 x1) {

	AMDrawingSurface *surface = (AMDrawingSurface *)_surface;
	AMPaintGen *paintGen = (AMPaintGen *)_paintGen;
	AMuint32 *scrPixels;
	AMint32 *covLine;
	AMuint32 i, ofs0;
	AMuint32 j;
	AMint32 cov;
	AMPixel32 DcaDa, ScaSa;
	AMPixel32 tmp;
	AMint32 invCov;
	AMint32 tmpCov;
	AMint32 fixedU;
	AMint32 fixedV;
	const AMuint32 *gradientPixels;
	VGColorRampSpreadMode spreadMode;
	AMuint8 *alphaPixels;
	AMuint32 mask;

	AM_ASSERT(surface);
	AM_ASSERT(paintGen);
	AM_ASSERT(paintGen->coverageLineDeltas);
	AM_ASSERT(y >= 0 && y < amSrfHeightGet(surface));
	AM_ASSERT(x0 <= x1);
	AM_ASSERT(x0 >= 0 && x0 < amSrfWidthGet(surface));
	AM_ASSERT(x1 >= 0 && x1 < amSrfWidthGet(surface));
#if RIM_VG_SRC
    ScaSa.value = 0;
#endif

	ofs0 = (amSrfHeightGet(surface) - y - 1) * amSrfWidthGet(surface) + x0;
	// go to the start screen pixel
	scrPixels = amSrfPixelsGet(surface) + ofs0;
	// extract coverage line
	covLine = paintGen->coverageLineDeltas + x0;
	// go to the start alpha pixel
	alphaPixels = surface->alphaMaskPixels + ofs0;
	// initialize the paint generation
	amPaintConGradInit(paintGen, x0 - 1, y);
	spreadMode = paintGen->paintDesc->paint->colorRampSpreadMode;
	gradientPixels = paintGen->gradTexture;
	j = 0;
	i = x1 - x0 + 1;
	cov = 0;

loop_ConGradScreenMask:
	// update paint generators
	paintGen->Uf += paintGen->DUf;
	paintGen->Vf += paintGen->DVf;
	cov += covLine[j];
	if (covLine[j] != 0)
		covLine[j] = 0;
	mask = alphaPixels[j];
	tmpCov = ((cov >> AM_RAS_COVERAGE_PRECISION) * mask) >> 8;
	if (tmpCov != 0) {
		AMfloat x = paintGen->Uf;
		AMfloat y = paintGen->Vf;
		AMfloat l = x * x + y * y;
		AMuint32 atan2Val;
		AMuint32 atan2U;
		if (l != 0) {
			AMfloat xhalf = 0.5f * l;
			AMint32 f = *(AMint32 *)&l;
			f = 0x5f3759df - (f >> 1);
			l = *(AMfloat*)&f;
			l = l * (1.5f - xhalf * l * l);
			x *= l;
			y *= l;
		}
		x = x * AM_ATAN2_TABLE_SIZE_K + AM_ATAN2_TABLE_SIZE_K;
		y = y * AM_ATAN2_TABLE_SIZE_K + AM_ATAN2_TABLE_SIZE_K;
		fixedU = (AMint32)x;
		fixedV = (AMint32)y;
		AM_ASSERT(fixedU >= 0 && fixedU < AM_GRADIENTS_CONICAL_TEXTURE_WIDTH);
		AM_ASSERT(fixedV >= 0 && fixedV < AM_GRADIENTS_CONICAL_TEXTURE_WIDTH);
		AM_GRADIENTS_CONICAL_PIXEL_GET(ScaSa.value, fixedU, fixedV)
		// get the pixel on the buffer, if blendMode is different than Src
		DcaDa.value = scrPixels[j];
		// apply blend equation
		tmp.value = amPxlAdds(ScaSa.value, amPxlMul(amPxlInv(ScaSa.value), DcaDa.value));
		// apply coverage
		invCov = 256 - tmpCov;
		if (invCov == 0) {
			// write pixel
			scrPixels[j++] = tmp.value;
			if (--i != 0)
				goto loop_ConGradScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
		else {
			// write pixel
			scrPixels[j++] = amPxlLerp((AMuint32)tmpCov, DcaDa.value, tmp.value);
			if (--i != 0)
				goto loop_ConGradScreenMask;
			else {
				covLine[j] = 0;
				return;
			}
		}
	}
	j++;
	if (--i != 0)
		goto loop_ConGradScreenMask;
	else 
		covLine[j] = 0;
}

#endif
#endif

#if defined (RIM_VG_SRC)
#pragma pop
#endif
