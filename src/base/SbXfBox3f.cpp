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
  \class SbXfBox3f SbBox.h Inventor/SbBox.h
  \brief The SbXfBox3f class is a 3 dimensional box with floating point coordinates and an attached transformation.
  \ingroup base

  This box class is used by many other classes in Coin
  for data exchange. It provides storage for two box corners with
  floating point coordinates, and for a floating point 4x4 transformation
  matrix.

  \sa SbBox3f, SbBox2f, SbBox2s, SbMatrix
*/

#include <Inventor/SbXfBox3f.h>
#include <float.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

/*!
  The default constructor makes an empty box and identity matrix.
 */
SbXfBox3f::SbXfBox3f()
{
  this->matrix.makeIdentity();
  this->invertedmatrix.makeIdentity();
}

/*!
  Constructs a box with the given corners.

  The coordinates of \a min should be less than the coordinates of
  \a max if you want to make a valid box.
 */
SbXfBox3f::SbXfBox3f(const SbVec3f &_min, const SbVec3f &_max):
  SbBox3f(_min,_max)
{
  this->matrix.makeIdentity();
  this->invertedmatrix.makeIdentity();
}

/*!
  Constructs a box from the given SbBox3f.

  The transformation is set to the identity matrix.
 */
SbXfBox3f::SbXfBox3f(const SbBox3f &box):
  SbBox3f(box)
{
  this->matrix.makeIdentity();
  this->invertedmatrix.makeIdentity();
}

/*!
  Default destructor does nothing.
 */
SbXfBox3f::~SbXfBox3f()
{
}

/*!
  Overloaded from SbBox3f, as the transformations are to be kept separate
  from the box in the SbXfBox3f class.
 */
void
SbXfBox3f::transform(const SbMatrix & m)
{
  this->setTransform(this->matrix.multRight(m));
}

/*!
  Sets the transformation to the given SbMatrix.
*/
void
SbXfBox3f::setTransform(const SbMatrix &m)
{
#if COIN_DEBUG && 0 // debug
  if (this->matrix.det4() == 0.0f) {
    SoDebugError::postWarning("SbXfBox3f::setTransform",
                              "invalid matrix (can't be inverted)");
    SoDebugError::postWarning("SbXfBox3f::setTransform",
                              "%f %f %f %f",
                              m[0][0], m[0][1], m[0][2], m[0][3]);
    SoDebugError::postWarning("SbXfBox3f::setTransform",
                              "%f %f %f %f",
                              m[1][0], m[1][1], m[1][2], m[1][3]);
    SoDebugError::postWarning("SbXfBox3f::setTransform",
                              "%f %f %f %f",
                              m[2][0], m[2][1], m[2][2], m[2][3]);
    SoDebugError::postWarning("SbXfBox3f::setTransform",
                              "%f %f %f %f",
                              m[3][0], m[3][1], m[3][2], m[3][3]);
  }
#endif // debug

  this->matrix = m;
  this->makeInvInvalid(); //Invalidate current inverse
}

/*!
  Returns the current transformation matrix.
*/
const SbMatrix &
SbXfBox3f::getTransform() const
{
  return this->matrix;
}

/*!
  Returns the inverse of the current transformation matrix.
*/
const SbMatrix &
SbXfBox3f::getInverse() const
{
  this->calcInverse();
  return this->invertedmatrix;
}

/*!
  Return the transformed center point of the box.
 */
SbVec3f
SbXfBox3f::getCenter() const
{
  SbVec3f orgcenter = SbBox3f::getCenter();
  SbVec3f transcenter;
  this->matrix.multVecMatrix(orgcenter,transcenter);
  return transcenter;
}

