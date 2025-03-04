__device__ float computeWeightedAverage(float top, float left, float diag) {
    return 0.4f * top + 0.4f * left + 0.2f * diag;
}