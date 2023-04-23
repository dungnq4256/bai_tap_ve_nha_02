#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 1234

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Tạo socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    // Thiết lập thông tin địa chỉ của server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Kết nối tới server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    // Nhập tên máy tính và danh sách ổ đĩa
    std::string hostname;
    std::cout << "Nhập tên máy tính: ";
    std::getline(std::cin, hostname);

    int num_drives;
    std::cout << "Nhập số ổ đĩa: ";
    std::cin >> num_drives;

    std::string drives;
    std::cout << "Điền chi tiết ổ đĩa(VD: C-500GB;D-250GB;E-250GB): ";
    std::cin.ignore();
    std::getline(std::cin, drives);

    // Đóng gói dữ liệu thành chuỗi
    std::string data = hostname + ";" + std::to_string(num_drives) + ";" + drives;

    // Gửi độ dài của chuỗi
    int len = data.length();
    send(sock, &len, sizeof(int), 0);

    // Gửi chuỗi dữ liệu
    send(sock, data.c_str(), len, 0);

std::cout << "Data sent to server: " << data << std::endl;

// Đóng socket
close(sock);
return 0;
}
