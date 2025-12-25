#include "ai/BotProcess.hpp"
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <memory>
#include <array>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#endif

namespace amazons {

// Platform-specific implementation
struct BotProcess::Impl {
#ifdef _WIN32
    HANDLE processHandle = INVALID_HANDLE_VALUE;
    HANDLE stdinWrite = INVALID_HANDLE_VALUE;
    HANDLE stdoutRead = INVALID_HANDLE_VALUE;
#else
    pid_t pid = -1;
    int stdinPipe[2] = {-1, -1};
    int stdoutPipe[2] = {-1, -1};
#endif
    
    ~Impl() {
        cleanup();
    }
    
    void cleanup() {
#ifdef _WIN32
        if (stdinWrite != INVALID_HANDLE_VALUE) CloseHandle(stdinWrite);
        if (stdoutRead != INVALID_HANDLE_VALUE) CloseHandle(stdoutRead);
        if (processHandle != INVALID_HANDLE_VALUE) {
            TerminateProcess(processHandle, 0);
            CloseHandle(processHandle);
        }
#else
        if (stdinPipe[0] != -1) close(stdinPipe[0]);
        if (stdinPipe[1] != -1) close(stdinPipe[1]);
        if (stdoutPipe[0] != -1) close(stdoutPipe[0]);
        if (stdoutPipe[1] != -1) close(stdoutPipe[1]);
        if (pid > 0) {
            kill(pid, SIGTERM);
            waitpid(pid, nullptr, 0);
        }
#endif
    }
};

BotProcess::BotProcess(const std::string& botPath)
    : impl(std::make_unique<Impl>()), botPath(botPath), keepRunningMode(false) {
}

BotProcess::~BotProcess() {
    stop();
}

bool BotProcess::start() {
    stop(); // Stop if already running
    
#ifdef _WIN32
    // Windows implementation
    // Create pipes for stdin/stdout
    SECURITY_ATTRIBUTES saAttr = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    
    HANDLE stdinRead, stdoutWrite;
    if (!CreatePipe(&stdinRead, &impl->stdinWrite, &saAttr, 0) ||
        !CreatePipe(&impl->stdoutRead, &stdoutWrite, &saAttr, 0)) {
        return false;
    }
    
    // Create process
    STARTUPINFO si = {sizeof(STARTUPINFO)};
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = stdinRead;
    si.hStdOutput = stdoutWrite;
    si.hStdError = stdoutWrite;
    
    PROCESS_INFORMATION pi;
    std::string cmd = botPath;
    if (!CreateProcess(NULL, cmd.data(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(stdinRead);
        CloseHandle(stdoutWrite);
        return false;
    }
    
    impl->processHandle = pi.hProcess;
    CloseHandle(pi.hThread);
    CloseHandle(stdinRead);
    CloseHandle(stdoutWrite);
    
#else
    // Unix implementation
    if (pipe(impl->stdinPipe) == -1 || pipe(impl->stdoutPipe) == -1) {
        return false;
    }
    
    impl->pid = fork();
    if (impl->pid == 0) {
        // Child process
        close(impl->stdinPipe[1]); // Close write end of stdin
        close(impl->stdoutPipe[0]); // Close read end of stdout
        
        dup2(impl->stdinPipe[0], STDIN_FILENO);
        dup2(impl->stdoutPipe[1], STDOUT_FILENO);
        
        execl(botPath.c_str(), botPath.c_str(), nullptr);
        exit(1); // exec failed
    } else if (impl->pid > 0) {
        // Parent process
        close(impl->stdinPipe[0]); // Close read end of stdin
        close(impl->stdoutPipe[1]); // Close write end of stdout
        
        // Set non-blocking mode for stdout
        int flags = fcntl(impl->stdoutPipe[0], F_GETFL, 0);
        fcntl(impl->stdoutPipe[0], F_SETFL, flags | O_NONBLOCK);
    } else {
        // Fork failed
        close(impl->stdinPipe[0]);
        close(impl->stdinPipe[1]);
        close(impl->stdoutPipe[0]);
        close(impl->stdoutPipe[1]);
        return false;
    }
#endif
    
    keepRunningMode = false;
    return true;
}

void BotProcess::stop() {
    if (impl) {
        impl->cleanup();
    }
    keepRunningMode = false;
}

bool BotProcess::isRunning() const {
#ifdef _WIN32
    if (impl->processHandle == INVALID_HANDLE_VALUE) return false;
    DWORD exitCode;
    GetExitCodeProcess(impl->processHandle, &exitCode);
    return exitCode == STILL_ACTIVE;
#else
    if (impl->pid <= 0) return false;
    int status;
    return waitpid(impl->pid, &status, WNOHANG) == 0;
#endif
}

bool BotProcess::sendFirstTurn(const std::vector<std::string>& moveHistory) {
    if (!isRunning()) return false;
    
    // For Amazons simple interaction protocol
    // First line: turn_id (1 for first turn)
    // Then 2*turn_id - 1 lines of move history
    // For first turn: 1 line of move history which is "-1 -1 -1 -1 -1 -1" for black player
    std::string input = "1\n";
    
    if (moveHistory.empty()) {
        // First move of the game, bot is black
        input += "-1 -1 -1 -1 -1 -1\n";
    } else {
        // Should have move history
        for (const auto& move : moveHistory) {
            input += move + "\n";
        }
    }
    
#ifdef _WIN32
    DWORD written;
    return WriteFile(impl->stdinWrite, input.c_str(), input.size(), &written, NULL) && written == input.size();
#else
    ssize_t written = write(impl->stdinPipe[1], input.c_str(), input.size());
    return written == static_cast<ssize_t>(input.size());
#endif
}

bool BotProcess::sendTurn(const std::string& opponentMove) {
    if (!isRunning() || !keepRunningMode) return false;
    
    std::string input = opponentMove + "\n";
    
#ifdef _WIN32
    DWORD written;
    return WriteFile(impl->stdinWrite, input.c_str(), input.size(), &written, NULL) && written == input.size();
#else
    ssize_t written = write(impl->stdinPipe[1], input.c_str(), input.size());
    return written == static_cast<ssize_t>(input.size());
#endif
}

std::string BotProcess::readMove(double timeoutSeconds) {
    return readLine(timeoutSeconds);
}

std::string BotProcess::readKeepRunning(double timeoutSeconds) {
    return readLine(timeoutSeconds);
}

std::string BotProcess::readLine(double timeoutSeconds) {
    if (!isRunning()) return "";
    
    std::string line;
    auto startTime = std::chrono::steady_clock::now();
    
    while (std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count() < timeoutSeconds) {
        char ch;
#ifdef _WIN32
        DWORD bytesRead;
        if (ReadFile(impl->stdoutRead, &ch, 1, &bytesRead, NULL) && bytesRead == 1) {
            if (ch == '\n') break;
            line += ch;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
#else
        ssize_t bytesRead = ::read(impl->stdoutPipe[0], &ch, 1);
        if (bytesRead == 1) {
            if (ch == '\n') break;
            line += ch;
        } else if (bytesRead == 0) {
            // EOF
            break;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else {
            break;
        }
#endif
    }
    
    return line;
}

std::string BotProcess::getBotPath() const {
    return botPath;
}

bool BotProcess::isKeepRunning() const {
    return keepRunningMode;
}

} // namespace amazons