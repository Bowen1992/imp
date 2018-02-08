/**
 *  \file IMP/algebra/Rotation3D.h   \brief Simple 3D rotation class.
 *
 *  Copyright 2007-2018 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPALGEBRA_ROTATION_3D_H
#define IMPALGEBRA_ROTATION_3D_H

#include <IMP/algebra/algebra_config.h>
#include "Vector3D.h"
#include "utility.h"
#include "constants.h"
#include "GeometricPrimitiveD.h"
#include <Eigen/Dense>

#include <IMP/log.h>
#include <cmath>
#include <iostream>
#include <algorithm>

IMPALGEBRA_BEGIN_NAMESPACE

#if !defined(IMP_DOXYGEN) && !defined(SWIG)
class Rotation3D;
Rotation3D compose(const Rotation3D &a, const Rotation3D &b);
#endif

//! 3D rotation class.
/** Rotations are currently represented using quaternions and a cached
    copy of the rotation matrix. The quaternion allows for fast and
    stable composition and the cached rotation matrix means that
    rotations are performed quickly. See
    http://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation for
    a comparison of different implementations of rotations.

    Currently the rotation can be initialized from either:
    - XYZ Euler angles
    - Rotation Matrix
    - Quaternion
    - angle/axis representation

    \geometry
*/
class IMPALGEBRAEXPORT Rotation3D : public GeometricPrimitiveD<3> {
  VectorD<4> v_;
  mutable bool has_cache_;
  mutable Vector3D matrix_[3];
  IMP_NO_SWIG(friend Rotation3D compose(const Rotation3D &a,
                                        const Rotation3D &b));
  void fill_cache() const {
    IMP_USAGE_CHECK(get_is_valid(),
                    "Attempting to apply uninitialized rotation");
    has_cache_ = true;
    double v0s = get_squared(v_[0]);
    double v1s = get_squared(v_[1]);
    double v2s = get_squared(v_[2]);
    double v3s = get_squared(v_[3]);
    double v12 = v_[1] * v_[2];
    double v01 = v_[0] * v_[1];
    double v02 = v_[0] * v_[2];
    double v23 = v_[2] * v_[3];
    double v03 = v_[0] * v_[3];
    double v13 = v_[1] * v_[3];
    matrix_[0] =
        Vector3D(v0s + v1s - v2s - v3s, 2 * (v12 - v03), 2 * (v13 + v02));
    matrix_[1] =
        Vector3D(2 * (v12 + v03), v0s - v1s + v2s - v3s, 2 * (v23 - v01));
    matrix_[2] =
        Vector3D(2 * (v13 - v02), 2 * (v23 + v01), v0s - v1s - v2s + v3s);
  }

 public:
  //! Rotation3D copy constructor, faster than default copy constructor
  //! in some cases
  Rotation3D(const Rotation3D &rot) :
  v_(rot.v_), has_cache_(rot.has_cache_)
  {
    if(has_cache_){
      matrix_[0]=rot.matrix_[0];
      matrix_[1]=rot.matrix_[1];
      matrix_[2]=rot.matrix_[2];
    }
  }

  //!  IMP_CXX11_DEFAULT_COPY_CONSTRUCTOR(Rotation3D);

  //! Create a rotation from a vector of 4 quaternion coefficients.
  //! @note: use assume_normalized with care - inputting an unnormalized
  //!        vector would result in unexpected results if it is true
  explicit Rotation3D(const VectorD<4> &v,
                      bool assume_normalized=false)
    : v_(assume_normalized ? v : v.get_unit_vector()),
    has_cache_(false) {}

