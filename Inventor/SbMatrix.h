/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SBMATRIX_H
#define COIN_SBMATRIX_H

#include <stdio.h>
#include <Inventor/SbBasic.h>

class SbLine;
class SbRotation;
class SbVec3f;
class SbVec4f;

typedef float SbMat[4][4];

#ifdef __PIVY__
%{
static SbMat *
convert_SbMat_array(PyObject *input, SbMat *temp)
{
  if (PySequence_Check(input)) {
	if (!PyArg_ParseTuple(input, "(ffff)(ffff)(ffff)(ffff)",
						  &(*temp)[0][0], &(*temp)[0][1], &(*temp)[0][2], &(*temp)[0][3],
						  &(*temp)[1][0], &(*temp)[1][1], &(*temp)[1][2], &(*temp)[1][3],
						  &(*temp)[2][0], &(*temp)[2][1], &(*temp)[2][2], &(*temp)[2][3],
						  &(*temp)[3][0], &(*temp)[3][1], &(*temp)[3][2], &(*temp)[3][3])) {
	  PyErr_SetString(PyExc_TypeError, "sequence must contain 4 sequences where every sequence contains 4 float elements");
	  return NULL;
	}
	return temp;
  } else {
	PyErr_SetString(PyExc_TypeError, "expected a sequence.");
    return NULL;
  }  
}
%}

%typemap(in) SbMat & (SbMat temp) {
  $1 = convert_SbMat_array($input, &temp);
}

%typemap(out) SbMat & {
  int i,j;
  $result = PyList_New(4);
  
  for (i=0; i<4; i++) {
	PyObject *oi = PyList_New(4);
	for (j=0; j<4; j++) {
	  PyObject *oj = PyFloat_FromDouble((double) (*$1)[i][j]);
	  PyList_SetItem(oi, j, oj);
	}
	PyList_SetItem($result, i, oi);	
  }
}
#endif

class COIN_DLL_API SbMatrix {
public:

#ifndef __PIVY__
  SbMatrix(void);
  SbMatrix(const float a11, const float a12, const float a13, const float a14,
           const float a21, const float a22, const float a23, const float a24,
           const float a31, const float a32, const float a33, const float a34,
           const float a41, const float a42, const float a43, const float a44);
  SbMatrix(const SbMat & matrix);
  SbMatrix(const SbMat * matrix);
#endif

  %addmethods {
	const SbMatrix *__init__(PyObject *args) {
	  SbMat matrix;

	  switch (PyObject_Length(args)) {
	  case 0:
		return new SbMatrix();
		break;

	  case 1:
		if (PySequence_Check(args)) {
		  if (!PyArg_ParseTuple(args, "(ffff)(ffff)(ffff)(ffff)",
								&matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3],
								&matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3],
								&matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3],
								&matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3])) {
			PyErr_SetString(PyExc_TypeError, "sequence must contain 4 sequences where every sequence contains 4 float elements");
			return NULL;
		  }
		  return new SbMatrix(&matrix);
		} else {
		  PyErr_SetString(PyExc_TypeError, "expected a sequence.");
		  return NULL;
		}  		
		break;

	  case 16:
		if (!PyArg_ParseTuple(args, "ffffffffffffffff",
							  &matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3],
							  &matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3],
							  &matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3],
							  &matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3])) {
		  PyErr_SetString(PyExc_TypeError, "arguments must contain 16 float elements");
		  return NULL;
		}
		return new SbMatrix(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
							matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
							matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
							matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);		
		break;
	  default:
		PyErr_SetString(PyExc_TypeError, "wrong number or types of arguments given");
		break;
	  }
	}
  }

  ~SbMatrix(void);

#ifndef __PIVY__
  SbMatrix & operator =(const SbMat & m);
  SbMatrix & operator =(const SbMatrix & m);
  SbMatrix & operator =(const SbRotation & q);
  operator float*(void);
#endif

  void setValue(const SbMat & m);
  const SbMat & getValue(void) const;

  void makeIdentity(void);
  void setRotate(const SbRotation & q);
  SbMatrix inverse(void) const;
  float det3(int r1, int r2, int r3,
             int c1, int c2, int c3) const;
  float det3(void) const;
  float det4(void) const;

  SbBool equals(const SbMatrix & m, float tolerance) const;

#ifndef __PIVY__
  operator SbMat&(void);
#endif

#ifdef __PIVY__
  // add a method for wrapping c++ operator[] access
  %addmethods {
	const float *__getitem__(int i) {
	  return (self->getValue())[i];
	}
  }
#else
  float * operator [](int i);
  const float * operator [](int i) const;
#endif

  SbMatrix & operator *=(const SbMatrix & m);
  friend COIN_DLL_API SbMatrix operator *(const SbMatrix & m1, const SbMatrix & m2);
  friend COIN_DLL_API int operator ==(const SbMatrix & m1, const SbMatrix & m2);
  friend COIN_DLL_API int operator !=(const SbMatrix & m1, const SbMatrix & m2);

#ifndef __PIVY__
  void getValue(SbMat & m) const;
#endif

  static SbMatrix identity(void);
  void setScale(const float s);
  void setScale(const SbVec3f & s);
  void setTranslate(const SbVec3f & t);
  void setTransform(const SbVec3f & t, const SbRotation & r, const SbVec3f & s);
  void setTransform(const SbVec3f & t, const SbRotation & r, const SbVec3f & s,
                    const SbRotation & so);
  void setTransform(const SbVec3f & translation,
                    const SbRotation & rotation, const SbVec3f & scaleFactor,
                    const SbRotation & scaleOrientation, const SbVec3f & center);
  void getTransform(SbVec3f & t, SbRotation & r,
                    SbVec3f & s, SbRotation & so) const;
  void getTransform(SbVec3f & translation, SbRotation & rotation,
                    SbVec3f & scaleFactor, SbRotation & scaleOrientation,
                    const SbVec3f & center) const;
  SbBool factor(SbMatrix & r, SbVec3f & s, SbMatrix & u, SbVec3f & t,
                SbMatrix & proj);
  SbBool LUDecomposition(int index[4], float & d);
  void LUBackSubstitution(int index[4], float b[4]) const;
  SbMatrix transpose(void) const;
  SbMatrix & multRight(const SbMatrix & m);
  SbMatrix & multLeft(const SbMatrix & m);
  void multMatrixVec(const SbVec3f & src, SbVec3f & dst) const;
  void multVecMatrix(const SbVec3f & src, SbVec3f & dst) const;
  void multDirMatrix(const SbVec3f & src, SbVec3f & dst) const;
  void multLineMatrix(const SbLine & src, SbLine & dst) const;
  void multVecMatrix(const SbVec4f & src, SbVec4f & dst) const;

  void print(FILE * fp) const;

private:
  float matrix[4][4];

  void operator /=(const float v);
  void operator *=(const float v);
};

#ifndef __PIVY__
COIN_DLL_API SbMatrix operator *(const SbMatrix & m1, const SbMatrix & m2);
COIN_DLL_API int operator ==(const SbMatrix & m1, const SbMatrix & m2);
COIN_DLL_API int operator !=(const SbMatrix & m1, const SbMatrix & m2);
#endif

#endif // !COIN_SBMATRIX_H