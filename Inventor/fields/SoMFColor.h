#ifndef COIN_SOMFCOLOR_H
#define COIN_SOMFCOLOR_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/fields/SoMField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbColor.h>

#ifdef __PIVY__
%typemap(in) float rgb[][3] (float (*temp)[3]) {
  int len;

  if (PySequence_Check($input)) {
	len = PySequence_Length($input);

	temp = (float (*)[3]) malloc(len*3*sizeof(float));
	convert_SoMFVec3f_array($input, len, temp);
  
	$1 = temp;
  } else {
	PyErr_SetString(PyExc_TypeError, "expected a sequence.");
  }
}

%typemap(in) float hsv[3] (float temp[3]) {
  convert_SbVec3f_array($input, temp);
  $1 = temp;
}

%typemap(in) float rgb[3] (float temp[3]) {
  convert_SbVec3f_array($input, temp);
  $1 = temp;
}

%rename(setValue_col) SoMFColor::setValue(SbColor const &);
%rename(setValue_vec) SoMFColor::setValue(const SbVec3f &vec);
%rename(setValue_fff) SoMFColor::setValue(const float red, const float green, const float blue);

%feature("shadow") SoMFColor::setValue(const float rgb[3]) %{
def setValue(*args):
   if len(args) == 2:
      if isinstance(args[1], SbVec3f):
         return apply(_pivy.SoMFColor_setValue_vec,args)
      elif isinstance(args[1], SbColor):
         return apply(_pivy.SoMFColor_setValue_col,args)
      else:
         return apply(_pivy.SoMFColor_setValue,args)
   elif len(args) == 4:
      return apply(_pivy.SoMFColor_setValue_fff,args)
%}

%rename(set1Value_i_col) SoMFColor::set1Value(int const ,SbColor const &);
%rename(set1Value_i_vec) SoMFColor::set1Value(const int idx, const SbVec3f &vec);
%rename(set1Value_i_fff) SoMFColor::set1Value(const int idx, const float r, const float g, const float b);

%feature("shadow") SoMFColor::set1Value(const int idx, const float rgb[3]) %{
def set1Value(*args):
   if len(args) == 3:
      if isinstance(args[2], SbVec3f):
         return apply(_pivy.SoMFColor_set1Value_i_vec,args)
      elif isinstance(args[2], SbColor):
         return apply(_pivy.SoMFColor_set1Value_i_col,args)
      else:
         return apply(_pivy.SoMFColor_set1Value,args)
   elif len(args) == 5:
      return apply(_pivy.SoMFColor_set1Value_i_fff,args)
%}

%rename(setHSVValue_fff) SoMFColor::setHSVValue(const float h, const float s, const float v);

%feature("shadow") SoMFColor::setHSVValue(const float hsv[3]) %{
def setHSVValue(*args):
   if len(args) == 4:
      return apply(_pivy.SoMFColor_setHSVValue_fff,args)
   return apply(_pivy.SoMFColor_setHSVValue,args)
%}

%rename(set1HSVValue_i_fff) SoMFColor::set1HSVValue(const int idx, const float h, const float s, const float v);

%feature("shadow") SoMFColor::set1HSVValue(const int idx, const float hsv[3]) %{
def set1HSVValue(*args):
   if len(args) == 5:
      return apply(_pivy.SoMFColor_set1HSVValue_i_fff,args)
   return apply(_pivy.SoMFColor_set1HSVValue,args)
%}

%rename(setValues_i_i_col) SoMFColor::setValues(int const ,int const ,SbColor const *);

%feature("shadow") SoMFColor::setValues(const int start, const int num, const float rgb[][3]) %{
def setValues(*args):
   if isinstance(args[3], SbColor):
      return apply(_pivy.SoMFColor_setValues_i_i_col,args)
   return apply(_pivy.SoMFColor_setValues,args)
%}
#endif

class COIN_DLL_API SoMFColor : public SoMField {
    typedef SoMField inherited;

  SO_MFIELD_HEADER(SoMFColor, SbColor, const SbColor &);

  SO_MFIELD_SETVALUESPOINTER_HEADER(float);
  SO_MFIELD_SETVALUESPOINTER_HEADER(SbColor);

public:
  static void initClass(void);

  void setValues(const int start, const int num, const float rgb[][3]);
  void setHSVValues(const int start, const int num, const float hsv[][3]);

  void setValue(const SbVec3f & vec);
  void setValue(const float r, const float g, const float b);
  void setValue(const float rgb[3]);

  void setHSVValue(const float h, const float s, const float v);
  void setHSVValue(const float hsv[3]);

#ifdef __PIVY__
  %extend {
        void __call__(float rgb[3]) {
          self->setValue(rgb);
        }
  }
#endif

  void set1Value(const int idx, const SbVec3f & vec);
  void set1Value(const int idx, const float r, const float g, const float b);
  void set1Value(const int idx, const float rgb[3]);

  void set1HSVValue(const int idx, const float h, const float s, const float v);
  void set1HSVValue(const int idx, const float hsv[3]);
};

#endif // !COIN_SOMFCOLOR_H