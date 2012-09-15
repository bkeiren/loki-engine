//////////////////////////////////////////////////////////////////////////
// This file includes all of GLM's header files. This file is intended
// to be included into a precompiled header file.
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef INCLUDE_ALL_H
#define INCLUDE_ALL_H

#include "math/glm/glm.hpp"
#include "math/glm/ext.hpp"

// GLM Core.
#include "math/glm/core/func_common.hpp"
#include "math/glm/core/func_exponential.hpp"
#include "math/glm/core/func_geometric.hpp"
#include "math/glm/core/func_integer.hpp"
#include "math/glm/core/func_matrix.hpp"
#include "math/glm/core/func_noise.hpp"
#include "math/glm/core/func_packing.hpp"
#include "math/glm/core/func_trigonometric.hpp"
#include "math/glm/core/func_vector_relational.hpp"
#include "math/glm/core/type.hpp"

// GLM GTC
#include "math/glm/gtc/half_float.hpp"
#include "math/glm/gtc/matrix_access.hpp"
#include "math/glm/gtc/matrix_integer.hpp"
#include "math/glm/gtc/matrix_inverse.hpp"
#include "math/glm/gtc/matrix_transform.hpp"
#include "math/glm/gtc/quaternion.hpp"
#include "math/glm/gtc/swizzle.hpp"
#include "math/glm/gtc/type_precision.hpp"
#include "math/glm/gtc/type_ptr.hpp"

// GLM GTX
#include "math/glm/gtx/associated_min_max.hpp"
#include "math/glm/gtx/bit.hpp"
#include "math/glm/gtx/closest_point.hpp"
#include "math/glm/gtx/color_cast.hpp"
#include "math/glm/gtx/color_space.hpp"
#include "math/glm/gtx/color_space_YCoCg.hpp"
#include "math/glm/gtx/compatibility.hpp"
#include "math/glm/gtx/component_wise.hpp"
#include "math/glm/gtx/epsilon.hpp"
#include "math/glm/gtx/euler_angles.hpp"
#include "math/glm/gtx/extend.hpp"
#include "math/glm/gtx/extented_min_max.hpp"
#include "math/glm/gtx/fast_exponential.hpp"
#include "math/glm/gtx/fast_square_root.hpp"
#include "math/glm/gtx/fast_trigonometry.hpp"
#include "math/glm/gtx/gradient_paint.hpp"
#include "math/glm/gtx/handed_coordinate_space.hpp"
#include "math/glm/gtx/inertia.hpp"
#include "math/glm/gtx/int_10_10_10_2.hpp"
#include "math/glm/gtx/integer.hpp"
#include "math/glm/gtx/intersect.hpp"
#include "math/glm/gtx/log_base.hpp"
#include "math/glm/gtx/matrix_cross_product.hpp"
#include "math/glm/gtx/matrix_interpolation.hpp"
#include "math/glm/gtx/matrix_major_storage.hpp"
#include "math/glm/gtx/matrix_operation.hpp"
#include "math/glm/gtx/matrix_query.hpp"
#include "math/glm/gtx/mixed_product.hpp"
#include "math/glm/gtx/multiple.hpp"
#include "math/glm/gtx/noise.hpp"
#include "math/glm/gtx/norm.hpp"
#include "math/glm/gtx/normal.hpp"
#include "math/glm/gtx/normalize_dot.hpp"
#include "math/glm/gtx/number_precision.hpp"
#include "math/glm/gtx/ocl_type.hpp"
#include "math/glm/gtx/optimum_pow.hpp"
#include "math/glm/gtx/orthonormalize.hpp"
#include "math/glm/gtx/perpendicular.hpp"
#include "math/glm/gtx/polar_coordinates.hpp"
#include "math/glm/gtx/projection.hpp"
#include "math/glm/gtx/quaternion.hpp"
#include "math/glm/gtx/random.hpp"
#include "math/glm/gtx/raw_data.hpp"
#include "math/glm/gtx/reciprocal.hpp"
#include "math/glm/gtx/rotate_vector.hpp"

#ifdef MATH_USE_SIMD
	#include "math/glm/gtx/simd_mat4.hpp"
	#include "math/glm/gtx/simd_vec4.hpp"
#endif

//#include "math/glm/gtx/simplex.hpp"
#include "math/glm/gtx/spline.hpp"
#include "math/glm/gtx/std_based_type.hpp"
#include "math/glm/gtx/string_cast.hpp"
#include "math/glm/gtx/transform.hpp"
#include "math/glm/gtx/transform2.hpp"
#include "math/glm/gtx/ulp.hpp"
#include "math/glm/gtx/unsigned_int.hpp"
#include "math/glm/gtx/vec1.hpp"
#include "math/glm/gtx/vector_access.hpp"
#include "math/glm/gtx/vector_angle.hpp"
#include "math/glm/gtx/vector_query.hpp"
#include "math/glm/gtx/verbose_operator.hpp"
#include "math/glm/gtx/wrap.hpp"

namespace loki
{

// Use 'math' as an alias for GLM.
namespace math = glm;

}

// Common math.
#include "math/math_common.h"

#endif