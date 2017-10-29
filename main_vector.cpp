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

        // Query for platforms
        std::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        // Get a list of devices on this platform
        std::vector<cl::Device> devices;
        // Select the platform.
        platforms[platform_id].getDevices(CL_DEVICE_TYPE_GPU|CL_DEVICE_TYPE_CPU, &devices);

        // Create a context
        cl::Context context(devices);

        // Create a command queue
        // Select the device.
        cl::CommandQueue queue(context, devices[device_id]);

        // Create the memory buffers (Implicit copy to buffers)
        cl::Buffer bufferA(context, A.begin(), A.end(), true);
        cl::Buffer bufferB(context, B.begin(), B.end(), true);
        cl::Buffer bufferC(context, CL_MEM_WRITE_ONLY, N_ELEMENTS * sizeof(int));

        // Read the program source
        std::ifstream sourceFile("vecadd.cl");
        std::string sourceCode( std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
        cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()));

        // Make program from the source code
        cl::Program program(context, source);

        // Build the program for the devices
        program.build(devices);

        // Make kernel
        cl::make_kernel<cl::Buffer&, cl::Buffer&, cl::Buffer&> vecadd_kernel(program, "vecadd");

        // Enqueue the kernel arguments
        cl::NDRange global( N_ELEMENTS );
        cl::NDRange local( 256 );
        cl::EnqueueArgs eargs(queue, global, local);

        // Execute the kernel
        vecadd_kernel(eargs, bufferA, bufferB, bufferC);

        // Copy the output data back to the host
        cl::copy(queue, bufferC, C.begin(), C.end());

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
