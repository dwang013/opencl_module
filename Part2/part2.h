#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define __CL_ENABLE_EXCEPTIONS	
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

using namespace std;
using namespace cl;

class Exception : public std::exception
{
	const char* out;

public:

	const char* what() const throw()
	{
		return out;
	}

	Exception(const char* in) :exception()
	{
		out = in;
	}

	Exception(string& in) :exception()
	{
		out = in.c_str();
	}
};

template<typename T>
static const char* getOpenCLErrorCodeStr(T input)  //credit: C:\Users\monst\AMD APP SDK\3.0\include\SDKUtil\CLUtil.hpp
{
	int errorCode = (int)input;
	switch (errorCode)
	{
	case CL_DEVICE_NOT_FOUND:
		return "CL_DEVICE_NOT_FOUND";
	case CL_DEVICE_NOT_AVAILABLE:
		return "CL_DEVICE_NOT_AVAILABLE";
	case CL_COMPILER_NOT_AVAILABLE:
		return "CL_COMPILER_NOT_AVAILABLE";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case CL_OUT_OF_RESOURCES:
		return "CL_OUT_OF_RESOURCES";
	case CL_OUT_OF_HOST_MEMORY:
		return "CL_OUT_OF_HOST_MEMORY";
	case CL_PROFILING_INFO_NOT_AVAILABLE:
		return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case CL_MEM_COPY_OVERLAP:
		return "CL_MEM_COPY_OVERLAP";
	case CL_IMAGE_FORMAT_MISMATCH:
		return "CL_IMAGE_FORMAT_MISMATCH";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case CL_BUILD_PROGRAM_FAILURE:
		return "CL_BUILD_PROGRAM_FAILURE";
	case CL_MAP_FAILURE:
		return "CL_MAP_FAILURE";
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:
		return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
		return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case CL_INVALID_VALUE:
		return "CL_INVALID_VALUE";
	case CL_INVALID_DEVICE_TYPE:
		return "CL_INVALID_DEVICE_TYPE";
	case CL_INVALID_PLATFORM:
		return "CL_INVALID_PLATFORM";
	case CL_INVALID_DEVICE:
		return "CL_INVALID_DEVICE";
	case CL_INVALID_CONTEXT:
		return "CL_INVALID_CONTEXT";
	case CL_INVALID_QUEUE_PROPERTIES:
		return "CL_INVALID_QUEUE_PROPERTIES";
	case CL_INVALID_COMMAND_QUEUE:
		return "CL_INVALID_COMMAND_QUEUE";
	case CL_INVALID_HOST_PTR:
		return "CL_INVALID_HOST_PTR";
	case CL_INVALID_MEM_OBJECT:
		return "CL_INVALID_MEM_OBJECT";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
		return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case CL_INVALID_IMAGE_SIZE:
		return "CL_INVALID_IMAGE_SIZE";
	case CL_INVALID_SAMPLER:
		return "CL_INVALID_SAMPLER";
	case CL_INVALID_BINARY:
		return "CL_INVALID_BINARY";
	case CL_INVALID_BUILD_OPTIONS:
		return "CL_INVALID_BUILD_OPTIONS";
	case CL_INVALID_PROGRAM:
		return "CL_INVALID_PROGRAM";
	case CL_INVALID_PROGRAM_EXECUTABLE:
		return "CL_INVALID_PROGRAM_EXECUTABLE";
	case CL_INVALID_KERNEL_NAME:
		return "CL_INVALID_KERNEL_NAME";
	case CL_INVALID_KERNEL_DEFINITION:
		return "CL_INVALID_KERNEL_DEFINITION";
	case CL_INVALID_KERNEL:
		return "CL_INVALID_KERNEL";
	case CL_INVALID_ARG_INDEX:
		return "CL_INVALID_ARG_INDEX";
	case CL_INVALID_ARG_VALUE:
		return "CL_INVALID_ARG_VALUE";
	case CL_INVALID_ARG_SIZE:
		return "CL_INVALID_ARG_SIZE";
	case CL_INVALID_KERNEL_ARGS:
		return "CL_INVALID_KERNEL_ARGS";
	case CL_INVALID_WORK_DIMENSION:
		return "CL_INVALID_WORK_DIMENSION";
	case CL_INVALID_WORK_GROUP_SIZE:
		return "CL_INVALID_WORK_GROUP_SIZE";
	case CL_INVALID_WORK_ITEM_SIZE:
		return "CL_INVALID_WORK_ITEM_SIZE";
	case CL_INVALID_GLOBAL_OFFSET:
		return "CL_INVALID_GLOBAL_OFFSET";
	case CL_INVALID_EVENT_WAIT_LIST:
		return "CL_INVALID_EVENT_WAIT_LIST";
	case CL_INVALID_EVENT:
		return "CL_INVALID_EVENT";
	case CL_INVALID_OPERATION:
		return "CL_INVALID_OPERATION";
	case CL_INVALID_GL_OBJECT:
		return "CL_INVALID_GL_OBJECT";
	case CL_INVALID_BUFFER_SIZE:
		return "CL_INVALID_BUFFER_SIZE";
	case CL_INVALID_MIP_LEVEL:
		return "CL_INVALID_MIP_LEVEL";
	case CL_INVALID_GLOBAL_WORK_SIZE:
		return "CL_INVALID_GLOBAL_WORK_SIZE";
	case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:
		return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
	case CL_PLATFORM_NOT_FOUND_KHR:
		return "CL_PLATFORM_NOT_FOUND_KHR";
		//case CL_INVALID_PROPERTY_EXT:
		//    return "CL_INVALID_PROPERTY_EXT";
	case CL_DEVICE_PARTITION_FAILED_EXT:
		return "CL_DEVICE_PARTITION_FAILED_EXT";
	case CL_INVALID_PARTITION_COUNT_EXT:
		return "CL_INVALID_PARTITION_COUNT_EXT";
	case CL_INVALID_DEVICE_QUEUE:
		return "CL_INVALID_DEVICE_QUEUE";
	case CL_INVALID_PIPE_SIZE:
		return "CL_INVALID_PIPE_SIZE";

	default:
		return "unknown error code";
	}
}