/*!
  Extend the boundaries of the box by the given point, i.e. make the
  point fit inside the box if it isn't already so.

  The point is assumed to be in transformed space.
*/
void
SbXfBox3f::extendBy(const SbVec3f &pt)
{
  SbMatrix im = this->getInverse();
#if 0 // debug
  SoDebugError::postInfo("SbXfBox3f::extendBy",
                         "\n\t%.3f %.3f %.3f %.3f\n"
                         "\t%.3f %.3f %.3f %.3f\n"
                         "\t%.3f %.3f %.3f %.3f\n"
                         "\t%.3f %.3f %.3f %.3f",
                         im[0][0], im[0][1], im[0][2], im[0][3],
                         im[1][0], im[1][1], im[1][2], im[1][3],
                         im[2][0], im[2][1], im[2][2], im[2][3],
                         im[3][0], im[3][1], im[3][2], im[3][3]);
#endif // debug
  SbVec3f trans;
  im.multVecMatrix(pt, trans);
#if 0 // debug
  SoDebugError::postInfo("SbXfBox3f::extendBy", "trans pt: <%f, %f, %f>",
                         trans[0], trans[1], trans[2]);
#endif // debug
  SbBox3f::extendBy(trans);
}

/*!
  Extend the boundaries of the box by the given \a bb parameter.
  The given box is assumed to be in transformed space.

  The two given boxes will be combined in such a way so that the resultant
  bounding box always has the smallest possible volume. To accomplish this,
  the transformation on this SbXfBox3f will sometimes be flattened before
  it's combined with \a bb.
*/
void
SbXfBox3f::extendBy(const SbBox3f &bb)
{
#if COIN_DEBUG
  if (bb.isEmpty()) {
    SoDebugError::postWarning("SbXfBox3f::extendBy",
                              "Extending box is empty.");
    return;
  }
#endif // COIN_DEBUG

  if (this->isEmpty()) {
    *this = bb;
    this->matrix.makeIdentity();
    this->invertedmatrix.makeIdentity();
    return;
  }

  SbVec3f points[2] = { bb.getMin(), bb.getMax() };

  // Combine bboxes while keeping the transformation matrix.
  SbBox3f box1 = *this;
  {
    SbMatrix im = this->getInverse();
    // Transform all the corners and include them into the new box.
    for (int i=0; i < 8; i++) {
      SbVec3f corner, dst;
      // Find all corners the "binary" way :-)
      corner.setValue(points[(i&4)>>2][0],
                      points[(i&2)>>1][1],
                      points[i&1][2]);
      // Don't try to optimize the transformation out of the loop,
      // it's not as easy as it seems.
      im.multVecMatrix(corner, dst);
#if 0 // debug
      SoDebugError::postInfo("SbXfBox3f::extendBy",
                             "point: <%f, %f, %f> -> <%f, %f, %f>",
                             corner[0], corner[1], corner[2],
                             dst[0], dst[1], dst[2]);
#endif // debug
      box1.extendBy(dst);
    }
  }


  // Combine bboxes with a flattened transformation matrix.
  SbBox3f box2 = this->project();
  {
    for (int j=0;j<8;j++) {
      SbVec3f corner;
      corner.setValue(points[(j&4)>>2][0],
                      points[(j&2)>>1][1],
                      points[j&1][2]);
      box2.extendBy(corner);
    }
  }

  SbXfBox3f xfbox(box1);
  xfbox.setTransform(this->matrix);
#if 0 // debug
  SoDebugError::postInfo("SbXfBox3f::extendBy",
                         "kintel-volume: %f, mortene-volume: %f",
                         xfbox.getVolume(), box2.getVolume());
#endif // debug

  // Choose result from one of the two techniques based on the volume
  // of the resultant bbox.
  if (xfbox.getVolume() < box2.getVolume()) {
    this->setBounds(box1.getMin(), box1.getMax());
  }
  else {
    this->setBounds(box2.getMin(), box2.getMax());
    this->matrix.makeIdentity();
    this->invertedmatrix.makeIdentity();
  }
}

