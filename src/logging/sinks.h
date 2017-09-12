#pragma once

#include "Sink.h"
#include <memory>
#include "Formatter.h"
#include <vector>
#include <mutex>

namespace logging {
	enum class Level;

	class LevelFilter : public Sink {
	public:
		LevelFilter(std::shared_ptr<Sink> innerSink, Level minLevel);
		void receive(const Entry& entry) override;
	private:
		std::shared_ptr<Sink> innerSink;
		Level minLevel;
	};

	class StreamSink : public Sink {
	public:
		StreamSink(std::shared_ptr<std::ostream> stream, std::shared_ptr<Formatter> formatter);
		void receive(const Entry& entry) override;
	private:
		std::shared_ptr<std::ostream> stream;
		std::shared_ptr<Formatter> formatter;
	};

	class StdErrSink : public StreamSink {
	public:
		explicit StdErrSink(std::shared_ptr<Formatter> formatter);
	};

	class PausableSink : public Sink {
	public:
		explicit PausableSink(std::shared_ptr<Sink> innerSink);
		void receive(const Entry& entry) override;
		void pause();
		void resume();
	private:
		std::shared_ptr<Sink> innerSink;
		std::vector<Entry> buffer;
		std::mutex mutex;
		bool isPaused = false;
	};

}
