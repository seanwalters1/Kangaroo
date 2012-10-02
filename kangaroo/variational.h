#pragma once

#include "kangaroo.h"

namespace Gpu {

//////////////////////////////////////////////////////

void Divergence(Image<float> divA, Image<float2> A);

//////////////////////////////////////////////////////

void TVL1GradU_DualAscentP(
        Image<float2> p, Image<float> u,
        float sigma
);

void HuberGradU_DualAscentP(
        Image<float2> p, const Image<float> u,
        float sigma, float alpha
);

void WeightedHuberGradU_DualAscentP(
        Image<float2> imgp, const Image<float> imgu, const Image<float> imgw,
        float sigma, float alpha
);

void L2_u_minus_g_PrimalDescent(
        Image<float> imgu, const Image<float2> imgp, const Image<float> imgg,
        float tau, float lambda
);

void L2_u_minus_g_PrimalDescent(
        Image<float> imgu, const Image<float2> imgp, const Image<float> imgg,
        const Image<float> imglambdaweight,
        float tau, float lambda
);

void WeightedL2_u_minus_g_PrimalDescent(
        Image<float> imgu, const Image<float2> imgp, const Image<float> imgg, const Image<float> imgw,
        float tau, float lambda
);

//////////////////////////////////////////////////////

void DeconvolutionDual_qAscent(
        Image<float> q, const Image<float> Au, const Image<float> g,
        float sigma_q, float lambda
);

void Deconvolution_uDescent(
        Image<float> imgu, const Image<float2> imgp, const Image<float> imgATq,
        float tau, float lambda
);

//////////////////////////////////////////////////////

void GradU(Image<float2> imgv, Image<float> imgu);

void TGV_L1_DenoisingIteration(
    Image<float> imgu, Image<float2> imgv,
    Image<float2> imgp, Image<float4> imgq, Image<float> imgr,
    Image<float> imgf,
    float alpha0, float alpha1,
    float sigma, float tau,
    float delta
);

}