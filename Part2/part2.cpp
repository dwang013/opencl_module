#include "part2.h"

int main()
{
	string fileName;

	cout << "enter the image file name: ";

	getline(cin, fileName);

	vector<cl_uchar> imgData;
	int imgWidth, imgHeight;

	try
	{
		read_BMP_RGB_to_RGBA(fileName.c_str(), &imgWidth, &imgHeight, imgData);

		vector<cl_uchar> imgOut(imgData.size());

		Context context;

		CommandQueue queue;
		vector<Kernel> kernels;

		setup(context, queue, kernels);
		ImageFormat imgFormat(CL_RGBA, CL_UNORM_INT8);
		Image2D input(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, &imgData[0]);
		Image2D output(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, &imgOut[0]);

		cl::size_t<3> origin, region;
		origin[0] = origin[1] = origin[2] = 0;
		region[0] = imgWidth;
		region[1] = imgHeight;
		region[2] = 1;

		NDRange offset(0, 0), globalSize(imgWidth, imgHeight);

		kernels[0].setArg(0, input);
		kernels[0].setArg(1, output);

		queue.enqueueNDRangeKernel(kernels[0], offset, globalSize);

		queue.enqueueReadImage(output, CL_TRUE, origin, region, 0, 0, &imgOut[0]);

		write_BMP_RGBA_to_RGB(fileName.insert(fileName.size() - 4, "B&W").c_str(), imgOut, imgWidth, imgHeight);

		fileName.erase(fileName.size() - 6, 3);

		cout << "operation completed successfully!" << endl;

		doubledline();

		cl_float total=0, avg;
		vector<cl_float> luminance;
		for (int i = 0; i < imgOut.size(); i+=4) 
		{
				 luminance.push_back(imgOut[i]*0.299 + imgOut[i + 1]*0.587 + imgOut[i + 2]*0.114);
				 total += luminance.back();
		}

		avg = total / (luminance.size());

		cout << "the average luminance for task b is: " << avg << endl;

		//cout << luminance.size() << endl;

		doubledline();

		std::size_t workgroupSize = queue.getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
		std::size_t localMemorySize = queue.getInfo<CL_QUEUE_DEVICE>().getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
		cl_ulong kernelWorkgroupSize = kernels[1].getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(queue.getInfo<CL_QUEUE_DEVICE>());

		// display the information
		std::cout << "Max workgroup size: " << workgroupSize << std::endl;
		std::cout << "Local memory size: " << localMemorySize << std::endl;
		std::cout << "Kernel workgroup size: " << kernelWorkgroupSize << std::endl;

		// if kernel only allows one work-item per work-group, abort
		if (kernelWorkgroupSize == 1)
			throw Exception("Abort: Cannot run reduction kernel, because kernel workgroup size is 1.");

		// if allowed kernel work group size smaller than device's max workgroup size
		if (workgroupSize > kernelWorkgroupSize)
			workgroupSize = kernelWorkgroupSize;

		// ensure sufficient local memory is available
		while (localMemorySize < sizeof(cl_float) * workgroupSize * 4)
		{
			workgroupSize /= 4;
		}

		cl_float sum;

		Buffer dataBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * luminance.size(), &luminance[0]);
		Buffer sumBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float), &sum);

		LocalSpaceArg localSpace = cl::Local(sizeof(cl_float) * workgroupSize * 4);

		// set kernel arguments
		kernels[1].setArg(0, dataBuffer);
		kernels[1].setArg(1, localSpace);

		kernels[2].setArg(0, dataBuffer);
		kernels[2].setArg(1, localSpace);
		kernels[2].setArg(2, sumBuffer);

		std::size_t global = luminance.size() / 4;

		NDRange offset2(0);
		NDRange globalSize2(global);
		NDRange localSize(workgroupSize);

		queue.enqueueNDRangeKernel(kernels[1], offset2, globalSize2, localSize);

		cout << "Global size: " << global << endl;

		// run reduction kernel until work-items can fit within one work-group
		while (global / workgroupSize > workgroupSize)
		{
			global /= workgroupSize;
			globalSize2 = global;
			queue.enqueueNDRangeKernel(kernels[1], offset2, globalSize2, localSize);
			std::cout << "Global size: " << global << std::endl;
		}

		global /= workgroupSize;
		globalSize2 = global;
		queue.enqueueNDRangeKernel(kernels[2], offset2, globalSize2);

		queue.finish();

		queue.enqueueReadBuffer(sumBuffer, CL_TRUE, 0, sizeof(cl_float), &sum);

		cout << "the average luminance for task c is: " << sum / luminance.size() << endl;

		//cout << luminance.size() % 4 << endl;

		doubledline();
	}
	catch (Error& e)
	{
		cerr << "error: " << e.what() << endl;
		cerr << "error code: " << getOpenCLErrorCodeStr(e.err()) << endl;
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}

	press_enter();
}