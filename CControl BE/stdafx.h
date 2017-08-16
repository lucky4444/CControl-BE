// stdafx.h: Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once

#include "targetver.h"

// TODO: Hier auf zusätzliche Header, die das Programm erfordert, verweisen.
/*--------------------- Standard  --------------------------*/
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

/*--------------------- MOSQUITTO --------------------------*/
#include <mosquitto.h>
#include <mosquittopp.h>

/*---------------------   BOOST   --------------------------*/
#include <boost/log/core.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/common.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio.hpp>

/*----------------------   own    --------------------------*/
#include "util/constants.h"