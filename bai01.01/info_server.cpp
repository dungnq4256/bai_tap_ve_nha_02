#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1234

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Tạo socket server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Thiết lập thông tin địa chỉ của server
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Setsockopt error" << std::endl;
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Gắn địa chỉ vào socket server
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // Lắng nghe kết nối từ client
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen error" << std::endl;
        return -1;
    }

    // Chấp nhận kết nối từ client
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Accept error" << std::endl;
        return -1;
    }

    // Đọc độ dài của chuỗi dữ liệu
    int len;
    recv(new_socket, &len, sizeof(int), 0);

    // Nhận chuỗi dữ liệu
    recv(new_socket, buffer, len, 0);

    std::string data(buffer);
    std::cout << "Data received from client: " << data << std::endl;

    // Tách các phần của chuỗi dữ liệu
    std::string delimiter = ";";
    size_t pos = 0;
    std::string token;
    std::string hostname;
    int num_drives;
    std::string drives;
    int count = 0;

    while ((pos = data.find(delimiter)) != std::string::npos) {
        token = data.substr(0, pos);
        if (count == 0) {
            hostname = token;
        } else if (count == 1) {
            num_drives = std::stoi(token);
        } else {
            drives += token + ";";
        }
        data.erase(0, pos + delimiter.length());
        count++;
    }

    std::cout << "Tên máy tính: " << hostname << std::endl;
    std::cout << "Số ổ đĩa: " << num_drives << std::endl;
    std::cout << drives << std::endl;

    // Đóng socket
    close(new_socket);
    close(server_fd);
    return 0;
}