/*!
  Extend the boundaries of the box by the given \a bb parameter.

  The given box is assumed to be in transformed space.
*/
void
SbXfBox3f::extendBy(const SbXfBox3f & bb)
{
#if COIN_DEBUG
  if (bb.isEmpty()) {
    SoDebugError::postWarning("SbXfBox3f::extendBy",
                              "Extending box is empty.");
    return;
  }
#endif // COIN_DEBUG

  if (this->isEmpty()) {
    *this = bb;
    return;
  }

#if 0 // debug
  SoDebugError::postInfo("SbXfBox3f::extendBy",
                         "bb: <%f, %f, %f>, <%f, %f, %f>",
                         bb.getMin()[0],
                         bb.getMin()[1],
                         bb.getMin()[2],
                         bb.getMax()[0],
                         bb.getMax()[1],
                         bb.getMax()[2]);
#endif // debug

  // Try extending while keeping the transform on "this" first.
  SbXfBox3f box1 = *this;
  {
    SbVec3f points[2] = { bb.getMin(), bb.getMax() };
    {
      SbMatrix m = bb.getTransform();
      m.multRight(box1.getInverse());

      for (int i=0; i < 8; i++) {
        SbVec3f corner, dst;
        corner.setValue(points[(i&4)>>2][0],
                        points[(i&2)>>1][1],
                        points[i&1][2]);
        m.multVecMatrix(corner, dst);
#if 0 // debug
        SoDebugError::postInfo("SbXfBox3f::extendBy",
                               "corner: <%f, %f, %f>, dst <%f, %f, %f>",
                               corner[0], corner[1], corner[2],
                               dst[0], dst[1], dst[2]);
#endif // debug
        ((SbBox3f *)&box1)->extendBy(dst);
#if 0 // debug
        SoDebugError::postInfo("SbXfBox3f::extendBy",
                               "dst: <%f, %f, %f>  ->   "
                               "box1: <%f, %f, %f>, <%f, %f, %f>",
                               dst[0], dst[1], dst[2],
                               box1.getMin()[0],
                               box1.getMin()[1],
                               box1.getMin()[2],
                               box1.getMax()[0],
                               box1.getMax()[1],
                               box1.getMax()[2]);
#endif // debug
      }
    }
  }

  // Try extending while keeping the transform on bb.
  SbXfBox3f box2 = bb;
  {
    SbVec3f points[2] = { this->getMin(), this->getMax() };
    {
      SbMatrix m = this->getTransform();
      m.multRight(box2.getInverse());

      for (int i=0; i < 8; i++) {
        SbVec3f corner, dst;
        corner.setValue(points[(i&4)>>2][0],
                        points[(i&2)>>1][1],
                        points[i&1][2]);
        m.multVecMatrix(corner, dst);
#if 0 // debug
        SoDebugError::postInfo("SbXfBox3f::extendBy",
                               "corner: <%f, %f, %f>, dst <%f, %f, %f>",
                               corner[0], corner[1], corner[2],
                               dst[0], dst[1], dst[2]);
#endif // debug
        ((SbBox3f *)&box2)->extendBy(dst);
#if 0 // debug
        SoDebugError::postInfo("SbXfBox3f::extendBy",
                               "dst: <%f, %f, %f>  ->   "
                               "box2: <%f, %f, %f>, <%f, %f, %f>",
                               dst[0], dst[1], dst[2],
                               box2.getMin()[0],
                               box2.getMin()[1],
                               box2.getMin()[2],
                               box2.getMax()[0],
                               box2.getMax()[1],
                               box2.getMax()[2]);
#endif // debug
      }
    }
  }

#if 0 // debug
  SoDebugError::postInfo("SbXfBox3f::extendBy",
                         "box1-volume: %f, box2-volume: %f",
                         box1.getVolume(), box2.getVolume());
#endif // debug

  // Compare volumes and pick the smallest bounding box.
  if (box1.getVolume() < box2.getVolume()) *this = box1;
  else *this = box2;
}

/*!
  Check if the given point lies within the boundaries of this box.

  The point is assumed to be in transformed space.
*/
SbBool
SbXfBox3f::intersect(const SbVec3f &pt) const
{
  this->calcInverse();
  SbVec3f trans;
  this->invertedmatrix.multVecMatrix(pt, trans);
  return SbBox3f::intersect(trans);
}

