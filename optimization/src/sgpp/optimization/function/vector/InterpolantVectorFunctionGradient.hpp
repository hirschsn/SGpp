// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#ifndef SGPP_OPTIMIZATION_FUNCTION_VECTOR_INTERPOLANTVECTORFUNCTIONGRADIENT_HPP
#define SGPP_OPTIMIZATION_FUNCTION_VECTOR_INTERPOLANTVECTORFUNCTIONGRADIENT_HPP

#include <sgpp/globaldef.hpp>

#include <sgpp/base/datatypes/DataVector.hpp>
#include <sgpp/base/datatypes/DataMatrix.hpp>
#include <sgpp/base/operation/hash/OperationEvalGradient.hpp>
#include <sgpp/base/operation/BaseOpFactory.hpp>
#include <sgpp/base/grid/Grid.hpp>
#include <sgpp/optimization/function/vector/VectorFunctionGradient.hpp>

#include <cstddef>

namespace sgpp {
namespace optimization {

/**
 * Sparse grid interpolant gradient of a vector-valued function.
 *
 * @see InterpolantVectorFunction
 */
class InterpolantVectorFunctionGradient : public VectorFunctionGradient {
 public:
  /**
   * Constructor.
   * Do not destruct the grid before the
   * InterpolantVectorFunctionGradient object!
   *
   * @param grid  sparse grid
   * @param alpha coefficient matrix
   *              (j-th column contains hierarchical surplusses
   *              \f$\alpha_{\cdot,j}\f$ of \f$g_j\f$)
   */
  InterpolantVectorFunctionGradient(base::Grid& grid, const base::DataMatrix& alpha)
      : VectorFunctionGradient(grid.getDimension(), alpha.getNcols()),
        grid(grid),
        opEvalGradient(op_factory::createOperationEvalGradientNaive(grid)),
        alpha(alpha) {}

  /**
   * Destructor.
   */
  ~InterpolantVectorFunctionGradient() override {}

  /**
   * Evaluation of the function and its gradient.
   *
   * @param[in]  x        evaluation point \f$\vec{x} \in [0, 1]^d\f$
   * @param[out] value    \f$g(\vec{x})\f$
   * @param[out] gradient Jacobian \f$\nabla g(\vec{x}) \in
   *                      \mathbb{R}^{m \times d}\f$
   */
  inline void eval(const base::DataVector& x, base::DataVector& value,
                   base::DataMatrix& gradient) override {
    for (size_t t = 0; t < d; t++) {
      if ((x[t] < 0.0) || (x[t] > 1.0)) {
        value.setAll(INFINITY);
        return;
      }
    }

    opEvalGradient->evalGradient(alpha, x, value, gradient);
  }

  /**
   * @param[out] clone pointer to cloned object
   */
  void clone(std::unique_ptr<VectorFunctionGradient>& clone) const override {
    clone =
        std::unique_ptr<VectorFunctionGradient>(new InterpolantVectorFunctionGradient(grid, alpha));
  }

  /**
   * @return coefficient matrix
   */
  const base::DataMatrix& getAlpha() const { return alpha; }

  /**
   * @param alpha coefficient matrix
   */
  void setAlpha(const base::DataMatrix& alpha) { this->alpha = alpha; }

 protected:
  /// sparse grid
  base::Grid& grid;
  /// pointer to evaluation operation
  std::unique_ptr<base::OperationEvalGradient> opEvalGradient;
  /// coefficient matrix
  base::DataMatrix alpha;
};
}  // namespace optimization
}  // namespace sgpp

#endif /* SGPP_OPTIMIZATION_FUNCTION_VECTOR_INTERPOLANTVECTORFUNCTIONGRADIENT_HPP */
