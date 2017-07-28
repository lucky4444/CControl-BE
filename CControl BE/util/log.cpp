#include "../stdafx.h"
#include "log.h"
using boost::log::trivial::severity_level;
void log_formatter(const boost::log::record_view &view, boost::log::formatting_ostream &os)
{
	severity_level l = view.attribute_values()["Severity"].extract<severity_level>().get();
	std::string level = "TRACE";
	switch (l) {
	case severity_level::trace:
		level = "TRACE";
		break;
	case severity_level::debug:
		level = "DEBUG";
		break;
	case severity_level::info:
		level = "INFO";
		break;
	case severity_level::warning:
		level = "WARNING";
		break;
	case severity_level::error:
		level = "ERROR";
		break;
	case severity_level::fatal:
		level = "FATAL";
		break;
	}
	os << "[" << boost::posix_time::to_simple_string(view.attribute_values()["TimeStamp"].extract<boost::posix_time::ptime>().get())
		<< "]{" << level << "}: " <<
		view.attribute_values()["Message"].extract<std::string>();
}

//void init_logging(bool writeToFile) {
//	boost::log::add_common_attributes();
//	typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > text_sink;
//	boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();
//
//	if (writeToFile) {
//		boost::shared_ptr<std::ostream> filestream = boost::make_shared< std::ofstream >("CControl.log");
//		// Add a stream to write log to
//		sink->locked_backend()->add_stream(filestream);
//	}
//
//	boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
//	sink->locked_backend()->add_stream(stream);
//	sink->set_formatter(&log_formatter);
//	// Register the sink in the logging core
//	boost::log::core::get()->add_sink(sink);
//}

namespace src = boost::log::sources;

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, src::severity_logger_mt) {
	boost::log::add_common_attributes();

	src::severity_logger_mt<boost::log::trivial::severity_level> logger;

	typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > text_sink;
	boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

	boost::shared_ptr<std::ostream> filestream = boost::make_shared< std::ofstream >("CControl.log");
	// Add a stream to write log to
	sink->locked_backend()->add_stream(filestream);

	boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
	sink->locked_backend()->add_stream(stream);
	sink->set_formatter(&log_formatter);
	// Register the sink in the logging core
	boost::log::core::get()->add_sink(sink);

	return logger;
}