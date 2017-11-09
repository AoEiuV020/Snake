//
// Created by AoEiuV020 on 2017.11.09-10:43:32.
//

#ifndef SNAKE_FILEUTIL_H
#define SNAKE_FILEUTIL_H

#include <string>

class FileUtil {
public:
    /**
     * 得到文件名，
     * @param filename 文件全路径，
     * @return 返回文件名，
     */
    static std::string basename(const std::string &filename);

    /**
     * 得到文件上级目录，文件分隔符结尾，
     * 不能对根目录使用，
     * @param filename 文件全路径，
     * @return 返回文件上级目录，
     */
    static std::string parent(const std::string &filename);

    /**
     * 得到指定目录下的指定文件，
     * @param parent
     * @param filename
     * @return
     */
    static std::string subFile(const std::string &parent, const std::string &filename);

private:
    static std::string separator;
};


#endif //SNAKE_FILEUTIL_H
