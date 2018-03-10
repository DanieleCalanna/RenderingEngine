#include "CL/ocl_boiler.h"

void ocl_check(cl_int err, const char *msg, ...)
{
	if (err != CL_SUCCESS)
	{
		char msg_buf[BUFSIZE + 1];
		va_list ap;
		va_start(ap, msg);
		vsnprintf(msg_buf, BUFSIZE, msg, ap);
		va_end(ap);
		msg_buf[BUFSIZE] = '\0';
		fprintf(stderr, "%s - error %d\n", msg_buf, err);
		exit(1);
	}
}

// Return the ID of the platform specified in the OCL_PLATFORM
// environment variable (or the first one if none specified)
cl_platform_id select_platform(cl_uint platformIndex)
{
	cl_uint nplats;
	cl_int err;
	cl_platform_id *plats;

	err = clGetPlatformIDs(0, NULL, &nplats);
	ocl_check(err, "counting platforms");

	plats = new cl_platform_id[nplats];

	err = clGetPlatformIDs(nplats, plats, NULL);
	ocl_check(err, "getting platform IDs");

	if (platformIndex >= nplats)
	{
		fprintf(stderr, "no platform number %u", platformIndex);
		exit(1);
	}

	cl_platform_id choice = plats[platformIndex];

	char buffer[BUFSIZE];

	err = clGetPlatformInfo(choice, CL_PLATFORM_NAME, BUFSIZE,
		buffer, NULL);
	ocl_check(err, "getting platform name");

	printf(" %d Platforms: %d > %s\n", nplats, platformIndex, buffer);

	return choice;
}

// Return the ID of the device (of the given platform p) specified in the
// OCL_DEVICE environment variable (or the first one if none specified)
cl_device_id select_device(cl_platform_id p, cl_uint deviceIndex)
{
	cl_uint ndevs;
	cl_int err;
	cl_device_id *devs;

	err = clGetDeviceIDs(p, CL_DEVICE_TYPE_ALL, 0, NULL, &ndevs);
	ocl_check(err, "counting devices");

	devs = new cl_device_id[ndevs];

	err = clGetDeviceIDs(p, CL_DEVICE_TYPE_ALL, ndevs, devs, NULL);
	ocl_check(err, "devices #2");

	if (deviceIndex >= ndevs)
	{
		fprintf(stderr, "no device number %u", deviceIndex);
		exit(1);
	}

	cl_device_id choice = devs[deviceIndex];

	char buffer[BUFSIZE];

	err = clGetDeviceInfo(choice, CL_DEVICE_NAME, BUFSIZE,
		buffer, NULL);
	ocl_check(err, "device name");

	printf(" %d Devices: %d > %s\n", ndevs, deviceIndex, buffer);

	return choice;
}

// Create a one-device context
cl_context create_context(cl_platform_id p, cl_device_id d)
{
	cl_int err;

	cl_context_properties ctx_prop[] = {
		CL_CONTEXT_PLATFORM, (cl_context_properties)p, 0
	};

	cl_context ctx = clCreateContext(ctx_prop, 1, &d,
		NULL, NULL, &err);
	ocl_check(err, "create context");

	return ctx;
}

// Create a command queue for the given device in the given context
cl_command_queue create_queue(cl_context ctx, cl_device_id d)
{
	cl_int err;

	cl_command_queue que = clCreateCommandQueue(ctx, d,
		CL_QUEUE_PROFILING_ENABLE, &err);
	ocl_check(err, "create queue");
	return que;
}

// Compile the device part of the program, stored in the external
// file `fname`, for device `dev` in context `ctx`
cl_program create_program(const char * const fname, cl_context ctx, cl_device_id dev)
{
	cl_int err, errlog;
	cl_program prg;

	char src_buf[BUFSIZE + 1];
	char *log_buf = NULL;
	size_t logsize;
	const char* buf_ptr = src_buf;
	time_t now = time(NULL);

	memset(src_buf, 0, BUFSIZE);

	//snprintf(src_buf, BUFSIZE, " // %s #include \"%s\"\n", ctime(&now), fname);
	snprintf(src_buf, BUFSIZE, " #include \"%s\"\n", fname);
	printf("\n > Compiling:\n%s", src_buf);
	prg = clCreateProgramWithSource(ctx, 1, &buf_ptr, NULL, &err);
	ocl_check(err, "create program");

	err = clBuildProgram(prg, 1, &dev, "-I.", NULL, NULL);
	errlog = clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_LOG, 0, NULL, &logsize);
	ocl_check(errlog, "get program build log size");
	log_buf = new char[logsize];
	errlog = clGetProgramBuildInfo(prg, dev, CL_PROGRAM_BUILD_LOG, logsize, log_buf, NULL);
	ocl_check(errlog, "get program build log");
	while (logsize > 0 &&
		(log_buf[logsize - 1] == '\n' ||
			log_buf[logsize - 1] == '\0'))
	{
		logsize--;
	}
	if (logsize > 0)
	{
		log_buf[logsize] = '\n';
		log_buf[logsize + 1] = '\0';
	}
	else
	{
		log_buf[logsize] = '\0';
	}

	if (strlen(log_buf) != 0)
	{
		printf("\n==================== BUILD LOG =====================\n");
		printf("%s", log_buf);
	}
	ocl_check(err, "build program");
	return prg;
}

// Runtime of an event, in nanoseconds. Note that if NS is the
// runtimen of an event in nanoseconds and NB is the number of byte
// read and written during the event, NB/NS is the effective bandwidth
// expressed in GB/s
cl_ulong runtime_ns(cl_event evt)
{
	cl_int err;
	cl_ulong Start, end;
	err = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_START, sizeof(Start), &Start, NULL);
	ocl_check(err, "get start");
	err = clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_END, sizeof(end), &end, NULL);
	ocl_check(err, "get end");
	return (end - Start);
}

// Runtime of an event, in milliseconds
double runtime_ms(cl_event evt) { return runtime_ns(evt)*1.0e-6; }

/* round gws to the next multiple of lws */
size_t round_mul_up(size_t gws, size_t lws) { return ((gws + lws - 1) / lws)*lws; }