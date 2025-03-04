__global__ void computeKernel(float* A, int N, int M) {
    int i = blockIdx.y * blockDim.y + threadIdx.y + 1; // Row index
    int j = blockIdx.x * blockDim.x + threadIdx.x + 1; // Column index
    
    // Check if thread is within valid bounds
    if (i < N && j < M) {
        // Assuming A is stored in row-major order
        int idx = i * M + j;         // Current position
        int idx_up = (i-1) * M + j;  // Top neighbor
        int idx_left = i * M + (j-1); // Left neighbor
        int idx_diag = (i-1) * M + (j-1); // Top-left neighbor
        
        // Apply the function f to the three neighboring values
        A[idx] = f(A[idx_up], A[idx_left], A[idx_diag]);
    }
}