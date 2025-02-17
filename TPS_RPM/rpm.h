#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <vector>

using namespace Eigen;
using namespace std;

#define RPM_USE_BOTHSIDE_OUTLIER_REJECTION
#define RPM_REGULARIZE_AFFINE_PARAM

namespace rpm {
    const int D = 2;
    // Annealing params
    extern double T_start, T_end;
    extern double r, I0, epsilon0;
    extern double alpha; // 5 * 5
    // Softassign params
    extern double I1, epsilon1;
    // Thin-plate spline params
    extern double lambda_start;
    extern double r_lambda;

    extern double scale;  // for visualize

    void set_T_start(double T, double scale);

    class ThinPlateSplineParams {
    public:
        ThinPlateSplineParams(const MatrixXd &X);

        ThinPlateSplineParams(const ThinPlateSplineParams &other);

        // (D + 1) * (D + 1) matrix representing the affine transformation.
        MatrixXd d;
        // K * (D + 1) matrix representing the non-affine deformation.
        MatrixXd w;

        MatrixXd applyTransform(bool hnormalize = false) const;

        MatrixXd applyTransform(const MatrixXd &P, bool hnormalize = false) const;

        Vector2d applyTransform(const Vector2d &p, bool hnormalize = false) const;

        MatrixXd get_phi() { return phi; };

        MatrixXd get_Q() { return Q; };

        MatrixXd get_R() { return R; };

    private:
        MatrixXd X;

        // K * K matrix
        MatrixXd phi;

        // Q, R
        MatrixXd Q, R;
    };

    // Compute the thin-plate spline params and 2d point correspondence from two point sets.
    //
    // Input:
    //   X, Y		source and target points set.
    // Output:
    //	 M			correspondence between X and Y
    //	 params		thin-plate spline params
    // Returns true on success, false on failure
    //
    bool estimate(
            const MatrixXd &X,
            const MatrixXd &Y,
            MatrixXd &M,
            ThinPlateSplineParams &params,
            const vector<pair<int, int> > &matched_point_indices = vector<pair<int, int> >()
    );

    bool init_params(
            const MatrixXd &X,
            const MatrixXd &Y,
            const double T,
            MatrixXd &M,
            ThinPlateSplineParams &params
    );

    // Compute the thin-plate spline parameters from two point sets.
    //
    // Input:
    //   X, Y		source and target points set.
    //	 params		thin-plate spline params
    //	 T			temperature
    // Output:
    //	 M			correspondence between X and Y
    // Returns true on success, false on failure
    //
    bool estimate_correspondence(
            const MatrixXd &X,
            const MatrixXd &Y,
            const vector<pair<int, int> > &matched_point_indices,
            const ThinPlateSplineParams &params,
            const double T,
            const double T0,
            MatrixXd &M
    );

    // Compute the thin-plate spline parameters from two point sets.
    //
    // Input:
    //   X, Y		source and target points set.
    //	 M			correspondence between X and Y
    // Output:
    //	 params		thin-plate spline params
    // Returns true on success, false on failure
    //
    bool estimate_transform(
            const MatrixXd &X,
            const MatrixXd &Y,
            const MatrixXd &M,
            const double lambda,
            ThinPlateSplineParams &params
    );

    MatrixXd apply_correspondence(
            const MatrixXd &Y,
            const MatrixXd &M);
}


