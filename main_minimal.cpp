#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <stdlib.h>

#define __CL_ENABLE_EXCEPTIONS
#ifdef __APPLE__
# include <OpenCL/cl.hpp>
#else
# include <CL/cl.hpp>
#endif

int main ()
{
    const int N_ELEMENTS=1024*1024;
    int platform_id=0, device_id=0;

    try{
        std::vector<int> A(N_ELEMENTS);
        std::vector<int> B(N_ELEMENTS);
        std::vector<int> C(N_ELEMENTS);

        for( int i = 0; i < N_ELEMENTS; ++i )
        {
            A[i] = i;
            B[i] = i;
        }

        // Get default context
        cl::Context(CL_DEVICE_TYPE_DEFAULT);

        cl::Buffer bufferA(A.begin(), A.end(), true, false);
        cl::Buffer bufferB(B.begin(), B.end(), true, false);
        cl::Buffer bufferC(CL_MEM_READ_WRITE, N_ELEMENTS * sizeof(float));

        // Read the program source
        std::ifstream sourceFile("vecadd.cl");
        std::string sourceCode( std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));

        // Make program from the source code
        cl::Program program(sourceCode.c_str(), true);

        // Make kernel
        cl::make_kernel<cl::Buffer&, cl::Buffer&, cl::Buffer&> vecadd_kernel(program, "vecadd");

        // Execute the kernel
        vecadd_kernel(cl::EnqueueArgs(N_ELEMENTS), bufferA, bufferB, bufferC);

        // Copy the output data back to the host
        cl::copy(bufferC, C.begin(), C.end());

        // Verify the result
        bool result=true;
        for (int i=0; i<N_ELEMENTS; i++)
        {
            if (C[i] !=A[i]+B[i])
            {
                result = false;
                break;
            }
        }
        if (result)
            std::cout<< "Success!\n";
        else
            std::cout<< "Failed!\n";
    }
    catch (cl::Error err) {
        std::cout << "Error: " << err.what() << "(" << err.err() << ")" << std::endl;
        return( EXIT_FAILURE );
    }

    std::cout << "Done.\n";
    return( EXIT_SUCCESS );
}