void singleline()
{
	cout << setw(70) << setfill('-') << '-' << setfill(' ') << endl;
}

void doubledline()
{
	singleline();
	singleline();
}

void press_enter()
{
	cout << "press enter to continue..." << endl;
	cin.get();
}

void setup(Context &context, CommandQueue &queue, vector<Kernel> &kernels)
{
	vector<Platform> platforms;
	vector<vector<Device>> devices;

	try
	{
		Platform::get(&platforms);

		for (vector<Platform>::iterator it = platforms.begin(); it != platforms.end(); ++it)
		{
			vector<Device> obj;

			(*it).getDevices(CL_DEVICE_TYPE_ALL, &obj);

			devices.push_back(obj);
		}

		int counter = 0;

		vector<Device*> index;

		for (vector<vector<Device>>::iterator it = devices.begin(); it != devices.end(); ++it)
		{
			for (vector<Device>::iterator i = (*it).begin(); i != (*it).end(); ++i)
			{
				cout << "Device: " << ++counter << endl;

				char* platName; std::size_t size;

				clGetPlatformInfo((*i).getInfo<CL_DEVICE_PLATFORM>(), CL_PLATFORM_NAME, NULL, NULL, &size);

				platName = (char*)malloc(size);

				clGetPlatformInfo((*i).getInfo<CL_DEVICE_PLATFORM>(), CL_PLATFORM_NAME, size, platName, NULL);

				cout << platName << endl;

				free(platName);

				if ((*i).getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_CPU)
					cout << "Type: " << "CPU" << endl;
				else
					cout << "Type: " << "GPU" << endl;

				cout << "Device name: " << (*i).getInfo<CL_DEVICE_NAME>() << endl;

				cout << "Number of compute units: " << (*i).getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << endl;

				cout << "Maximumu work group size: " << (*i).getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << endl;

				vector<std::size_t> items = (*i).getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();

				cout << "Maximum work item sizes :";

				for (unsigned int j = 0; j < items.size(); j++)
				{
					cout << items[j] << '\t';
				}

				cout << endl;

				cout << "Local memory size: " << (*i).getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << endl;

				cout << endl;

				index.push_back(&*i);
			}
		}

		doubledline();

		if (counter)
		{
			int op = 0;

			while (1)
			{
				cout << "please select a device number listed to continue.\nDevice number: ";

				cin >> op;
				cin.clear();
				cin.ignore(9999, '\n');

				if (op > 0 && op <= counter)
					break;
				else
					cerr << "invalid device!\n" << endl;
			}

			context = Context(*index[op - 1]);

			queue = CommandQueue(context, *index[op - 1]);

			try
			{
				fstream file("source.cl", ios::in);

				if (file.is_open())
				{

					string input(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));

					file.close();

					//Program::Sources source; 
					vector<pair<const char*, ::size_t>> source;
					source.push_back(make_pair(input.c_str(), input.size() + 1));

					Program program(context, source);

					vector<Device> contextDevices(context.getInfo<CL_CONTEXT_DEVICES>());

					try
					{
						program.build(contextDevices);
					}
					catch (Error& err)
					{
						if (err.err() != CL_BUILD_PROGRAM_FAILURE)
							throw err;
					}

					for (unsigned int j = 0; j < contextDevices.size(); j++)
						cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[j]) << endl;
					//cout << "build log on " << contextDevices[0].getInfo<CL_DEVICE_NAME>() << ":\n" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[0]) << endl;

					doubledline();

					program.createKernels(&kernels);

					/*cout << "number of kernels in program: " << kernels.size() << endl;

					cout << "kernel names: ";

					for (unsigned int j = 0; j < kernels.size(); j++)
					{
						cout << kernels[j].getInfo < CL_KERNEL_FUNCTION_NAME>() << '\t';
					}*/

					cout << endl;

					doubledline();
				}
				else
					throw Exception("source.cl not found!\n");
			}
			catch (exception& ex)
			{
				cerr << ex.what() << endl;
				press_enter();
				exit(0);
			}
		}
	}
	catch (Error& e)
	{
		cerr << "error: " << e.what() << endl;
		cerr << "error code: " << getOpenCLErrorCodeStr(e.err()) << endl;
		press_enter();
		exit(0);
	}
}


