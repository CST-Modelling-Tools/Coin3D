/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOSFBOOL_H
#define COIN_SOSFBOOL_H

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>


class COIN_DLL_EXPORT SoSFBool : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFBool, SbBool, SbBool);

public:
  static void initClass(void);
};

#endif // !COIN_SOSFBOOL_H
