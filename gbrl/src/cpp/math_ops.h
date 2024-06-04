//////////////////////////////////////////////////////////////////////////////
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
//  property and proprietary rights in and to this material, related
//  documentation and any modifications thereto. Any use, reproduction,
//  disclosure or distribution of this material and related documentation
//  without an express license agreement from NVIDIA CORPORATION or
//  its affiliates is strictly prohibited.
//
//////////////////////////////////////////////////////////////////////////////
#ifndef MATH_OPS_H
#define MATH_OPS_H

#include <cmath>

void add_vec_to_mat(float *mat, const float *vec, const int n_samples, const int n_cols, const int par_th);
void subtract_vec_from_mat(float *mat, float *vec, const int n_samples, const int n_cols, const int par_th);
float* calculate_mean(const float *mat, const int n_samples, const int n_cols, const int par_th);
float* calculate_var(const float *mat, const float *mean, const int n_samples, const int n_cols, const int par_th);
void multiply_mat_by_scalar(float *mat, float scalar, const int n_samples, const int n_cols, const int par_th);
float* calculate_var_and_center(float *mat, const float *mean, const int n_samples, const int n_cols, const int par_th);
float* calculate_std_and_center(float *mat, const float *mean, const int n_samples, const int n_cols, const int par_th);
float* copy_mat(const float *mat, const int size, const int par_th);
float* calculate_row_covariance(const float *mat_l, const float *mat_r, const int n_samples, const int n_cols, const int par_th);
void _element_wise_addition(float *mat_l, const float *mat_r, const int size, const int par_th);
float* element_wise_division(const float *mat_l, const float *mat_r, const int size, const int par_th);
void multiply_mat_by_vec_subtract_result(float *result, const float *mat, const float *vec, const int n_samples, const int n_cols, const int par_th);
void divide_mat_by_vec_inplace(float *mat, const float *vec, const int n_samples, const int n_cols, const int par_th);
float* init_zero_mat(const int size, const int par_th);
void set_zero_mat(float *mat, const int size, const int par_th);
float* calculate_max(const float *mat, const int n_samples, const int n_cols, const int par_th);
float* calculate_min(const float *mat, const int n_samples, const int n_cols, const int par_th);
void calculate_squared_norm(float *norm, const float *mat, const int n_samples, const int n_cols, const int par_th);

inline float mat_vec_dot_sum(const int *indices, const float *grads, const float *vec, const int n_samples, const int n_cols){
    float sum = 0.0f;
    #pragma omp simd
    for (int i = 0; i < n_samples*n_cols; ++i){
        int row = i / n_cols;
        int col = i % n_cols;
        sum += grads[indices[row]*n_cols + col] * vec[col];
    }
    return sum;
}

inline float norm(const float *vec, const int n_samples){
    float sum = 0.0f;
    #pragma omp simd
    for (int n = 0; n < n_samples; ++n){
        sum += (vec[n]*vec[n]);
    }
    return sqrtf(sum);
}

inline float squared_norm(const float *vec, const int n_samples){
    float sum = 0.0f;
    #pragma omp simd
    for (int n = 0; n < n_samples; ++n){
        sum += (vec[n]*vec[n]);
    }
    return sum;
}

inline float cosine_dist(const int *indices, const float *raw_grads, const float *mean, const int n_samples, const int n_cols, float squared_norms){
    if (n_samples == 0)
        return 0.0f;
    float n_samples_f = static_cast<float>(n_samples);
    float sum_dot_product = mat_vec_dot_sum(indices, raw_grads, mean, n_samples, n_cols);
    float mean_norm = norm(mean, n_cols);
    float denominator = mean_norm * sqrtf(squared_norms);
    if (denominator == 0.0f) {
        return 0.0f;
    }
    return (sum_dot_product / denominator) * n_samples_f;
}

#endif 