/* ****************************************************************************
* Copyright (C) 2009 Technische Universitaet Muenchen                         *
* This file is part of the SG++ project. For conditions of distribution and   *
* use, please see the copyright notice at http://www5.in.tum.de/SGpp          *
**************************************************************************** */
// @author Fabian Franzelin, fabian.franzelin@ipvs.uni-stuttgart.de

#ifndef OPERATIONROSENBLATTTRANSFORMATIONLINEAR_HPP
#define OPERATIONROSENBLATTTRANSFORMATIONLINEAR_HPP

#include "base/grid/Grid.hpp"
#include "datadriven/operation/OperationRosenblattTransformation.hpp"

namespace sg {
  namespace datadriven {

    /**
     * keep applying marginalize to function until it's reduced to only 1 dimension
     */

    class OperationRosenblattTransformationLinear : public OperationRosenblattTransformation {
      public:
    	OperationRosenblattTransformationLinear(base::Grid* grid) : grid(grid) {}
        virtual ~OperationRosenblattTransformationLinear() {}

        /**
         * Transformation with mixed starting dimensions
         *
         * @param alpha Coefficient vector for current grid
         * @param samples Output DataMatrix (rows: # of samples, columns: # of dims)
         * @param num_samples # of samples to draw
         */
        void doTransformation(base::DataVector* alpha, base::DataMatrix* points, base::DataMatrix* pointscdf);

 		/**
         * Transformation with specified starting dimension
         *
         * @param alpha Coefficient vector for current grid
         * @param samples Output DataMatrix (rows: # of samples, columns: # of dims)
         * @param num_samples # of samples to draw
         * @param dim_start Starting dimension
         */
        void doTransformation(base::DataVector* alpha, base::DataMatrix* points, base::DataMatrix* pointscdf, size_t dim_start);

      protected:
        base::Grid* grid;
        void doTransformation_start_dimX(base::Grid* g_in, base::DataVector* a_in, size_t dim_start, base::DataVector* coords1d, base::DataVector* cdfs1d);
        void doTransformation_in_next_dim(base::Grid* g_in, base::DataVector* a_in, size_t dim_x, base::DataVector* coords1d, base::DataVector* cdfs1d, size_t& curr_dim);
        virtual double doTransformation1D(base::Grid* grid1d, base::DataVector* alpha1d, double coord1d);
    };

  }
}
#endif /* OPERATIONROSENBLATTTRANSFORMATIONLINEAR_HPP */





