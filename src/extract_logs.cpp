#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = filesystem; // C++17 feature for directory handling

void extractLogs(const string &date) {
    string logFilePath = "test_logs.log";  // The large log file
    string outputDir = "output";
    string outputFilePath = outputDir + "/output_" + date + ".txt";

    // Ensure output directory exists
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    ifstream infile(logFilePath);  // Open log file for reading
    if (!infile) {
        cerr << "Error: Could not open log file '" << logFilePath << "'" << endl;
        return;
    }

    ofstream outfile(outputFilePath);  // Open output file for writing
    if (!outfile) {
        cerr << "Error: Could not create output file '" << outputFilePath << "'" << endl;
        return;
    }

    vector<string> buffer;  // Buffer to store matching lines
    const size_t BUFFER_SIZE = 10000;  // Adjust buffer size for performance

    string line;
    while (getline(infile, line)) {
        if (line.rfind(date, 0) == 0) { // Efficiently check if line starts with date
            buffer.push_back(line);

            // Flush buffer to file when full
            if (buffer.size() >= BUFFER_SIZE) {
                for (const string &log : buffer) {
                    outfile << log << "\n";
                }
                buffer.clear();  // Clear buffer after writing
            }
        }
    }

    // Write remaining buffered logs to file
    for (const string &log : buffer) {
        outfile << log << "\n";
    }

    cout << "Logs for " << date << " extracted to " << outputFilePath << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./extract_logs YYYY-MM-DD" << endl;
        return 1;
    }

    string inputDate = argv[1];
    extractLogs(inputDate);
    return 0;
}
