#pragma once

#include <string>
#include <vector>
#include <memory>

namespace amazons {

class BotProcess {
public:
    // Constructor
    BotProcess(const std::string& botPath);
    
    // Destructor
    ~BotProcess();
    
    // Start the bot process
    bool start();
    
    // Stop the bot process
    void stop();
    
    // Check if process is running
    bool isRunning() const;
    
    // Send first turn input with move history
    bool sendFirstTurn(const std::vector<std::string>& moveHistory);
    
    // Send subsequent turn input (opponent's last move)
    bool sendTurn(const std::string& opponentMove);
    
    // Read move output with timeout
    std::string readMove(double timeoutSeconds = 5.0);
    
    // Read keep-running signal
    std::string readKeepRunning(double timeoutSeconds = 0.5);
    
    // Get the bot path
    std::string getBotPath() const;
    
    // Check if in keep-running mode
    bool isKeepRunning() const;
    
private:
    // Read a line with timeout
    std::string readLine(double timeoutSeconds);
    
private:
    // Platform-specific implementation details
    struct Impl;
    std::unique_ptr<Impl> impl;
    
    // Bot executable path
    std::string botPath;
    
    // Keep-running mode flag
    bool keepRunningMode;
};

} // namespace amazons