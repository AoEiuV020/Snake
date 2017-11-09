//
// Created by AoEiuV020 on 2017.11.09-10:43:32.
//

#include "util/FileUtil.h"

#ifdef _WIN32
std::string FileUtil::separator = "\\";
#else
std::string FileUtil::separator = "/";
#endif

std::string FileUtil::parent(const std::string &filename) {
    if (filename.empty()) {
        return {};
    }
    auto len = filename.length();
    auto index = filename.find_last_of(separator);
    if (index == std::string::npos) {
        return filename;
    }
    if (index + 1 >= len) {
        return parent(filename.substr(0, index));
    }
    return filename.substr(0, index + 1);
}

std::string FileUtil::basename(const std::string &filename) {
    if (filename.empty()) {
        return {};
    }
    auto len = filename.length();
    auto index = filename.find_last_of(separator);
    if (index == std::string::npos) {
        return filename;
    }
    if (index + 1 >= len) {
        len--;
        index = filename.substr(0, len).find_last_of(separator);
        if (len == 0) {
            return filename;
        }
        if (index == 0) {
            return filename.substr(1, len - 1);
        }
        if (index == std::string::npos) {
            return filename.substr(0, len);
        }
        return filename.substr(index + 1, len - index - 1);
    }
    return filename.substr(index + 1, len - index);
}

std::string FileUtil::subFile(const std::string &parent, const std::string &filename) {
    std::string result = parent;
    if (result[result.length() - 1] != separator[0]) {
        result += separator;
    }
    return result + filename;
}
