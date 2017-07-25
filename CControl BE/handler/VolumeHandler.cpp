#include "../stdafx.h"
#include "VolumeHandler.h"
#include <Windows.h>
#include <Mmdeviceapi.h>
#include <endpointvolume.h>

namespace pt = boost::property_tree;

void VolumeHandler::updateVolume(int volume)
{
	if (volume < 0 || volume > 100) {
		std::stringstream ret;
		ret << "Invalid volume value: " << volume;
		throw std::runtime_error(ret.str());
	}

	HRESULT hr =  CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		throw std::runtime_error("Could not set volume because initialization of COM library failed.");
	}
	
	IMMDeviceEnumerator* pEnumerator;
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator);
	if (FAILED(hr)) {
		CoUninitialize();
		throw std::runtime_error("Could not set volume because initialization of device enumerator failed.");
	}

	IMMDevice* device;
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
	pEnumerator->Release();
	if (FAILED(hr)) {
		CoUninitialize();
		if (hr == E_OUTOFMEMORY) {
			throw std::runtime_error("Could not set volume because fetching of default audio endpoint failed. Out of memory!");
		}
		else if (hr == E_NOTFOUND) {
			throw std::runtime_error("Could not set volume because fetching of default audio endpoint failed. No device available!");
		}
		else {
			throw std::runtime_error("Could not set volume because fetching of default audio endpoint failed for unknown reasons.");
		}
	}

	IAudioEndpointVolume* volumeControl;
	const IID IID_IAudioEndpointVolume = __uuidof(IAudioEndpointVolume);
	hr = device->Activate(IID_IAudioEndpointVolume, CLSCTX_ALL, NULL, (void**)&volumeControl);
	device->Release();
	if (FAILED(hr)) {
		CoUninitialize();
		if (hr == E_OUTOFMEMORY) {
			throw std::runtime_error("Could not set volume because activating AudioEndpoint failed. Out of memory!");
		}
		else {
			throw std::runtime_error("Could not set volume because activating AudioEndpoint failed for unknown reasons.");
		}
	}

	//now actually set the volume
	float value = (float)volume / 100;
	volumeControl->SetMasterVolumeLevelScalar(value, NULL);

	//release resources
	volumeControl->Release();
	CoUninitialize();
}

void VolumeHandler::handle(Message msg)
{
	pt::ptree payload;
	std::stringstream stream;
	stream << msg.payload;
	pt::read_json(stream, payload);
	int volume = payload.get("volume",-1);
	std::string clientid = payload.get<std::string>("clientid");
	if (clientid != CLIENTID) {
		BOOST_LOG_SEV(lg, log_level::DEBUG) << "Volume change request for different client received. Ignoring it...";
		return;
	}
	
	BOOST_LOG_SEV(lg, log_level::DEBUG) << "Volume change request received.";
	
	try {
		updateVolume(volume);
	}
	catch (std::runtime_error e) {
		BOOST_LOG_SEV(lg, log_level::ERR) << e.what();
		return;
	}

	BOOST_LOG_SEV(lg, log_level::DEBUG) << "Volume set to " << volume << "%";
}