void read_BMP_RGB_to_RGBA(const char *filename, int* widthOut, int* heightOut, vector<unsigned char>& imageData)
{
	char fileHeader[54];	// to store the file header, bmp file format bmpheader (14 bytes) + bmpheaderinfo (40 bytes) = 54 bytes 
	int width, height;		// width and height of image
	int offset;				// offset where image data starts in the file
	int imageSize;			// image size in bytes
	int padSize;			// in the bmp file format, each row must be a multiple of 4
	int rowSize;			// size per row in bytes
	int i, j;
	char colour[3];

	// open file stream
	ifstream textureFileStream(filename, ios::in | ios::binary);

	// check whether file opened successfully
	if (!textureFileStream.is_open())
	{
		static string e("Failed to open texture file - ");
		throw Exception( e+= filename);
	}

	// get file header
	textureFileStream.read(fileHeader, 54);

	// get offset, width and height information
	offset = fileHeader[10];
	width = fileHeader[21] << 24;
	width |= fileHeader[20] << 16;
	width |= fileHeader[19] << 8;
	width |= fileHeader[18];
	width = abs(width);
	height = fileHeader[25] << 24;
	height |= fileHeader[24] << 16;
	height |= fileHeader[23] << 8;
	height |= fileHeader[22];
	height = abs(height);

	// allocate RGBA image data
	imageSize = width * height * 4;
	imageData = vector<unsigned char>(imageSize);

	// move read position by offset
	textureFileStream.seekg(offset, ios::beg);

	// compute amount of row padding
	padSize = width % 4;
	if (padSize != 0) {
		padSize = 4 - padSize;
	}

	// bitmaps are stored in upside-down raster order
	rowSize = width * 4;

	// read each RGB pixel colour
	for (i = 0; i < height; i++) {
		for (j = 0; j < rowSize; j += 4) {
			textureFileStream.read(colour, 3);
			imageData[i*rowSize + j] = colour[0];
			imageData[i*rowSize + j + 1] = colour[1];
			imageData[i*rowSize + j + 2] = colour[2];
			imageData[i*rowSize + j + 3] = 255;
		}
		// in the bmp format, each row has to be a multiple of 4
		// read in the junk data and throw it away
		for (j = 0; j < padSize; j++) {
			textureFileStream.read(colour, 3);
		}
	}

	// close file stream
	textureFileStream.close();

	// record width and height, and return pointer to image data
	*widthOut = width;
	*heightOut = height;
}

