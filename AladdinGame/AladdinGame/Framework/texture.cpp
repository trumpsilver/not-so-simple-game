﻿#include "Texture.h"
LINK_FRAMEWORK

Texture::Texture(void)
{
	_color = C_WHITE;
	_texture = nullptr;		// nullptr have the same meaning NULL.
}

Texture::~Texture(void)
{
	//_texture->Release();
	// do no thing. use release instead.
}

HRESULT Texture::loadFromFile(LPD3DXSPRITE spriteHandle, LPWSTR filePath, D3DXCOLOR color, D3DXCOLOR colorkey)
{
	HRESULT			result;

	result = D3DXGetImageInfoFromFile(filePath, &this->_imageInfo);
	if (result != D3D_OK)
	{
		return result;
	}

	LPDIRECT3DDEVICE9 _device = DXDevice::getInstance()->getDevice();
	spriteHandle->GetDevice(&_device);

	result = D3DXCreateTextureFromFileEx(
		_device,
		filePath,
		this->_imageInfo.Width,
		this->_imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		colorkey,
		&this->_imageInfo,
		nullptr,
		&this->_texture);

	_color = color;

	return result;
}
void Texture::release()
{
	this->_texture->Release();
}
void Texture::render(LPD3DXSPRITE spriteHandle, const RECT* rect, const GVector3* center, const GVector3* position)
{
	spriteHandle->Draw(
		this->_texture,
		rect,
		center,
		position,
		_color);
}

void Texture::render(LPD3DXSPRITE spriteHandle, RECT * srcRect, GVector2 position, GVector2 scale, float rotate, GVector2 origin, float zIndex,GVector2 translation)
{
	D3DXMATRIX matFinal;
	D3DXMATRIX matTransformed;
	D3DXMATRIX matTranslate;
	D3DXMATRIX matOld;

	// origin in position
	GVector3 center = GVector3(abs(srcRect->right - srcRect->left) * origin.x, abs(srcRect->top - srcRect->bottom) * (1 - origin.y), zIndex);

	// get matrix texture
	spriteHandle->GetTransform(&matOld);

	D3DXMatrixTransformation2D(
		&matTransformed,						// ma tran ket qua sau transform
		&position,								// goc toa do / diem neo
		0.0f,
		&scale,									// ti le scale
		&position,								// goc toa do / diem neo
		D3DXToRadian(rotate),					// góc xoay theo radian
		0										// vi trí
	);

	D3DXMatrixTranslation(
		&matTranslate,
		translation.x,
		translation.y,
		0.0f
	);

	matFinal = matOld * matTransformed * matTranslate;

	//set matrix transformed
	spriteHandle->SetTransform(&matFinal);

	// BEGIN
	spriteHandle->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DONOTSAVESTATE);

	spriteHandle->Draw(
		this->_texture,
		srcRect,
		&center,
		&GVector3(position.x, position.y, zIndex),
		_color);

	spriteHandle->SetTransform(&matOld);

	// END
	spriteHandle->End();
}

void Texture::render(LPD3DXSPRITE spriteHandle, RECT * srcRect, Viewport viewport, GVector2 position, GVector2 scale, float rotate, GVector2 origin, float zIndex,GVector2 translation)
{
	GVector3 positionViewport;
	positionViewport = viewport.getPositionInViewport(&GVector3(position.x, position.y, zIndex));

	render(spriteHandle, srcRect, GVector2((int)positionViewport.x, (int)positionViewport.y), scale, rotate, origin, positionViewport.z, translation);
}

void Texture::setColor(D3DXCOLOR color)
{
	this->_color = color;
}

D3DXCOLOR Texture::getColor()
{
	return _color;
}

int Texture::getWidth()
{
	return _imageInfo.Width;
}

int Texture::getHeight()
{
	return _imageInfo.Height;
}