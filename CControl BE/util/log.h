#pragma once

void log_formatter(const boost::log::record_view &view, boost::log::formatting_ostream &os);

void init_logging(bool writeToFile);

enum log_level {
	ERR,
	WARNING,
	INFO,
	NOTICE,
	DEBUG
};

typedef boost::log::sources::severity_logger_mt<log_level> CControlLogger;