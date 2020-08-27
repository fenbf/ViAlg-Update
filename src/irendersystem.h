#pragma once

#include "tg_math.h"

enum BlockType { btPoint, btBox, btPyramid, btCylinder };
enum ColorType { ctNormal, ctMarked, ctHighlighted };

class IRenderSystem {
public:
	IRenderSystem() = default;
	virtual ~IRenderSystem() = default;

	virtual void SetDiagramBlockInfo(BlockType bType, const VECTOR3D& vCol, const VECTOR3D& vColMarked,
		const VECTOR3D& vColHighlighted) = 0;
	virtual void BeginDrawing(double fMaxData, int nCount) = 0;
	virtual void DrawDiagramBlock(double fValue, ColorType cType) = 0;
	virtual void EndDrawing() = 0;
	virtual void SetMaxSize(float fWidth, float fHeight, float fDepth)  = 0;
	virtual void SetOutlook(const VECTOR3D& vFrameCol, unsigned int iFrameTex) = 0;
	virtual void SetBlockType(BlockType bType) = 0;
};

