__global__ void matrixMulNaive(float *A, float *B, float *C, int width) {
    // Calculate row and column indices
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    // Check if we're within matrix bounds
    if (row < width && col < width) {
        float sum = 0.0f;
        // Perform dot product
        for (int k = 0; k < width; k++) {
            sum += A[row * width + k] * B[k * width + col];
        }
        // Store result
        C[row * width + col] = sum;
    }
}