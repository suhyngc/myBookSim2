#include "tracereader.hpp"
#include <sstream>
#include <iostream>

TraceReader::TraceReader(const std::string& filename) 
    : _eof(false) {
    _trace_file.open(filename.c_str());
    if (!_trace_file.is_open()) {
        std::cerr << "Error: Could not open trace file " << filename << std::endl;
        exit(-1);
    }
    
    // Skip header line
    std::string header;
    std::getline(_trace_file, header);
}

TraceReader::~TraceReader() {
    if (_trace_file.is_open()) {
        _trace_file.close();
    }
}

void TraceReader::readNextLine() {
    std::string line;
    if (std::getline(_trace_file, line)) {
        std::istringstream iss(line);
        FlitEvent event;
        
        iss >> event.time;
        iss >> event.source;
        iss >> event.destination;
        iss >> event.type;
        iss >> event.flitId;
        iss >> event.packetId;
        iss >> event.position;
        iss >> event.isHead;
        iss >> event.isTail;
        
        _trace_queue.push(event);
    } else {
        _eof = true;
    }
}

std::vector<TraceReader::FlitEvent> TraceReader::getNextFlits(int current_time) {
    std::vector<FlitEvent> events;
    
    // Read more events if queue is empty
    while (!_eof && _trace_queue.empty()) {
        readNextLine();
    }
    
    // Get all events for current time
    while (!_trace_queue.empty() && _trace_queue.top().time <= current_time) {
        events.push_back(_trace_queue.top());
        _trace_queue.pop();
        
        // Read next line if queue is empty
        if (_trace_queue.empty() && !_eof) {
            readNextLine();
        }
    }
    
    return events;
}