  //! Create an invalid rotation
  Rotation3D() : v_(0, 0, 0, 0), has_cache_(false) {}
  //! Create a rotation from a quaternion
  /** \throw ValueException if the rotation is not a unit vector.
   */
  Rotation3D(double a, double b, double c, double d)
      : v_(a, b, c, d), has_cache_(false) {
    IMP_USAGE_CHECK_FLOAT_EQUAL(
        v_.get_squared_magnitude(), 1.0,
        "Attempting to construct a rotation from a "
            << " non-quaternion value. The coefficient vector"
            << " must have a length of 1. Got: " << a << " " << b << " " << c
            << " " << d << " gives " << v_.get_squared_magnitude());
    if (a < 0) {
      // make them canonical
      v_ = -v_;
    }
  }
  ~Rotation3D(){}

#ifndef IMP_DOXYGEN
  Vector3D get_rotated_no_cache(const Vector3D &o) const {
    IMP_USAGE_CHECK(get_is_valid(),
                    "Attempting to access uninitialized rotation");
    return Vector3D(
        (v_[0] * v_[0] + v_[1] * v_[1] - v_[2] * v_[2] - v_[3] * v_[3]) * o[0] +
            2 * (v_[1] * v_[2] - v_[0] * v_[3]) * o[1] +
            2 * (v_[1] * v_[3] + v_[0] * v_[2]) * o[2],
        2 * (v_[1] * v_[2] + v_[0] * v_[3]) * o[0] +
            (v_[0] * v_[0] - v_[1] * v_[1] + v_[2] * v_[2] - v_[3] * v_[3]) *
                o[1] +
            2 * (v_[2] * v_[3] - v_[0] * v_[1]) * o[2],
        2 * (v_[1] * v_[3] - v_[0] * v_[2]) * o[0] +
            2 * (v_[2] * v_[3] + v_[0] * v_[1]) * o[1] +
            (v_[0] * v_[0] - v_[1] * v_[1] - v_[2] * v_[2] + v_[3] * v_[3]) *
                o[2]);
  }

  //! Get only the requested rotation coordinate of the vector
  double get_rotated_one_coordinate_no_cache(const Vector3D &o,
                                             unsigned int coord) const {
    IMP_USAGE_CHECK(get_is_valid(),
                    "Attempting to apply uninitialized rotation");
    switch (coord) {
      case 0:
        return (v_[0] * v_[0] + v_[1] * v_[1] - v_[2] * v_[2] - v_[3] * v_[3]) *
                   o[0] +
               2 * (v_[1] * v_[2] - v_[0] * v_[3]) * o[1] +
               2 * (v_[1] * v_[3] + v_[0] * v_[2]) * o[2];
        break;
      case 1:
        return 2 * (v_[1] * v_[2] + v_[0] * v_[3]) * o[0] +
               (v_[0] * v_[0] - v_[1] * v_[1] + v_[2] * v_[2] - v_[3] * v_[3]) *
                   o[1] +
               2 * (v_[2] * v_[3] - v_[0] * v_[1]) * o[2];

        break;
      case 2:
        return 2 * (v_[1] * v_[3] - v_[0] * v_[2]) * o[0] +
               2 * (v_[2] * v_[3] + v_[0] * v_[1]) * o[1] +
               (v_[0] * v_[0] - v_[1] * v_[1] - v_[2] * v_[2] + v_[3] * v_[3]) *
                   o[2];
        break;
      default:
        IMP_THROW("Out of range coordinate " << coord, IndexException);
    }
  }
#endif
  //! Rotate a vector around the origin
  Vector3D get_rotated(const Vector3D &o) const {
    if (!has_cache_) fill_cache();
    return Vector3D(o * matrix_[0], o * matrix_[1], o * matrix_[2]);
  }

  //! Get only the requested rotation coordinate of the vector
  double get_rotated_one_coordinate(const Vector3D &o,
                                    unsigned int coord) const {
    if (!has_cache_) fill_cache();
    return o * matrix_[coord];
  }

  //! Rotate a vector around the origin
  Vector3D operator*(const Vector3D &v) const { return get_rotated(v); }
  Vector3D get_rotation_matrix_row(int i) const {
    IMP_USAGE_CHECK((i >= 0) && (i <= 2), "row index out of range");
    if (!has_cache_) fill_cache();
    return matrix_[i];
  }
  IMP_SHOWABLE_INLINE(Rotation3D,
  { out << v_[0] << " " << v_[1] << " " << v_[2] << " " << v_[3]; });

