// Copyright (C) 2008-today The SG++ project
// This file is part of the SG++ project. For conditions of distribution and
// use, please see the copyright notice provided with SG++ or at
// sgpp.sparsegrids.org

#pragma once

#ifdef USE_GSL
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#endif /* USE_GSL */

#include <sgpp/base/exception/algorithm_exception.hpp>
#include <sgpp/datadriven/algorithm/DBMatOffline.hpp>

#include <string>

namespace sgpp {
namespace datadriven {

class DBMatOfflineOrthoAdapt : public DBMatOffline {
 public:
  /**
   * Constructor
   * Builds DBMatOfflineOrthoAdapt Object from configuration
   */
  DBMatOfflineOrthoAdapt();

  /**
   * Constructor
   * Builds object from serialized offline object, stored in file
   *
   * @param fileName path to the file storing object
   */
  explicit DBMatOfflineOrthoAdapt(const std::string& fileName);

  DBMatOffline* clone();

  bool isRefineable() override;

  /**
   * Returns the decomposition type of the DBMatOffline object
   * @return the type of matrix decomposition
   */
  sgpp::datadriven::MatrixDecompositionType getDecompositionType() override;

  /**
   * Builds the left hand side matrix without the regularization term
   * @param grid the underlying grid
   * @param regularizationConfig configuaration for the regularization employed
   */
  void buildMatrix(Grid* grid, RegularizationConfiguration& regularizationConfig);

  /**
   * Decomposes and inverts the lhsMatrix of the offline object
   * (lhs + lambda*I)^{-1} = Q * (T + lambda*I)^{-1} * Q^t = Q * T_inv * Q^t
   *
   * The matrix lhsMatrix of the parent object will be altered during the process
   * @param regularizationConfig the regularization configuration
   * @param densityEstimationConfig the density estimation configuration
   */
  void decomposeMatrix(RegularizationConfiguration& regularizationConfig,
                       DensityEstimationConfiguration& densityEstimationConfig);

  /**
   * Decomposes the lhsMatrix into lhs = Q * T * Q^t and stores the orthogonal
   * matrix Q into the member q_ortho_matrix_. The information to reconstruct T
   * is written into diag and subdiag
   *
   * @param diag Diagonal entries of T
   * @param subdiag Sub- and superdiagonal entries of T (symmetric)
   */
  void hessenberg_decomposition(sgpp::base::DataVector& diag, sgpp::base::DataVector& subdiag);

  /**
   * Inverts a symmetric tridiagonal matrix T, which is given in the form of
   * its diagonal and subdiagonal vectors. When finished, diag and subdiag no
   * longer hold their initial values.
   *
   * @param diag Diagonal entries of T
   * @param subdiag Sub- and superdiagonal entries of T (symmetric)
   */
  void invert_symmetric_tridiag(sgpp::base::DataVector& diag, sgpp::base::DataVector& subdiag);

  /**
   * Serializes the DBMatOfflineOrthoAdapt object
   *
   * q_ortho_matrix_ and t_inv_tridiag_ are stored into the specified file,
   * which is the explicit representation of the decomposition needed for the
   * online phase
   *
   * @param fileName path where to store the file
   */
  void store(const std::string& fileName);

  /**
   * Override to sync Q and Tinv
   */
  void syncDistributedDecomposition(std::shared_ptr<BlacsProcessGrid> processGrid,
                                    const ParallelConfiguration& parallelConfig) override;

  /*
   * explicitly computes the inverse
   * note: the computed inverse is not the inverse of the decomposed matrix,
   * but rather the inverse of the previous undecomposed matrix
   * @param inv the matrix to store the computed inverse
   */
  void compute_inverse() override;

  sgpp::base::DataMatrix& getQ() { return this->q_ortho_matrix_; }

  sgpp::base::DataMatrix& getTinv() { return this->t_tridiag_inv_matrix_; }

  DataMatrixDistributed& getQDistributed() { return this->q_ortho_matrix_distributed_; }

  DataMatrixDistributed& getTinvDistributed() { return this->t_tridiag_inv_matrix_distributed_; }

 protected:
  sgpp::base::DataMatrix q_ortho_matrix_;        // orthogonal matrix of decomposition
  sgpp::base::DataMatrix t_tridiag_inv_matrix_;  // inverse of the tridiag matrix of decomposition

  // distributed matrices, only initialized if scalapack is used
  DataMatrixDistributed q_ortho_matrix_distributed_;
  DataMatrixDistributed t_tridiag_inv_matrix_distributed_;
};
}  // namespace datadriven
}  // namespace sgpp