//
// tests for intersection between an axis aligned box and the
// 12 edges defined by the 8 points in the 'points' array.
//
static
SbBool intersect_box_edges(const SbVec3f &min,
                           const SbVec3f &max,
                           const SbVec3f * const points)
{
  // lookup table for edges in the cube.
  static int lines[12*2] =
  {
    0,1,
    0,2,
    0,4,
    1,3,
    1,5,
    2,3,
    2,6,
    3,7,
    4,5,
    4,6,
    5,7,
    6,7
  };

  // need this for the innermost loop
  SbVec3f boxpts[2];
  boxpts[0] = min;
  boxpts[1] = max;

  // test for edge intersection
  for (int i = 0; i < 12; i++) { // 12 edges in a cube
    SbVec3f l1 = points[lines[i*2]];
    SbVec3f l2 = points[lines[i*2+1]];
    // possible optimization: reuse directional vectors
    SbVec3f dir = l2 - l1;
    dir.normalize();
    SbVec3f lmin(SbMin(l1[0], l2[0]),
                 SbMin(l1[1], l2[1]),
                 SbMin(l1[2], l2[2]));
    SbVec3f lmax(SbMax(l1[0], l2[0]),
                 SbMax(l1[1], l2[1]),
                 SbMax(l1[2], l2[2]));

    // the bbox to test against is axis-aligned, and this makes it
    // quite simple.
    for (int j = 0; j < 3; j++) { // test planes in all three dimensions
      for (int k = 0; k < 2; k++) { // test both min and max planes
        // check if line crosses current plane
        if (dir[j] != 0.0f &&
            lmin[j] <= boxpts[k][j] && lmax[j] >= boxpts[k][j]) {
          // find the two other coordinates
          int t1 = j+1;
          int t2 = j+2;
          // do this instead of modulo 3
          if (t1 >= 3) t1 -= 3;
          if (t2 >= 3) t2 -= 3;

          // find what we need to multiply coordinate j by to
          // put it onto the current plane
          float delta = (float)fabs((boxpts[k][j] - l1[j]) / dir[j]);
          // calculate the two other coordinates
          float v1 = l1[t1] + delta*dir[t1];
          float v2 = l1[t2] + delta*dir[t2];
          if (v1 > boxpts[0][t1] && v1 < boxpts[1][t1] &&
              v2 > boxpts[0][t2] && v2 < boxpts[1][t2]) {
            return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

//
// weak box-box intersection test: min, max defines an axis-aligned
// box, while boxmin, boxmax defines an box that should be transformed
// by matrix. This function only tests whether any of the 8
// (transformed) points in (boxmin, boxmax) is inside (min, max),
// and if any of the 12 edges in (boxmin, boxmax) intersects any of the
// planes in the box defined by (min, max).
//
// Use this function twice to cover all intersection cases.
//
static SbBool
intersect_box_box(const SbVec3f &min,
                  const SbVec3f &max,
                  const SbVec3f &boxmin,
                  const SbVec3f &boxmax,
                  const SbMatrix &matrix,
                  SbBool &alignedIntersect)
{
  SbVec3f transpoints[8];
  SbBox3f alignedBox;
  for (int i = 0;  i < 8; i++) {
    SbVec3f tmp, tmp2;
    tmp.setValue((i&4) ? boxmin[0] : boxmax[0],
                 (i&2) ? boxmin[1] : boxmax[1],
                 (i&1) ? boxmin[2] : boxmax[2]);
    matrix.multVecMatrix(tmp, tmp2);
    // is point inside
    if (tmp2[0] >= min[0] &&
        tmp2[0] <= max[0] &&
        tmp2[1] >= min[1] &&
        tmp2[1] <= max[1] &&
        tmp2[2] >= min[2] &&
        tmp2[2] <= max[2]) {
      return TRUE;
    }
    alignedBox.extendBy(tmp2);
    transpoints[i] = tmp2;
  }
  // this is just an optimization:
  // if the axis aligned box doesn't intersect the box, there
  // is no chance for any intersection.
  SbBox3f thisbox(min, max);
  alignedIntersect = thisbox.intersect(alignedBox);

  // only test edge intersection if aligned boxes intersect
  if (alignedIntersect)
    return intersect_box_edges(min, max, transpoints);
  return FALSE;
}

/*!
  Check if the given \a box lies wholly or partly within the boundaries
  of this box.

  The given box is assumed to be in transformed space.
*/
SbBool
SbXfBox3f::intersect(const SbBox3f &bb) const
{
  if (this->matrix == SbMatrix::identity()) return SbBox3f::intersect(bb);

  //
  // do double-test to get all intersection cases
  //
  SbBool alignedIntersect;

  if (intersect_box_box(bb.getMin(), bb.getMax(),
                        this->getMin(), this->getMax(),
                        this->matrix, alignedIntersect)) return TRUE;

  if (!alignedIntersect) return FALSE;

  // will need the inverse matrix here
  this->calcInverse();
  return intersect_box_box(this->getMin(), this->getMax(),
                           bb.getMin(), bb.getMax(),
                           this->invertedmatrix,
                           alignedIntersect);
}

/*!
  Find the span of the box in the given direction (i.e. how much room in
  the given direction the box needs). The distance is returned as the minimum
  and maximum distance from Origo to the closest and furthest plane defined
  by the direction vector and each of the box' corners. The difference
  between these values gives the span.
*/
void
SbXfBox3f::getSpan(const SbVec3f &direction, float &dMin, float &dMax) const
{
  this->project().getSpan(direction, dMin, dMax);
}

/*!
  Project the SbXfBox3f into a SbBox3f.

  This gives the same resulting SbBox3f as doing a SbBox3f::transform()
  with this transformation matrix as parameter.
*/
SbBox3f
SbXfBox3f::project(void) const
{
  SbBox3f box(this->getMin(), this->getMax());
  if (!box.isEmpty()) box.transform(this->matrix);
  return box;
}

/*!
  Check if \a b1 and \a b2 are equal. Return 1 if they are equal,
  or 0 if they are unequal. Note that the method will do a dumb
  component by component comparison.
*/
int
operator ==(const SbXfBox3f &b1, const SbXfBox3f &b2)
{
  return
    (b1.getMin() == b2.getMin()) &&
    (b1.getMax() == b2.getMax()) &&
    (b1.matrix == b2.matrix);
}

/*!
  Check if \a b1 and \a b2 are unequal. Return 0 if they are equal,
  or 1 if they are unequal. See the note on operator==().
 */
int
operator !=(const SbXfBox3f &b1, const SbXfBox3f &b2)
{
  return !(b1 == b2);
}

/*!
  Return box volume. Overloaded from parent class to take into account
  the possibility of scaling in the transformation matrix.
 */
float
SbXfBox3f::getVolume(void) const
{
  if (!this->hasVolume()) return 0.0f;

  float scalex = (float)sqrt(this->matrix[0][0] * this->matrix[0][0] +
                             this->matrix[1][0] * this->matrix[1][0] +
                             this->matrix[2][0] * this->matrix[2][0]);
  float scaley = (float)sqrt(this->matrix[0][1] * this->matrix[0][1] +
                             this->matrix[1][1] * this->matrix[1][1] +
                             this->matrix[2][1] * this->matrix[2][1]);
  float scalez = (float)sqrt(this->matrix[0][2] * this->matrix[0][2] +
                             this->matrix[1][2] * this->matrix[1][2] +
                             this->matrix[2][2] * this->matrix[2][2]);

  SbVec3f min, max;
  this->getBounds(min, max);

  return (float)fabs(((max[0]-min[0]) * scalex *
                      (max[1]-min[1]) * scaley *
                      (max[2]-min[2]) * scalez));
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized compile.
 */
void
SbXfBox3f::print(FILE * fp) const
{
#if COIN_DEBUG
  SbVec3f minv, maxv;
  this->getBounds(minv, maxv);
  fprintf( fp, "  bounds " );
  minv.print(fp);
  fprintf( fp, " " );
  maxv.print(fp);
  fprintf( fp, "\n" );

  fprintf( fp, "  center " );
  this->getCenter().print(fp);
  fprintf( fp, "\n" );
  float x, y, z;
  this->getOrigin(x, y, z);
  fprintf( fp, "  origin " );
  SbVec3f(x, y, z).print(fp);
  fprintf( fp, "\n" );
  this->getSize(x, y, z);
  fprintf( fp, "  size " );
  SbVec3f(x, y, z).print(fp);

  fprintf( fp, "\n" );
  fprintf( fp, "  volume %f\n", this->getVolume() );
  this->getTransform().print(fp);

  fprintf( fp, "  project " );
  this->project().print(fp);
  fprintf( fp, "\n" );
#endif // COIN_DEBUG
}

void
SbXfBox3f::calcInverse(void) const
{
  if (this->invertedmatrix[0][0] == FLT_MAX)
    ((SbXfBox3f *)this)->invertedmatrix = this->matrix.inverse();
}

void
SbXfBox3f::makeInvInvalid(void)
{
  this->invertedmatrix[0][0] = FLT_MAX;
}