  //! Return the rotation which undoes this rotation.
  inline Rotation3D get_inverse() const {
    IMP_USAGE_CHECK(get_is_valid(),
                    "Attempting to invert uninitialized rotation");
    Rotation3D ret(v_[0], -v_[1], -v_[2], -v_[3]);
    return ret;
  }

  //! Return the quaternion so that it can be stored
  /** Note that there is no guarantee on which of the two
      equivalent quaternions is returned.
  */
  const Vector4D &get_quaternion() const {
    IMP_USAGE_CHECK(get_is_valid(),
                    "Attempting to access uninitialized rotation");
    return v_;
  }

  //! Multiply two rotations
  Rotation3D operator*(const Rotation3D &q) const {
    IMP_USAGE_CHECK(get_is_valid(),
                    "Attempting to compose uninitialized rotation");
    return compose(*this, q);
  }

  //! Compute the rotation which when composed with r gives this
  Rotation3D operator/(const Rotation3D &r) const {
    IMP_USAGE_CHECK(get_is_valid(),
                    "Attempting to compose uninitialized rotation");
    return compose(*this, r.get_inverse());
  }

  const Rotation3D &operator/=(const Rotation3D &r) {
    *this = *this / r;
    return *this;
  }

  /** \brief Return the derivative of the local position o with respect to
      the i'th internal quaternion coefficient, for i in [0..3],
      namely (dx/dQi, dy/dQi, dz/dQi)
  */
  const Vector3D get_derivative(const Vector3D &o, unsigned int i) const;

  /** Return true is the rotation is valid, false if
      invalid or not initialized (e.g., only initialized by
      the empty constructor)
  */
  bool get_is_valid() const {
    return  v_.get_squared_magnitude() > 0; // TODO: add that magnitude ~ 1?
  }
};

IMP_VALUES(Rotation3D, Rotation3Ds);

//! Return a rotation that does not do anything
/** \see Rotation3D */
inline Rotation3D get_identity_rotation_3d() { return Rotation3D(1, 0, 0, 0); }

//! Return a distance between the two rotations
/** The distance runs between 0 and 1. More precisely,
    the distance returned is distance between the two
    quaternion vectors properly normalized, divided
    by sqrt(2).

    A vector with distance d from the unit vector
    represents a rotation of

    \f$ \theta= \cos^{-1}\left(1-\sqrt{2}d\right)\f$
    \see Rotation3D
*/
inline double get_distance(const Rotation3D &r0, const Rotation3D &r1) {
  double dot =
      (r0.get_quaternion() - r1.get_quaternion()).get_squared_magnitude();
  double odot =
      (r0.get_quaternion() + r1.get_quaternion()).get_squared_magnitude();
  double ans = std::min(dot, odot);
  // TODO: barak - added static for efficiency
  static const double s2 = std::sqrt(2.0);
  double ret = ans / s2;
  return std::max(std::min(ret, 1.0), 0.0);
}

//! Generate a Rotation3D object from a rotation around an axis
//! that is assumed to be normalized
/**
   \param[in] axis_norm the normalized rotation axis passing through (0,0,0)
   \param[in] angle the rotation angle in radians in the
   clockwise direction
   \note http://en.wikipedia.org/wiki/Rotation_matrix
   \note www.euclideanspace.com/maths/geometry/rotations/conversions/
   angleToQuaternion/index.htm
   \see Rotation3D
*/
inline Rotation3D
get_rotation_about_normalized_axis
(const Vector3D &axis_norm, double angle)
{
  IMP_USAGE_CHECK(axis_norm.get_magnitude() - 1.0 < 1e-6,
                  "expected normalized vector as axis of rotation");
  double s = std::sin(angle / 2);
  double a, b, c, d;
  a = std::cos(angle / 2);
  b = axis_norm[0] * s;
  c = axis_norm[1] * s;
  d = axis_norm[2] * s;
  return Rotation3D(a, b, c, d);
}


