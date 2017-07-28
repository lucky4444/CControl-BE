#pragma once
#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

void log_formatter(const boost::log::record_view &view, boost::log::formatting_ostream &os);

//void init_logging(bool writeToFile);

BOOST_LOG_GLOBAL_LOGGER(logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)

#define LOG(severity) BOOST_LOG_SEV(logger::get(),boost::log::trivial::severity)

#define LOG_FATAL LOG(fatal)
#define LOG_ERROR LOG(error)
#define LOG_WARNING LOG(warning)
#define LOG_INFO LOG(info)
#define LOG_DEBUG LOG(debug)
#define LOG_TRACE LOG(trace)

//typedef boost::log::sources::severity_logger_mt<log_level> CControlLogger;