#ifndef _TRACEREADER_HPP_
#define _TRACEREADER_HPP_

#include <fstream>
#include <string>
#include <queue>
#include <vector>

class TraceReader {
public:
    struct FlitEvent {
        int time;
        int source;
        int destination;
        std::string type;
        long long flitId;
        long long packetId;
        std::string position;
        bool isHead;
        bool isTail;

        // Define comparison operator for priority queue
        bool operator<(const FlitEvent& other) const {
            return time > other.time;  // Earlier times have higher priority
        }
    };

    TraceReader(const std::string& filename);
    ~TraceReader();
    
    // Read all events at current time
    std::vector<FlitEvent> getNextFlits(int current_time);
    bool hasMore() const { return !_trace_queue.empty() || !_eof; }

private:
    void readNextLine();
    
    std::ifstream _trace_file;
    bool _eof;
    std::priority_queue<FlitEvent> _trace_queue;
};

#endif