//! Generate a Rotation3D object from a rotation around an axis
/**
   \param[in] axis the rotation axis passes through (0,0,0)
   \param[in] angle the rotation angle in radians in the
   clockwise direction
   \note http://en.wikipedia.org/wiki/Rotation_matrix
   \note www.euclideanspace.com/maths/geometry/rotations/conversions/
   angleToQuaternion/index.htm
   \see Rotation3D
*/
inline Rotation3D
get_rotation_about_axis
(const Vector3D &axis, double angle)
{
    // normalize the vector
    Vector3D axis_norm = axis.get_unit_vector();
  return get_rotation_about_normalized_axis(axis_norm, angle);
}

//! Create a rotation from the first vector to the second one.
/** \see Rotation3D
 */
IMPALGEBRAEXPORT Rotation3D
    get_rotation_taking_first_to_second(const Vector3D &v1, const Vector3D &v2);

//! Generate a Rotation3D object from a rotation matrix
/**
   \see Rotation3D
*/
IMPALGEBRAEXPORT Rotation3D
    get_rotation_from_matrix(double m00, double m01, double m02, double m10,
                             double m11, double m12, double m20, double m21,
                             double m22);

//! Generate a Rotation3D object from a rotation matrix
/**
   \see Rotation3D
*/
IMPALGEBRAEXPORT Rotation3D get_rotation_from_matrix(Eigen::Matrix3d m);

//! Pick a rotation at random from all possible rotations
/** \see Rotation3D */
IMPALGEBRAEXPORT Rotation3D get_random_rotation_3d();

//! Pick a rotation at random near the provided one
/** This method generates a rotation that is within the provided
    distance of center.
    \param[in] center The center of the rotational volume
    \param[in] distance See
    get_distance(const Rotation3D&,const Rotation3D&)
    for a full definition.

    \note The cost of this operation increases as distance goes to 0.

    \see Rotation3D
*/
IMPALGEBRAEXPORT Rotation3D
    get_random_rotation_3d(const Rotation3D &center, double distance);

//! Cover the space of rotations evenly
/** If you care about the distance between samples instead of the number
    of samples, the "surface area" of the set of rotations is pi^2. If
    you allocate each sample a volume of 4/3 pi d^3 (to space them d apart),
    Then you want 3/4 pi/d^3 points.

    Creates at least num_points rotations.
*/
IMPALGEBRAEXPORT Rotation3Ds
    get_uniform_cover_rotations_3d(unsigned int num_points);

//! Generates a nondegenerate set of Euler angles with a delta resolution
/**
\param[in] delta sample every delta angles in radians.
 */
IMPALGEBRAEXPORT algebra::Rotation3Ds get_uniformly_sampled_rotations(
    double delta);

//! Compute a rotation from an unnormalized quaternion
/** \see Rotation3D */
inline Rotation3D get_rotation_from_vector4d(const VectorD<4> &v) {
  VectorD<4> uv = v.get_unit_vector();
  return Rotation3D(uv[0], uv[1], uv[2], uv[3]);
}

/** \see Rotation3D
 */
inline Rotation3D compose(const Rotation3D &a, const Rotation3D &b) {
  return Rotation3D(a.v_[0] * b.v_[0] - a.v_[1] * b.v_[1] - a.v_[2] * b.v_[2] -
                        a.v_[3] * b.v_[3],
                    a.v_[0] * b.v_[1] + a.v_[1] * b.v_[0] + a.v_[2] * b.v_[3] -
                        a.v_[3] * b.v_[2],
                    a.v_[0] * b.v_[2] - a.v_[1] * b.v_[3] + a.v_[2] * b.v_[0] +
                        a.v_[3] * b.v_[1],
                    a.v_[0] * b.v_[3] + a.v_[1] * b.v_[2] - a.v_[2] * b.v_[1] +
                        a.v_[3] * b.v_[0]);
}

/** \name Euler Angles
    There are many conventions for how to define Euler angles, based on choices
    of which of the x,y,z axis to use in what order and whether the rotation
    axis is in the body frame (and hence affected by previous rotations) or in
    in a fixed frame. See
    http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
    for a general description.

    - All Euler angles are specified in radians.
    - Only x-y-z order is currently supported.
    - To convert Euler angles in a different order, one can compose a Rotation3D
    from three rotations using get_rotation_about_axis function.
    @{
*/

