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

/*!
  \class SoComposeVec4f SoComposeVec4f.h Inventor/engines/SoComposeVec4f.h
  \brief The SoComposeVec4f class is used to compose 4D coordinates from four floats.
  \ingroup engines

  FIXME: doc
*/

#include <Inventor/engines/SoComposeVec4f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoComposeVec4f);

/*!
  Default constructor.
*/
SoComposeVec4f::SoComposeVec4f()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoComposeVec4f);

  SO_ENGINE_ADD_INPUT(x,(0.0f));
  SO_ENGINE_ADD_INPUT(y,(0.0f));
  SO_ENGINE_ADD_INPUT(z,(0.0f));
  SO_ENGINE_ADD_INPUT(w,(0.0f));

  SO_ENGINE_ADD_OUTPUT(vector,SoMFVec4f);
}

// overloaded from parent
void
SoComposeVec4f::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoComposeVec4f);
}

//
// private members
//
SoComposeVec4f::~SoComposeVec4f()
{
}

// overloaded from parent
void
SoComposeVec4f::evaluate()
{
  int numX=x.getNum();
  int numY=y.getNum();
  int numZ=z.getNum();
  int numW=w.getNum();

  int numOut=numX>numY?numX:numY;
  numOut=numZ>numOut?numZ:numOut;
  numOut=numW>numOut?numW:numOut;

  SO_ENGINE_OUTPUT(vector,SoMFVec4f,setNum(numOut));

  int i;
  float xVal,yVal,zVal,wVal;
  for (i=0;i<numOut;i++) {
    xVal=i<numX?x[i]:x[numX-1];
    yVal=i<numY?y[i]:y[numY-1];
    zVal=i<numZ?z[i]:z[numZ-1];
    wVal=i<numW?w[i]:w[numZ-1];

    SO_ENGINE_OUTPUT(vector,SoMFVec4f,set1Value(i,xVal,yVal,zVal,wVal));
  }
}
