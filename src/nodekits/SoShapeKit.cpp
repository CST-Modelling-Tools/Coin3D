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
  \class SoShapeKit SoShapeKit.h Inventor/nodekits/SoShapeKit.h
  \brief The SoShapeKit class ...
  \ingroup nodekits

  FIXME: write class doc
*/

#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/nodes/SoTextureCoordinateBinding.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCoordinate4.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoTextureCoordinate2.h>
#include <Inventor/nodes/SoProfileCoordinate2.h>
#include <Inventor/nodes/SoProfileCoordinate3.h>
#include <Inventor/nodes/SoTextureCoordinateDefault.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoProfile.h>
#include <coindefs.h> // COIN_STUB


SO_KIT_SOURCE(SoShapeKit);


/*!
  Constructor.
*/
SoShapeKit::SoShapeKit(void)
{
  SO_KIT_INTERNAL_CONSTRUCTOR(SoShapeKit);

  // Note: we must use "" instead of , , to humour MS VisualC++ 6.

  SO_KIT_ADD_CATALOG_ENTRY(materialBinding, SoMaterialBinding, TRUE, topSeparator, normalBinding, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(normalBinding, SoNormalBinding, TRUE, topSeparator, textureCoordinateBinding, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(textureCoordinateBinding, SoTextureCoordinateBinding, TRUE, topSeparator, shapeHints, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(shapeHints, SoShapeHints, TRUE, topSeparator, coordinate3, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(coordinate3, SoCoordinate3, TRUE, topSeparator, coordinate4, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(coordinate4, SoCoordinate4, TRUE, topSeparator, normal, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(normal, SoNormal, TRUE, topSeparator, textureCoordinate2, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(textureCoordinate2, SoTextureCoordinate2, TRUE, topSeparator, profileCoordinate2, TRUE);
  SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(textureCoordinateFunction, SoTextureCoordinateFunction, SoTextureCoordinateDefault, TRUE, topSeparator, localTransform, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(profileCoordinate2, SoProfileCoordinate2, TRUE, topSeparator, profileCoordinate3, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(profileCoordinate3, SoProfileCoordinate3, TRUE, topSeparator, profileList, TRUE);
  SO_KIT_ADD_CATALOG_LIST_ENTRY(profileList, SoGroup, TRUE, topSeparator, childList, SoProfile, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(localTransform, SoTransform, TRUE, topSeparator, shapeSeparator, TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(shapeSeparator, SoSeparator, TRUE, topSeparator, "", FALSE);
  SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(shape, SoShape, SoCube, FALSE, shapeSeparator, "", TRUE);

  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoShapeKit::~SoShapeKit()
{
}

/*!
  Does initialization common for all objects of the
  SoShapeKit class. This includes setting up the
  type system, among other things.
*/
void
SoShapeKit::initClass(void)
{
  SO_KIT_INTERNAL_INIT_CLASS(SoShapeKit);
}

/*!
  FIXME: write function documentation
*/
void
SoShapeKit::setDefaultOnNonWritingFields(void)
{
  COIN_STUB();
}
