#include <iostream>
#include <fstream>
#include <thread>
#include <boost/asio.hpp>
#include <atomic>
#include <chrono>
#include <vector>
#include <sstream>
#include <filesystem>

using boost::asio::ip::tcp;

std::atomic<bool> running(true);

void signal_handler(int signal) {
    if (signal == SIGTERM) {
        std::cout << "\nReceived SIGTERM, shutting down server...\n";
        running = false;
    }
}

void stream_ascii_content(tcp::socket& socket, const std::string& filename, const std::string& delimiter = "\x1B[H") {
    try {
        // Open the ASCII art file (e.g., ASCII movie)
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open the file " << filename << std::endl;
            return;
        }

        // Read the entire file into a string
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        size_t pos = 0;
        std::string line;

        // Split by the custom delimiter
        while (running && (pos = content.find(delimiter)) != std::string::npos) {
            line = content.substr(0, pos);  // Get the substring until the delimiter
            content.erase(0, pos + delimiter.length());  // Remove processed part and delimiter from content

            // Write to the socket
            boost::asio::write(socket, boost::asio::buffer(line + delimiter));
            std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Simulate frame delay (100ms)
        }

        // Send the remaining content, if any
        if (!content.empty() && running) {
            boost::asio::write(socket, boost::asio::buffer(content + delimiter));
        }

        std::cout << "Streaming completed or connection closed." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}



void handle_client(tcp::socket socket, const std::string& filename) {
    try {
        std::cout << "New connection from " << socket.remote_endpoint() << std::endl;
        stream_ascii_content(socket, filename);
    } catch (const std::exception& e) {
        std::cerr << "Exception in client thread: " << e.what() << std::endl;
    }
}

void start_telnet_server(boost::asio::io_context& io_context, int port, const std::string& filename) {
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Telnet server running on port " << port << ", streaming " << filename << "..." << std::endl;

    while (running) {
        tcp::socket socket(io_context);
        boost::system::error_code ec;

        acceptor.accept(socket, ec);
        if (ec) {
            std::cerr << "Error accepting connection: " << ec.message() << std::endl;
            continue;
        }

        std::thread(handle_client, std::move(socket), filename).detach();
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Current path: " << std::filesystem::current_path() << std::endl;

    // Setup signal handler for SIGTERM
    std::signal(SIGTERM, signal_handler);

    try {
        boost::asio::io_context io_context;
        std::string filename = "ascii_star_wars.txt";
        int port = 23;

        if (argc > 2) {
            filename = argv[1];
            port = std::stoi(argv[2]);
        }else {
            if (argc > 1) {
                try {
                    port = std::stoi(argv[1]);
                } catch (const std::invalid_argument &e) {
                    filename = argv[1];
                } catch (const std::out_of_range &e) {
                    std::cerr << "Port number out of range. Using default port 23." << std::endl;
                }
            }
        }





        // Start the Telnet server in a separate thread
        std::thread server_thread([&io_context, port, &filename]() {
            start_telnet_server(io_context, port, filename);
        });

        // Wait until SIGTERM is received
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Stop the io_context to clean up Boost.Asio's resources
        io_context.stop();

        // Detach the server thread (it will exit automatically as 'running' is now false)
        server_thread.detach();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "Server shutdown gracefully." << std::endl;
    return 0;
}
