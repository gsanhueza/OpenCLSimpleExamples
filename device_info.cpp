#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>
#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    try
    {
        // Discover number of platforms
        vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        cout << endl;
        cout << "Number of OpenCL plaforms: " << platforms.size() << endl;

        // Investigate each platform
        cout << endl;
        cout << "**********************" << endl;
        for (vector<cl::Platform>::iterator plat = platforms.begin(); plat != platforms.end(); plat++)
        {
            string s;
            plat->getInfo(CL_PLATFORM_NAME, &s);
            cout << "Platform: " << s << endl;

            plat->getInfo(CL_PLATFORM_VENDOR, &s);
            cout << "\tVendor:  " << s << endl;

            plat->getInfo(CL_PLATFORM_VERSION, &s);
            cout << "\tVersion: " << s << endl;

            // Discover number of devices
            vector<cl::Device> devices;
            plat->getDevices(CL_DEVICE_TYPE_ALL, &devices);
            cout << endl;
            cout << "\tNumber of devices: " << devices.size() << endl;

            // Investigate each device
            for (vector<cl::Device>::iterator dev = devices.begin(); dev != devices.end(); dev++ )
            {
                cout << "\t**********************" << endl;

                dev->getInfo(CL_DEVICE_NAME, &s);
                cout << "\t\tName: " << s << endl;

                dev->getInfo(CL_DEVICE_OPENCL_C_VERSION, &s);
                cout << "\t\tVersion: " << s << endl;

                int i;
                dev->getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &i);
                cout << "\t\tMax. Compute Units: " << i << endl;

                size_t size;
                dev->getInfo(CL_DEVICE_LOCAL_MEM_SIZE, &size);
                cout << "\t\tLocal Memory Size: " << size/1024 << " KB" << endl;

                dev->getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &size);
                cout << "\t\tGlobal Memory Size: " << size/(1024*1024) << " MB" << endl;

                dev->getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &size);
                cout << "\t\tMax Alloc Size: " << size/(1024*1024) << " MB" << endl;

                dev->getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &size);
                cout << "\t\tMax Work-group Total Size: " << size << endl;

                vector<size_t> d;
                dev->getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES, &d);
                cout << "\t\tMax Work-group Dims: (";
                for (vector<size_t>::iterator st = d.begin(); st != d.end(); st++)
                cout << *st << " ";
                cout << "\x08)" << endl;

                cout << "\t**********************" << endl;

            }

            cout << endl;
            cout << "**********************";
            cout << endl;
        }

    }
    catch (cl::Error err)
    {
        cout << "OpenCL Error: " << err.what() << endl;
        cout << "Check cl.h for error codes." << endl;
        exit(-1);
    }
    return 0;
}