void write_BMP_RGBA_to_RGB(const char *filename, vector<unsigned char>& imageData, int width, int height)
{
	char fileHeader[54] = {
		// BITMAPHEADER
		'B','M',		// bmp file
		0, 0, 0, 0,		// file size in bytes
		0, 0,			// reserved
		0, 0,			// reserved
		54, 0, 0, 0,	// offset	
		// BITMAPINFOHEADER
		40, 0, 0, 0,	// size of info header
		0, 0, 0, 0,		// width
		0, 0, 0, 0,		// heigth
		1, 0,			// number colour planes
		24, 0,			// number of bits per pixel
		0, 0, 0, 0,		// compression
		0, 0, 0, 0,		// image size
		0, 0, 0, 0,		// horizontal resolution
		0, 0, 0, 0,		// vertical resolution
		0, 0, 0, 0,		// number of colours in palette
		0, 0, 0, 0,		// number of important colours
	};
	int imageSize;		// image size in bytes
	int padSize;		// in the bmp file format, each row must be a multiple of 4
	int rowSize;		// size per row in bytes
	int fileSize;		// file size in bytes (image size + header size)
	int i, j;
	char colour[3];

	// compute amount of padding so that each row is a multiple of 4
	padSize = width % 4;
	if (padSize != 0) {
		padSize = 4 - padSize;
	}

	// compute image size
	imageSize = (width + padSize) * height * 3;

	// open output stream
	ofstream outFileStream(filename, ios::out | ios::binary);

	// check whether output stream opened successfully
	if (!outFileStream.is_open())
	{
		cerr << "Failed to open output file - " << filename << endl;
		return;
	}

	// compute file size (image size + header size)
	fileSize = 54 + imageSize;

	// fill in appropriate bmp header fields in little endian order
	fileHeader[2] = (unsigned char)fileSize;
	fileHeader[3] = (unsigned char)(fileSize >> 8);
	fileHeader[4] = (unsigned char)(fileSize >> 16);
	fileHeader[5] = (unsigned char)(fileSize >> 24);

	fileHeader[18] = (unsigned char)width;
	fileHeader[19] = (unsigned char)(width >> 8);
	fileHeader[20] = (unsigned char)(width >> 16);
	fileHeader[21] = (unsigned char)(width >> 24);

	fileHeader[22] = (unsigned char)height;
	fileHeader[23] = (unsigned char)(height >> 8);
	fileHeader[24] = (unsigned char)(height >> 16);
	fileHeader[25] = (unsigned char)(height >> 24);

	fileHeader[34] = (unsigned char)imageSize;
	fileHeader[35] = (unsigned char)(imageSize >> 8);
	fileHeader[36] = (unsigned char)(imageSize >> 16);
	fileHeader[37] = (unsigned char)(imageSize >> 24);

	// write file header to out stream
	outFileStream.write(fileHeader, 54);

	// bitmaps are stored in upside-down raster order
	rowSize = width * 4;

	// output each RGB pixel colour
	for (i = 0; i < height; i++) {
		for (j = 0; j < rowSize; j += 4) {
			colour[0] = (unsigned char)imageData[i*rowSize + j];
			colour[1] = (unsigned char)imageData[i*rowSize + j + 1];
			colour[2] = (unsigned char)imageData[i*rowSize + j + 2];
			outFileStream.write(colour, 3);
		}

		// in bmp format rows must be a multiple of 4-bytes, add junk padding if required
		for (j = 0; j < padSize; j++) {
			outFileStream.write(colour, 3);
		}
	}

	// close output file stream
	outFileStream.close();
}