//! Initialize a rotation in x-y-z order from three angles
/** \param[in] xr Rotation around the X axis in radians
    \param[in] yr Rotation around the Y axis in radians
    \param[in] zr Rotation around the Z axis in radians
    \note The three rotations are represented in the original (fixed)
    coordinate frame.
    \see Rotation3D
    \see FixedXYZ
*/
IMPALGEBRAEXPORT Rotation3D
    get_rotation_from_fixed_xyz(double xr, double yr, double zr);

//! Initialize a rotation from Euler angles
/**
   \param[in] phi   Rotation around the Z axis in radians
   \param[in] theta Rotation around the X axis in radians
   \param[in] psi   Rotation around the Z axis in radians
   \note The first rotation is by an angle phi about the z-axis.
   The second rotation is by an angle theta in [0,pi] about the
   former x-axis , and the third rotation is by an angle psi
   about the former z-axis.
   \see Rotation3D
*/
IMPALGEBRAEXPORT Rotation3D
    get_rotation_from_fixed_zxz(double phi, double theta, double psi);

//! Generate a rotation object from Euler Angles
/** \note The first rotation is by an angle about the z-axis.
    The second rotation is by an angle about the new y-axis.
    The third rotation is by an angle about the new z-axis.
    \param[in] Rot First Euler angle (radians) defining the rotation (Z axis)
    \param[in] Tilt Second Euler angle (radians) defining the rotation (Y axis)
    \param[in] Psi Third Euler angle (radians) defining the rotation (Z axis)
    \see Rotation3D
*/
IMPALGEBRAEXPORT Rotation3D
    get_rotation_from_fixed_zyz(double Rot, double Tilt, double Psi);

//! A simple class for returning XYZ Euler angles
class FixedXYZ : public GeometricPrimitiveD<3> {
  double v_[3];

 public:
  FixedXYZ() {}
  FixedXYZ(double x, double y, double z) {
    v_[0] = x;
    v_[1] = y;
    v_[2] = z;
  }
  double get_x() const { return v_[0]; }
  double get_y() const { return v_[1]; }
  double get_z() const { return v_[2]; }
  IMP_SHOWABLE_INLINE(FixedXYZ,
  { out << v_[0] << " " << v_[1] << " " << v_[2]; });
};

IMP_VALUES(FixedXYZ, FixedXYZs);

//! The inverse of rotation_from_fixed_xyz()
/**
   \see rotation_from_fixed_xyz()
   \see Rotation3D
   \see FixesXYZ
*/
IMPALGEBRAEXPORT FixedXYZ get_fixed_xyz_from_rotation(const Rotation3D &r);

/** @}*/

//! Interpolate between two rotations
/** It f ==0, return b, if f==1 return a.
    \see Rotation3D */
inline Rotation3D get_interpolated(const Rotation3D &a, const Rotation3D &b,
                                   double f) {
  VectorD<4> bq = b.get_quaternion(), aq = a.get_quaternion();
  if (bq * aq < 0) bq = -bq;
  return Rotation3D(f * aq + (1 - f) * bq);
}

/** Return the rotation which takes the native x and y axes to the
    given x and y axes.
    The two axes must be perpendicular unit vectors.
*/
IMPALGEBRAEXPORT Rotation3D
    get_rotation_from_x_y_axes(const Vector3D &x, const Vector3D &y);

//! Decompose a Rotation3D object into a rotation around an axis
/** For all identity rotations, returns the axis [1,0,0] and the angle 0.0.

   \note http://en.wikipedia.org/wiki/Rotation_matrix
   \note www.euclideanspace.com/maths/geometry/rotations/conversions/
   angleToQuaternion/index.htm
   \see Rotation3D

   @return axis direction and rotation about the axis in Radians
*/
IMPALGEBRAEXPORT std::pair<Vector3D, double> get_axis_and_angle(
    const Rotation3D &rot);

typedef std::pair<Vector3D, double> AxisAnglePair;
#ifndef IMP_DOXYGEN
typedef Vector<AxisAnglePair> AxisAnglePairs;
#endif

IMPALGEBRA_END_NAMESPACE
#endif /* IMPALGEBRA_ROTATION_3D_H */
