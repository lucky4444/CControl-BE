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

int VolumeHandler::getVolume()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		throw std::runtime_error("Could not fetch volume because initialization of COM library failed.");
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
		throw std::runtime_error("Could not fetch volume because initialization of device enumerator failed.");
	}

	IMMDevice* device;
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
	pEnumerator->Release();
	if (FAILED(hr)) {
		CoUninitialize();
		if (hr == E_OUTOFMEMORY) {
			throw std::runtime_error("Could not fetch volume because initialization of default audio endpoint failed. Out of memory!");
		}
		else if (hr == E_NOTFOUND) {
			throw std::runtime_error("Could not fetch volume because initialization of default audio endpoint failed. No device available!");
		}
		else {
			throw std::runtime_error("Could not fetch volume because initialization of default audio endpoint failed for unknown reasons.");
		}
	}

	IAudioEndpointVolume* volumeControl;
	const IID IID_IAudioEndpointVolume = __uuidof(IAudioEndpointVolume);
	hr = device->Activate(IID_IAudioEndpointVolume, CLSCTX_ALL, NULL, (void**)&volumeControl);
	device->Release();
	if (FAILED(hr)) {
		CoUninitialize();
		if (hr == E_OUTOFMEMORY) {
			throw std::runtime_error("Could not fetch volume because activating AudioEndpoint failed. Out of memory!");
		}
		else {
			throw std::runtime_error("Could not fetch volume because activating AudioEndpoint failed for unknown reasons.");
		}
	}

	//now actually set the volume
	//float value = (float)volume / 100;
	float volume;
	volumeControl->GetMasterVolumeLevelScalar(&volume);

	//release resources
	volumeControl->Release();

	CoUninitialize();
	int volumePerc = volume * 100;
	return volumePerc;
}

void VolumeHandler::handle(Message msg)
{
	LOG_DEBUG << "Volume change request received.";
	pt::ptree payload;
	std::stringstream stream;
	stream << msg.payload;
	try {
		pt::read_json(stream, payload);
	}
	catch (pt::json_parser_error e) {
		LOG_ERROR << "Failed to set volume because change request contains invalid json.";
		return;
	}

	int volume = payload.get("volume",-1);
	std::string clientid = payload.get("clientid","");
	if (clientid != CLIENTID) {
		LOG_DEBUG << "Received volume change request is for different client. Ignoring it...";
		return;
	}
	
	try {
		updateVolume(volume);
	}
	catch (std::runtime_error e) {
		LOG_ERROR << e.what();
		return;
	}

	LOG_DEBUG << "Volume set to " << volume << "%";
}
