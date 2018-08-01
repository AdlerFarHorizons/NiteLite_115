//	NiteLite_115.cpp
//	This program performs one image capture cycle using the Basler USB camera.
//	An image capture cycle consists of a sequence of images taken with varying
//	exposures. Both raw and TIFF versions of the images are saved to a specified
//	directory and named based on the time the image was taken, the exposure, the
//	image number in the sequence, and the camera ID. Metadata for each image is
//	also logged to the standard output.

// System includes
#include <iostream>

// Pylon API header files
#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCameraArray.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>

// System namespace
using namespace std;

// Pylon and Basler namespaces
using namespace Pylon;
using namespace GenApi;
using namespace Basler_UsbCameraParams;


void detect_cameras(CBaslerUsbInstantCameraArray &cameras)
{
	CTlFactory& tlFactory = CTlFactory::GetInstance();
	DeviceInfoList_t lstDevices;
	int i = 0;

	// Find and configure camera resources
	if ( tlFactory.EnumerateDevices(lstDevices) > 0 )
	{
		cerr << "Found " << lstDevices.size() << " device" << ((lstDevices.size() > 1)? "s" : "") << endl;
		cameras.Initialize(lstDevices.size());

		DeviceInfoList_t::const_iterator it;
		it = lstDevices.begin();
		for ( it = lstDevices.begin(); it != lstDevices.end(); ++it, ++i )
		{
			try
			{
				cameras[i].Attach(tlFactory.CreateDevice(*it));
				cameras[i].Open();
				cameras[i].PixelFormat.SetValue(PixelFormat_BayerRG12);

				cerr << "Camera " << cameras[i].GetDeviceInfo().GetFullName();
				cerr << " sn: " << cameras[i].GetDeviceInfo().GetSerialNumber();
				cerr << " detected" << endl;
			}
			catch (const GenericException &e)
			{
				cerr << "Exception " << e.what() << endl;
			}
		}
	}
	else
	{
		cerr << "No devices found" << endl;
	}
}


int main()
{
	CBaslerUsbInstantCameraArray cameras;

	// Initialize Pylon runtime before using any Pylon methods 
	PylonInitialize();

	try
	{
		detect_cameras(cameras);
	}
	catch (const GenericException &e)
	{
		cerr << "Exception: " << e.what() << endl;
	}

	PylonTerminate();
	exit(0);
}
