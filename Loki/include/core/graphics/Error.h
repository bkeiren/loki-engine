#pragma once

#ifndef ERROR_H
#define ERROR_H

namespace loki
{

namespace graphics
{

#ifdef _DEBUG
#define CheckGL()		loki::graphics::CheckGLErrors(__LINE__, __FILE__)
#else
#define CheckGL()
#endif

void CheckGLErrors( int _Line, const char* _File );

}

}

#endif