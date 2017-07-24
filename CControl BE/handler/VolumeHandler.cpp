#include "../stdafx.h"
#include "VolumeHandler.h"


VolumeHandler::VolumeHandler()
{

}

void VolumeHandler::handle(Message msg)
{
	BOOST_LOG_SEV(lg, DEBUG) << msg.payload;
}

VolumeHandler::~VolumeHandler()
{
}
